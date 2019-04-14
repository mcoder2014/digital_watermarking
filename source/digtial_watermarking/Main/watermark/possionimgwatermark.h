#ifndef POSSIONIMGWATERMARK_H
#define POSSIONIMGWATERMARK_H

#include "imgwatermark.h"


class PossionImgWatermark : public ImgWatermark
{
public:
    PossionImgWatermark();

    virtual void execute(cv::Mat& src, cv::Mat& dst);
    virtual std::string type(){return std::string("PossionImgWatermark");}
};

#endif // POSSIONIMGWATERMARK_H
