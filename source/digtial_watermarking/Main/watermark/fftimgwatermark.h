#ifndef FFTIMGWATERMARK_H
#define FFTIMGWATERMARK_H

#include "imgwatermark.h"
#include "unvisiblewatermark.h"

class FFTImgWatermark : public ImgWatermark, public UnvisibleWatermark
{

public:
    FFTImgWatermark();
    virtual void execute(cv::Mat& src, cv::Mat& dst);
    virtual std::string type(){return std::string("LSBImgWatermark");}
    virtual void checkWatermark(const cv::Mat& src, cv::Mat& dst);

    bool rotational_symmetry() const;
    void setRotational_symmetry(bool rotational_symmetry);

protected:
    bool _rotational_symmetry;     // 图像的水印是否中心对称

};

#endif // FFTIMGWATERMARK_H
