#ifndef POSSIONIMGWATERMARK_H
#define POSSIONIMGWATERMARK_H

#include "imgwatermark.h"


class PossionImgWatermark : public ImgWatermark
{
public:
    PossionImgWatermark();
    PossionImgWatermark(const PossionImgWatermark* possion);
    PossionImgWatermark(const ImgWatermark* imgwatermark);

    virtual void execute(cv::Mat& src, cv::Mat& dst);
    virtual std::string type(){return std::string("PossionImgWatermark");}
    virtual std::string toString();
};

#endif // POSSIONIMGWATERMARK_H
