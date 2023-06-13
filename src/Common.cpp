#include "Common.h"

/**
 * @brief Calculate Intersection Over Union
 *
 * @param rectA
 * @param rectB
 * @return The Intersection Over Union value
 */
float iou(cv::Rect &rectA, cv::Rect &rectB)
{
    int x1 = std::max(rectA.x, rectB.x);
    int y1 = std::max(rectA.y, rectB.y);
    int x2 = std::min(rectA.x + rectA.width, rectB.x + rectB.width);
    int y2 = std::min(rectA.y + rectA.height, rectB.y + rectB.height);
    int w = std::max(0, (x2 - x1 + 1));
    int h = std::max(0, (y2 - y1 + 1));
    float inter = w * h;
    float areaA = rectA.width * rectA.height;
    float areaB = rectB.width * rectB.height;
    float o = inter / (areaA + areaB - inter);
    return (o >= 0) ? o : 0;
}

/**
 * @brief Non Maximum Suppression algorithm
 *
 * @param boxes
 * @param nmsThreshold
 */

void nms(std::vector<Object> &boxes, const double nmsThreshold)
{
    std::vector<int> scores;

    for (int i = 0; i < boxes.size(); i++)
    {
        scores.push_back(boxes[i].score);
    }

    std::vector<int> index;

    for (int i = 0; i < scores.size(); ++i)
    {
        index.push_back(i);
    }

    sort(index.begin(), index.end(), [&](int a, int b)
         { return scores[a] > scores[b]; });

    std::vector<bool> del(scores.size(), false);

    for (size_t i = 0; i < index.size(); i++)
    {
        if (!del[index[i]])
        {
            for (size_t j = i + 1; j < index.size(); j++)
            {
                if (iou(boxes[index[i]].rec, boxes[index[j]].rec) > nmsThreshold)
                {
                    del[index[j]] = true;
                }
            }
        }
    }

    std::vector<Object> new_obj;
    for (const auto i : index)
    {
        Object obj;
        if (!del[i])
        {
            obj.classID = boxes[i].classID;
            obj.className = boxes[i].className;
            obj.rec.x = boxes[i].rec.x;
            obj.rec.y = boxes[i].rec.y;
            obj.rec.width = boxes[i].rec.width;
            obj.rec.height = boxes[i].rec.height;
            obj.score = boxes[i].score;
        }
        new_obj.push_back(obj);
    }
    boxes = new_obj;
}