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

    virtual void execute(cv::Mat &src, cv::Mat& dst);
    virtual std::string type(){return std::string("FFTImgWatermark");}
    virtual void checkWatermark(const cv::Mat& src, cv::Mat& dst);

    bool rotational_symmetry() const;
    void setRotational_symmetry(bool rotational_symmetry);
    std::string toString();

    uint64_t key() const;
    void setKey(const uint64_t &key);

protected:

    // whether the watermark in fft space is rotational symmetry
    bool _rotational_symmetry;
    uint64_t _key;

    void fftSingleChannel(const cv::Mat& src, cv::Mat& dst);
    void ifftSingleChannel(const cv::Mat& src, cv::Mat& dst);

    void mergeWatermark(const cv::Mat& watermark, std::vector<cv::Mat> dsts);

    void watermarkEncode(const cv::Mat& watermark, cv::Mat &dst, cv::Mat &src,int dst_x=0, int dst_y=0, uint64_t key=0);
    void watermarkDecode(const cv::Mat& src,cv::Mat& dst, uint64_t key=0);
};

#endif // FFTIMGWATERMARK_H
