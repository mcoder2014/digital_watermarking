#include "imgwatermark.h"

#include <iostream>

#include <QImage>

#include "tools.h"

ImgWatermark::ImgWatermark()
{

}

///
/// \brief ImgWatermark::execute
/// \param src
/// \param dst
///
void ImgWatermark::execute(cv::Mat &src, cv::Mat &dst)
{
    // 1. 缩放水印
    // 2. 定位
    // 3. 合并

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
    this->_content = value;

    // 加载图片到 mat
    QImage qimage(QString::fromStdString(value));
    this->_watermark = QImage2cvMat(qimage);

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
