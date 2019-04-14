#include "possionimgwatermark.h"

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

#include <QImage>
#include <QDebug>

#include "tools.h"

PossionImgWatermark::PossionImgWatermark():ImgWatermark ()
{

}

void PossionImgWatermark::execute(cv::Mat &src, cv::Mat &dst)
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

    std::vector<cv::Mat> tmp_channels;
    std::vector<cv::Mat> dst_channels;

    // Get the BGR channels of watermark and dst image
//    cv::Mat tmp_clone1 = tmp.clone();
//    cv::Mat dst_clone1 = dst.clone();

    cv::split(tmp,tmp_channels);
    cv::split(dst, dst_channels);

    std::vector<cv::Mat> tmp_bgr_channels;
    std::vector<cv::Mat> dst_bgr_channels;

    tmp_bgr_channels.push_back(tmp_channels[0]);
    tmp_bgr_channels.push_back(tmp_channels[1]);
    tmp_bgr_channels.push_back(tmp_channels[2]);

    dst_bgr_channels.push_back(dst_channels[0]);
    dst_bgr_channels.push_back(dst_channels[1]);
    dst_bgr_channels.push_back(dst_channels[2]);

    cv::Mat tmp_bgr;
    cv::Mat dst_bgr;
    cv::merge(tmp_bgr_channels, tmp_bgr);
    cv::merge(dst_bgr_channels, dst_bgr);

    // Seamless clone
    // cv::Mat Roi = dst(cv::Rect(dst_x,dst_y,tmp.size().width,tmp.size().height));
    cv::Point2i center(dst_x + tmp.size().width/2, dst_y+tmp.size().height/2);
    cv::seamlessClone(tmp_bgr, dst_bgr, tmp_channels[3], center, dst_bgr, cv::NORMAL_CLONE);

    // merge dst channel with its alpha channel
    dst_bgr_channels.clear();
    cv::split(dst_bgr, dst_bgr_channels);
    dst_bgr_channels.push_back(dst_channels[3]);
    cv::merge(dst_bgr_channels, dst);

}
