#include "fftimgwatermark.h"

FFTImgWatermark::FFTImgWatermark()
{

}

void FFTImgWatermark::execute(cv::Mat &src, cv::Mat &dst)
{

}

void FFTImgWatermark::checkWatermark(const cv::Mat &src, cv::Mat &dst)
{

}

bool FFTImgWatermark::rotational_symmetry() const
{
    return _rotational_symmetry;
}

void FFTImgWatermark::setRotational_symmetry(bool rotational_symmetry)
{
    _rotational_symmetry = rotational_symmetry;
}
