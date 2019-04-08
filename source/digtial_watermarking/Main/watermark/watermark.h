#ifndef WATERMARK_H
#define WATERMARK_H

#include <string>

#include <opencv2/core.hpp>

class Watermark
{
public:
    enum Anchor{LEFT_UPPER,LEFT_MIDDLE,LEFT_BOTTOM,
               CENTER_UPPER,CENTER_MIDDLE,CENTER_BOTTOM,
               RIGHT_UPPER,RIGHT_MIDDLE,RIGHT_BOTTOM};      // 相对图片的锚点
    enum Relative{NONE,WIDTH,HEIGHT};                       // 相对尺寸的

    Watermark();
    ~Watermark();
    virtual void execute(cv::Mat& src, cv::Mat& dst) = 0;   // 执行添加水印方法-对目标图像加水印
    virtual void print() = 0;                                   // 输出参数信息
    void setPos(float x, float y){this->_x = x; this->_y = y;}

    float x() const;
    float y() const;

    int getRotation() const;
    void setRotation(int value);

    std::string getContent() const;
    virtual void setContent(const std::string &value);

    Relative getRelative() const;
    void setRelative(const Relative &relative);

    float getRelative_size() const;
    void setRelative_size(float relative_size);

    bool getRelative_pos() const;
    void setRelative_pos(bool relative_pos);

protected:
    float _x, _y;            // 坐标
    bool _relative_pos;      // 是否是相对坐标
    int _rotation;           // 旋转值
    std::string _content;    // 内容
    Relative _relative;      // 是否相对尺寸
    float _relative_size;    // 相对尺寸

};

#endif // WATERMARK_H
