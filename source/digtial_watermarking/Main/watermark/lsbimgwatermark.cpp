#include "lsbimgwatermark.h"

#include <iostream>

#include <QImage>
#include <QDebug>

#include "tools.h"

LSBImgWatermark::LSBImgWatermark()
{

}

void LSBImgWatermark::execute(cv::Mat &src, cv::Mat &dst)
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

    // Image merge using LSB method
    // Reference: http://accu.cc/content/pil/watermark

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
