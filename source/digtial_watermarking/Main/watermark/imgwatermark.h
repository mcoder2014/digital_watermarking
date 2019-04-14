#ifndef IMGWATERMARK_H
#define IMGWATERMARK_H
#include "watermark.h"

class ImgWatermark : public Watermark
{
public:
    ImgWatermark();
    virtual void execute(cv::Mat& src, cv::Mat& dst);
    virtual std::string type(){return std::string("ImgWatermark");}
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

protected:
    int _size_width;        // Size width, if(relative == true) this is relative width, otherwise absolute width
    int _size_height;       // Just like size_width
    int _alpha;             // To set the transparent of watermark ROI

    cv::Mat _watermark;     // The source image of watermark content.
    cv::Mat _modifiedWatermark; // the watermark mat modified based on the description.

    void _updateWatermarkMat();
};

#endif // IMGWATERMARK_H
