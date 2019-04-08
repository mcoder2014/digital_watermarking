#ifndef TEXTWATERMARK_H
#define TEXTWATERMARK_H

#include "watermark.h"

class TextWatermark : public Watermark
{
public:
    TextWatermark();
    void execute(cv::Mat& src, cv::Mat& dst);
    void print();
};

#endif // TEXTWATERMARK_H
