#ifndef IMGWATERMARK_H
#define IMGWATERMARK_H
#include "watermark.h"

class ImgWatermark : public Watermark
{
public:
    ImgWatermark();
    void execute(cv::Mat& src, cv::Mat& dst);
    void print();

    void setContent(const std::string &value);
    int size_width() const;
    void setSize_width(int size_width);

    int size_height() const;
    void setSize_height(int size_height);

    cv::Mat watermark() const;
    void setWatermark(const cv::Mat &watermark);

    int alpha() const;
    void setAlpha(int alpha);

private:
    int _size_width;        // 如果不使用相对尺寸，则使用绝对尺寸
    int _size_height;       // 如果不使用相对尺寸，则使用相对尺寸
    int _alpha;             // 设置水印的半透明

    cv::Mat _watermark;     // 加载到的原本的水印图片
};

#endif // IMGWATERMARK_H
