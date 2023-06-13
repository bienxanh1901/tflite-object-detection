#include <fstream>
#include "ObjectDetector.h"
#include "Config.h"

/**
 * @brief Get a singleton instance of ObjectDetector
 *
 * @return ObjectDetector*
 */
ObjectDetector *ObjectDetector::GetInstance()
{
    static ObjectDetector *instance;

    if (!instance)
    {
        instance = new ObjectDetector();
    }

    return instance;
}

/**
 * @brief Construct a new Object Detector:: Object Detector object
 *
 */
ObjectDetector::ObjectDetector()
{
    
    // Load model
    this->LoadModel();
    // Read labels map
    this->ReadLabels();
    // Build interpreter
    this->BuildInterpreter();
    // Config the detector
    this->Configure();
    // Set detection threshold
    std::string threshold = Config::GetInstance()->GetConfigParam(CFG_THRESHOLD);
    if (!threshold.empty())
    {
        this->threshold = std::stof(threshold);
    }
    else
    {
       this->threshold = 0.5;
    }
}

/**
 * @brief Destroy the Object Detector:: Object Detector object
 *
 */
ObjectDetector::~ObjectDetector()
{
}

/**
 * @brief Show the model information
 *
 */
void ObjectDetector::ShowModelInfo()
{
    std::cout << "Model infomation:" << std::endl;
    std::cout << "\tBactch Size        = " << this->inputShape.batchSize << std::endl;
    std::cout << "\tInput Image Height = " << this->inputShape.height << std::endl;
    std::cout << "\tInput Image Width  = " << this->inputShape.width << std::endl;
    std::cout << "\tNumber Channels    = " << this->inputShape.channels << std::endl;
    std::cout << "\tInput Type         = " << this->inputShape.dataType << std::endl;
}


/**
 * @brief Load the model from config
 * 
 */
void ObjectDetector::LoadModel()
{
    std::string modelPath = Config::GetInstance()->GetConfigParam(CFG_MODEL_PATH);

    std::cout << "Loading Model" << std::endl;
    this->model = tflite::FlatBufferModel::BuildFromFile(modelPath.c_str());

    if (this->model == nullptr)
    {
        std::cout << "Cannot Load tf model: " << modelPath << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Loaded Model: " << modelPath << std::endl;
    }
}

/**
 * @brief Read list of labels from config
 * 
 */
void ObjectDetector::ReadLabels()
{
    std::string labelsPath = Config::GetInstance()->GetConfigParam(CFG_LABEL_PATH);

    std::cout << "Reading Labels" << std::endl;
    if (labelsPath.empty())
    {
        std::cout << "Please specify the labels.txt file path" << std::endl;
        exit(1);
    }
    else
    {
        std::ifstream ifs(labelsPath.c_str(), std::ifstream::in);

        if (ifs.is_open())
        {
            std::string line;

            while (std::getline(ifs, line))
            {
                this->labels.push_back(line);
            }

            ifs.close();

            std::cout << "Loaded labels file: " << labelsPath << std::endl;
            std::cout << "Number of classes: " << this->labels.size() << std::endl;
            
        }
        else
        {
            std::cout << "Cannot open labels file: " << labelsPath << std::endl;
            exit(1);
        }
    }
}

/**
 * @brief Build the interpreter with the InterpreterBuilder.
 * 
 */
void ObjectDetector::BuildInterpreter()
{
    std::cout << "Building Interpreter" << std::endl;

    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder builder(*this->model, resolver);
    builder(&this->interpreter);

    if (this->interpreter == nullptr)
    {
        std::cout << "Cannot build the interpreter " << std::endl;
        exit(1);
    }
}

/**
 * @brief Configure the detector
 *
 */
void ObjectDetector::Configure()
{
    // get input shape
    const std::vector<int> &inputs = interpreter->inputs();
    TfLiteTensor *tensor = interpreter->tensor(inputs[0]);

    this->inputShape.batchSize = tensor->dims->data[0];
    this->inputShape.height = tensor->dims->data[1];
    this->inputShape.width = tensor->dims->data[2];
    this->inputShape.channels = tensor->dims->data[3];
    this->inputShape.dataType = tensor->type;

    // Allocate tensor
    if (interpreter->AllocateTensors() != kTfLiteOk)
    {
        std::cout << "Failed to allocate tensor" << std::endl;
        exit(1);
    }

    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(this->numberInvokingThreads);
}

/**
 * @brief Preprocess image before invoking
 *  - Resize image to the input size
 *  - Normalize image data
 *  - Fill the input buffer
 *  - ...
 * @note This Preprocessor is used for model ssd_mobilenet_v1 
 *       (https://tfhub.dev/tensorflow/lite-model/ssd_mobilenet_v1/1/default/1)
 *       (https://www.tensorflow.org/lite/examples/object_detection/overview)
 *       To preprocess the input for other models, you need to custom this 
 *       function base on model input
 * 
 * @param cv::Mat input image
 */
void ObjectDetector::PreProcess(cv::Mat &image0)
{
    cv::Mat image;

    // Resize the image to model input size
    cv::resize(image0, image, cv::Size(this->inputShape.width, this->inputShape.height), cv::INTER_NEAREST);

    memcpy(this->interpreter->typed_input_tensor<uchar>(0), image.data, image.total() * image.elemSize());
}

/**
 * @brief Get object from outputs
 * 
 *  * @note This Postprocessor is used for model ssd_mobilenet_v1 
 *       (https://tfhub.dev/tensorflow/lite-model/ssd_mobilenet_v1/1/default/1)
 *       (https://www.tensorflow.org/lite/examples/object_detection/overview)
 *       To handle the output for other models, you need to custom this 
 *       function base on model output 
 * 
 * @param imageHeight 
 * @param imageWidth 
 * @return std::vector<Object> 
 */
std::vector<Object> ObjectDetector::PostProcess(int imageHeight, int imageWidth)
{
    std::vector<Object> objects;

    std::vector<int> outputs = this->interpreter->outputs();

    float* bboxes = this->interpreter->tensor(outputs[0])->data.f;
    float* classes = this->interpreter->tensor(outputs[1])->data.f;
    float* scores = this->interpreter->tensor(outputs[2])->data.f;
    int nums = *this->interpreter->tensor(outputs[3])->data.f;

    for(int i = 0; i < nums; i++){
        if(scores[i] > this->threshold){
            auto ymin = bboxes[4*i] * imageHeight;
            auto xmin = bboxes[4*i + 1] * imageWidth;
            auto ymax = bboxes[4*i + 2] * imageHeight;
            auto xmax = bboxes[4*i + 3] * imageWidth;
            auto width = xmax - xmin;
            auto height = ymax - ymin;

            Object object;
            object.classID = classes[i];
            object.className = this->labels.at(object.classID);
            object.rec.x = xmin;
            object.rec.y = ymin;
            object.rec.width = width;
            object.rec.height = height;
            object.score = scores[i];
            objects.push_back(object);
        }
    }

    return objects;
}


/**
 * @brief Detect objects in image
 * - Preprocess input
 * - Invoke image
 * - Postprocess output
 * 
 * @param image 
 * @return std::vector<Object> 
 */
std::vector<Object> ObjectDetector::Invoke(cv::Mat &image)
{
    int imageHeight = image.rows;
    int imageWidth = image.cols;

    // Preprocess data
    this->PreProcess(image);

    TfLiteStatus status = this->interpreter->Invoke();
    if (status != kTfLiteOk)
    {
        std::cout << "Failed to run the inference!" << std::endl;
        exit(1);
    }

    return this->PostProcess(imageHeight, imageWidth);
}