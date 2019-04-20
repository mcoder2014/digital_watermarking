#ifndef FFTIMGWATERMARK_H
#define FFTIMGWATERMARK_H

#include "imgwatermark.h"
#include "unvisiblewatermark.h"

class FFTImgWatermark : public ImgWatermark, public UnvisibleWatermark
{

public:
    FFTImgWatermark();
    FFTImgWatermark(const ImgWatermark* imgwatermark);
    FFTImgWatermark(const FFTImgWatermark* fftimgwatermark);

    virtual void execute(cv::Mat& src, cv::Mat& dst);
    virtual std::string type(){return std::string("LSBImgWatermark");}
    virtual void checkWatermark(const cv::Mat& src, cv::Mat& dst);

    bool rotational_symmetry() const;
    void setRotational_symmetry(bool rotational_symmetry);
    std::string toString();

protected:

    // whether the watermark in fft space is rotational symmetry
    bool _rotational_symmetry;

    void fftSingleChannel(const cv::Mat& src, cv::Mat& dst);
    void ifftSingleChannel(const cv::Mat& src, cv::Mat& dst);
    void singleChannelWatermark(
            const cv::Mat& watermark, std::vector<cv::Mat>& dst,
            int dst_x, int dst_y);
};

#endif // FFTIMGWATERMARK_H
