#include "imgwatermark.h"

#include <iostream>

#include <QImage>
#include <QDebug>

#include "tools.h"

ImgWatermark::ImgWatermark():Watermark ()
{
    this->_alpha = 100;
    this->_size_width = 100;
    this->_size_height = 100;
}

///
/// \brief ImgWatermark::execute
/// \param src
/// \param dst
///
void ImgWatermark::execute(cv::Mat &src, cv::Mat &dst)
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

    // Image merge of Photoshop
    // Reference: https://blog.csdn.net/chy555chy/article/details/54016317
    cv::Mat Roi = dst(cv::Rect(dst_x,dst_y,tmp.size().width,tmp.size().height));
    float alpha = this->_alpha/100.0;       // 0-100

    // Reference: http://accu.cc/content/pil/watermark
    for(int i=0; i< tmp.rows; i++)
    {
        for(int j=0; j<tmp.cols; j++)
        {
            cv::Vec4b dst_pixel = Roi.at<cv::Vec4b>(i,j);
            cv::Vec4b wm_pixel = tmp.at<cv::Vec4b>(i,j);
            float real_alpha = alpha * (wm_pixel[3]/255);
            dst_pixel = (1-real_alpha)*dst_pixel + real_alpha * wm_pixel;
            Roi.at<cv::Vec4b>(i,j) = dst_pixel;
        }
    }

}

void ImgWatermark::print()
{
    std::cout << "content: " << this->_content
              << "\tsize: " << this->_size_width << " " << this->_size_height
              << "\trelative: " << this->_relative
              << "\trelative_size: " << this->_relative_size
              << "\tpos: " << this->_x << " " << this->_y
              << "\talpha: " << this->_alpha
              << "\trelative_pos: " << this->_relative_pos
              << "\trotation: " << this->_rotation
              << std::endl;
}

void ImgWatermark::setContent(const std::string &value)
{
//    this->_content = value;
    Watermark::setContent(value);       // 调用 parent 的方法

    // 加载图片到 mat
    QImage qimage(QString::fromStdString(this->_content));
    this->_watermark = QImage2cvMat(qimage).clone();            // deep copy
//    DEBUG_SAVE_MAT(_watermark, "debug/watermark.png");

}

int ImgWatermark::size_width() const
{
    return _size_width;
}

void ImgWatermark::setSize_width(int size_width)
{
    _size_width = size_width;
}

int ImgWatermark::size_height() const
{
    return _size_height;
}

void ImgWatermark::setSize_height(int size_height)
{
    _size_height = size_height;
}

cv::Mat ImgWatermark::watermark() const
{
    return _watermark;
}

void ImgWatermark::setWatermark(const cv::Mat &watermark)
{
    _watermark = watermark;
}

int ImgWatermark::alpha() const
{
    return _alpha;
}

void ImgWatermark::setAlpha(int alpha)
{
    _alpha = alpha;
}

///
/// \brief ImgWatermark::_updateWatermarkMat
/// If image is absolute size, resize
/// and change the image's format to CV_8UC4
///
void ImgWatermark::_updateWatermarkMat()
{
//    qDebug() << "update_watermark:"<<QString::fromStdString(this->_content);

    this->_modifiedWatermark = this->_watermark.clone();                // deep copy
//    DEBUG_SAVE_MAT(_modifiedWatermark, "debug/copyWatermark.png");

    if(this->_relative == Watermark::NONE)
    {
        // Only when the size of watermark is absolute size, we will change its size here.
        // First resize

        cv::resize(_modifiedWatermark, _modifiedWatermark, cv::Size((int)this->_size_width, (int)this->_size_height));
//        qDebug() << "Resize: " << (size_t)this->_size_width << "\t"<<(size_t)this->_size_height;
    }

    // Change the type of image to 4channel
    switch (this->_modifiedWatermark.type())
    {
    case CV_8UC1:
        cv::cvtColor(this->_modifiedWatermark,this->_modifiedWatermark,cv::COLOR_GRAY2BGRA);
        break;
    case CV_8UC3:
        cv::cvtColor(this->_modifiedWatermark, this->_modifiedWatermark,cv::COLOR_BGR2BGRA);
        break;
    case CV_8UC4:
        break;
    default:
        this->_modifiedWatermark.convertTo(this->_modifiedWatermark, CV_8UC4);
    }

//    DEBUG_SAVE_MAT(_modifiedWatermark, "debug/modifiedWatermark.png");
}
