#ifndef WATERMARK_H
#define WATERMARK_H

#include <string>

#include <opencv2/core.hpp>

class Watermark
{
public:
    enum Relative{NONE,WIDTH,HEIGHT};                       // 相对尺寸的

    Watermark();
    Watermark(const Watermark* watermark);

    ~Watermark();
    virtual void execute(cv::Mat& src, cv::Mat& dst) = 0;   // 执行添加水印方法-对目标图像加水印
    virtual void print() = 0;                                   // 输出参数信息
    virtual std::string type(){return std::string("Watermark");}
    void setPos(float x, float y){this->_x = x; this->_y = y; this->_is_update = true;}

    float x() const;
    float y() const;

    int rotation() const;
    void setRotation(int value);

    std::string content() const;
    virtual void setContent(const std::string &value);

    Relative relative() const;
    void setRelative(const Relative &relative);

    float relative_size() const;
    void setRelative_size(float relative_size);

    bool relative_pos() const;
    void setRelative_pos(bool relative_pos);

    virtual std::string toString();

protected:
    float _x, _y;            // 坐标
    bool _relative_pos;      // 是否是相对坐标
    int _rotation;           // 旋转值
    std::string _content;    // 内容
    Relative _relative;      // 是否相对尺寸
    float _relative_size;    // 相对尺寸

    bool _is_update;         // 是否更新了设置

};

#endif // WATERMARK_H
