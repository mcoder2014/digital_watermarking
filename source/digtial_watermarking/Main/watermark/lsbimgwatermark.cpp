#include "lsbimgwatermark.h"

#include <iostream>

#include <QImage>
#include <QDebug>

#include "tools.h"

LSBImgWatermark::LSBImgWatermark():ImgWatermark ()
{
    this->_bits = 2;
}

LSBImgWatermark::LSBImgWatermark(const ImgWatermark *imgwatermark)
    :ImgWatermark (imgwatermark)
{

}

LSBImgWatermark::LSBImgWatermark(const LSBImgWatermark *lsbimgwatermark)
    :ImgWatermark (lsbimgwatermark),_bits(lsbimgwatermark->_bits)
{

}

void LSBImgWatermark::execute(cv::Mat &src, cv::Mat &dst)
{

    // 1. Adjust the watermark
    cv::Mat tmp;            // the watermark after transform
    // 2. Calculate Position
    int dst_x=0, dst_y=0;   // the postion (left-upper pointer of watermark) in destination image.
    this->_transformWatermark(src, tmp, dst_x, dst_y);

    // 3. Merge Watermark
    src.convertTo(dst, CV_8UC4);    // Change dst mat type

    // Image merge using LSB method
    // Reference: http://accu.cc/content/pil/watermark

//    dst = src.clone();
    cv::Mat Roi = dst(cv::Rect(dst_x,dst_y,tmp.size().width,tmp.size().height));
    float alpha = this->_alpha/100.0;       // 0-100
    int bits = this->_bits;
    int anti_bits = 8-bits;

    for(int i=0; i< tmp.rows; i++)
    {
        for(int j=0; j<tmp.cols; j++)
        {
            cv::Vec4b dst_pixel = Roi.at<cv::Vec4b>(i,j);
            cv::Vec4b wm_pixel = tmp.at<cv::Vec4b>(i,j);

            dst_pixel[0] = dst_pixel[0] >> bits << bits;
            dst_pixel[1] = dst_pixel[1] >> bits << bits;
            dst_pixel[2] = dst_pixel[2] >> bits << bits;

            wm_pixel[0] = wm_pixel[0] >> anti_bits;
            wm_pixel[1] = wm_pixel[1] >> anti_bits;
            wm_pixel[2] = wm_pixel[2] >> anti_bits;

            float real_alpha = alpha * (wm_pixel[3]/255);
            unsigned char alpha_channel = dst_pixel[3] * (1-real_alpha) + real_alpha * wm_pixel[3];

            dst_pixel = dst_pixel + real_alpha * wm_pixel;
            dst_pixel[3] = alpha_channel;

            Roi.at<cv::Vec4b>(i,j) = dst_pixel;
        }
    }

}

void LSBImgWatermark::checkWatermark(const cv::Mat &src, cv::Mat &dst)
{
    dst = src.clone();
    int anti_bits = 8 -_bits;
    for(int i=0; i< dst.rows; i++)
    {
        for(int j=0; j<dst.cols; j++)
        {
            cv::Vec4b pixel = dst.at<cv::Vec4b>(i,j);

            pixel[0] = pixel[0] << anti_bits;
            pixel[1] = pixel[1] << anti_bits;
            pixel[2] = pixel[2] << anti_bits;

            dst.at<cv::Vec4b>(i,j) = pixel;
        }
    }
}

std::string LSBImgWatermark::toString()
{
    std::string output = ImgWatermark::toString();
    output = output
            + "\tbits: " + std::to_string(this->_bits);
    return output;
}

int LSBImgWatermark::bits() const
{
    return _bits;
}

void LSBImgWatermark::setBits(int bits)
{

    if(bits ==0)
        _bits=0;
    else
    {
        _bits = bits%8;
        if(_bits == 0)
            _bits = 8;
    }

}
