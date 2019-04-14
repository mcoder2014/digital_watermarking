#include "fftimgwatermark.h"

#include <iostream>
#include <vector>

#include <QImage>
#include <QDebug>

#include "tools.h"


FFTImgWatermark::FFTImgWatermark():ImgWatermark ()
{
    this->_rotational_symmetry = true;
}

void FFTImgWatermark::execute(cv::Mat &src, cv::Mat &dst)
{
    if(this->_is_update)
    {
        // update Mat
        _updateWatermarkMat();
    }
    cv::Mat tmp = this->_modifiedWatermark.clone();                   // deep copy

    float factor = 1.0;
    if(this->_relative==Watermark::WIDTH)
    {
        factor = 1.0 * this->_relative_size * src.cols / tmp.cols;    // Get the factor of watermark

    }
    else if(this->_relative==Watermark::HEIGHT)
    {
        factor = 1.0 * this->_relative_size * src.rows / tmp.rows;
    }

    // Rotation :-> Caculate roate mat 2*3
    // Rotation and resize
    if(this->_rotation == 0)
    {
        tmp.resize((int)(factor * tmp.rows), cv::Scalar((int)(factor * tmp.cols))); // Resize watermark
    }
    else
    {
        // Reference https://blog.csdn.net/xiaowei_cqu/article/details/7616044
        int width = tmp.size().width;
        int height = tmp.size().height;
        double angle = this->_rotation * CV_PI /180;
        cv::Mat rotationMat=cv::getRotationMatrix2D(cv::Point2f (width/2.0, height/2.0), this->_rotation, factor);

        double sina = sin(angle), cosa = cos(angle);
        int width_rotation  = (int)((height*fabs(sina) + width*fabs(cosa))*factor);
        int height_rotation = (int)((width*fabs(sina) + height*fabs(cosa))*factor);

        // Check the offset of rotation
        rotationMat.at<double>(2) += (width_rotation - width)*factor/2;
        rotationMat.at<double>(5) += (height_rotation - height)*factor/2;

        cv::warpAffine(tmp,tmp,rotationMat,cv::Size(width_rotation, height_rotation));

//        DEBUG_SAVE_MAT(tmp, "debug/test_rotation.png");
    }


    // 2. Calculate Position
    int dst_x=0, dst_y=0;   // the postion (left-upper pointer of watermark) in destination image.

    if(this->_relative_pos)
    {
        // relative pos
        dst_x = this->_x /100.0 * src.size().width;
        dst_y = this->_y/100.0 * src.size().height;
    }
    else
    {
        // absolute pos
        dst_x = this->_x;
        dst_y = this->_y;
    }

    // 3. Merge Watermark
    src.convertTo(dst, CV_8UC4);    // Change dst mat type

//    cv::Mat Roi = dst(cv::Rect(dst_x,dst_y,tmp.size().width,tmp.size().height));
    int recommend_height = cv::getOptimalDFTSize(src.rows);
    int recommend_width = cv::getOptimalDFTSize(src.cols);

    // add zeros to larger src image for good performance
    cv::Mat src_larger;
    cv::copyMakeBorder(src, src_larger, 0, recommend_height-src.rows, 0, recommend_width-src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // split to BGRA and only do fft in BGR channels
    std::vector<cv::Mat> src_larger_channels;
    cv::split(src_larger, src_larger_channels);

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
