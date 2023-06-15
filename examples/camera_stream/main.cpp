
#include "ObjectDetector.h"
#include "Config.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

int main()
{
    ObjectDetector *objectDetector = ObjectDetector::GetInstance();
    objectDetector->ShowModelInfo();

    // Get camera source
    std::string cameraSource = Config::GetInstance()->GetConfigParam(CFG_CAMERA_SOURCE);

    if (cameraSource.empty())
    {
        std::cout << "Could not find camera source.\nPlease set your " << CFG_CAMERA_SOURCE
                  << " in config.txt" << std::endl;
        return 1;
    }

    cv::Mat frame;
    cv::VideoCapture cap;

    cap.open(cameraSource);
    if (!cap.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera " << cameraSource << std::endl;
        std::cout << cv::getBuildInformation() << std::endl;
        return -1;
    }

    while (true)
    {
        cap.read(frame);
        // check if we succeeded
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        std::vector<Object> results = objectDetector->Invoke(frame);

        for (std::vector<Object>::iterator it = results.begin(); it != results.end(); it++)
        {
            std::cout << it->className << " " << it->score << std::endl;
        }
    }

    return 0;
}