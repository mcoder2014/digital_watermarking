#ifndef UNVISIBLEWATERMARK_H
#define UNVISIBLEWATERMARK_H

#include <opencv2/core/core.hpp>

class UnvisibleWatermark
{
public:
    UnvisibleWatermark();
    virtual void checkWatermark(const cv::Mat& src, cv::Mat& dst)=0;

    void setSourceImg(const cv::Mat& src){src.copyTo(_src);}
    bool needSourceImg(){return _needSourceImg;}

protected:
    cv::Mat _src;
    bool _needSourceImg;        // If recover the watermark needed the source image
};

#endif // UNVISIBLEWATERMARK_H
