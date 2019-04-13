#ifndef LSBIMGWATERMARK_H
#define LSBIMGWATERMARK_H

#include "imgwatermark.h"
#include "unvisiblewatermark.h"

class LSBImgWatermark : public ImgWatermark, public UnvisibleWatermark
{
public:
    LSBImgWatermark();
    virtual void execute(cv::Mat& src, cv::Mat& dst);
    virtual std::string type(){return std::string("LSBImgWatermark");}
    virtual void checkWatermark(const cv::Mat& src, cv::Mat& dst);

    int bits() const;
    void setBits(int bits);

protected:
    int _bits;
};

#endif // LSBIMGWATERMARK_H
