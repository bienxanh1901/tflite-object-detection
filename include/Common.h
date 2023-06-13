#pragma once

#include <opencv2/opencv.hpp>

typedef struct
{
    cv::Rect rec;
    int classID;
    float score;
    std::string className;
} Object;

float iou(cv::Rect &rectA, cv::Rect &rectB);

void nms(std::vector<Object> &boxes, const double nmsThreshold);