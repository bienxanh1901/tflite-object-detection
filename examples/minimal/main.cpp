
#include "ObjectDetector.h"
#include <opencv2/opencv.hpp>

int main()
{
  ObjectDetector *objectDetector = ObjectDetector::GetInstance();

  objectDetector->ShowModelInfo();

  cv::Mat img = imread("../demo_data/image.jpg", cv::IMREAD_COLOR);

  if (img.empty())
  {
    std::cout << "Could not read the image: " << std::endl;
    return 1;
  }

  std::vector<Object> results = objectDetector->Invoke(img);

  std::vector<Object>::iterator it = results.begin();

  for (; it != results.end(); it++)
  {
    std::cout << it->className << " " << it->score << std::endl;
  }
}