#ifndef UNVISIBLEWATERMARK_H
#define UNVISIBLEWATERMARK_H

#include <opencv2/core/core.hpp>

class UnvisibleWatermark
{
public:
    UnvisibleWatermark();
    virtual void checkWatermark(const cv::Mat& src, cv::Mat& dst)=0;
};

#endif // UNVISIBLEWATERMARK_H
