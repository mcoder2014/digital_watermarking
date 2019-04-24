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

PossionImgWatermark::PossionImgWatermark(const PossionImgWatermark *possion)
    :ImgWatermark (possion)
{

}

PossionImgWatermark::PossionImgWatermark(const ImgWatermark *imgwatermark)
    :ImgWatermark (imgwatermark)
{

}

void PossionImgWatermark::execute(cv::Mat &src, cv::Mat &dst)
{

    // 1. Adjust the watermark
    cv::Mat tmp;            // the watermark after transform
    // 2. Calculate Position
    int dst_x=0, dst_y=0;   // the postion (left-upper pointer of watermark) in destination image.
    this->_transformWatermark(src, tmp, dst_x, dst_y);
    // 3. Merge Watermark
    src.convertTo(dst, CV_8UC4);    // Change dst mat type

    std::vector<cv::Mat> tmp_channels;
    std::vector<cv::Mat> dst_channels;

    // Get the BGR channels of watermark and dst image
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

std::string PossionImgWatermark::toString()
{
    std::string output = ImgWatermark::toString();
    return output;
}
