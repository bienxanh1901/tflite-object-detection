#pragma once

#include "Common.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/string_util.h"
#include "tensorflow/lite/model.h"
#include <opencv2/opencv.hpp>

typedef struct
{
    int batchSize;
    int height;
    int width;
    int channels;
    int dataType;
} InputShape;

class ObjectDetector
{

public:
    static ObjectDetector *GetInstance();
    ~ObjectDetector();

    void ShowModelInfo();

    std::vector<Object> Invoke(cv::Mat &image);

protected:
    void PreProcess(cv::Mat &image0);
    std::vector<Object> PostProcess(int imageHeight, int imageWidth);

private:
    ObjectDetector();

    void LoadModel();
    void ReadLabels();
    void BuildInterpreter();
    void Configure();

private:
    std::unique_ptr<tflite::FlatBufferModel> model;
    std::unique_ptr<tflite::Interpreter> interpreter;
    std::vector<std::string> labels;
    InputShape inputShape = {0};
    int numberInvokingThreads = 2;
    float threshold = 0.5;
};