#include "watermark.h"

Watermark::Watermark():_is_update(false)
{
    this->_x=0;
    this->_y=0;
    this->_content="";
    this->_relative = NONE;
    this->_rotation = 0;
    this->_relative_pos = false;
    this->_relative_size = 0;
}

Watermark::Watermark(const Watermark *watermark)
    :_x(watermark->_x), _y(watermark->_y),
      _relative(watermark->_relative),_relative_pos(watermark->_relative_pos),
      _relative_size(watermark->_relative_size)
{
    this->setContent(watermark->content());
}

Watermark::~Watermark()
{

}

float Watermark::x() const
{
    return _x;
}

float Watermark::y() const
{
    return _y;
}

int Watermark::rotation() const
{
    return _rotation;
}

void Watermark::setRotation(int value)
{
    _rotation = value;
    this->_is_update = true;
}

std::string Watermark::content() const
{
    return _content;
}

void Watermark::setContent(const std::string &value)
{
    _content = value;
    this->_is_update = true;
}

Watermark::Relative Watermark::relative() const
{
    return _relative;
}

void Watermark::setRelative(const Relative &relative)
{
    _relative = relative;
    this->_is_update = true;
}

float Watermark::relative_size() const
{
    return _relative_size;
}

void Watermark::setRelative_size(float relative_size)
{
    _relative_size = relative_size;
    this->_is_update = true;
}

bool Watermark::relative_pos() const
{
    return _relative_pos;
}

void Watermark::setRelative_pos(bool relative_pos)
{
    _relative_pos = relative_pos;
    this->_is_update = true;
}

std::string Watermark::toString()
{
    std::string output;
    output = output + "content: " + this->_content
            + "\trelative: " + std::to_string(this->_relative)
            + "\trelative_size: " + std::to_string(this->_relative_size)
            + "\tpos: " + std::to_string(this->_x) + " " + std::to_string(this->_y)
            + "\trelative_pos: " + std::to_string(this->_relative_pos)
            + "\trotation: " + std::to_string(this->_rotation);
    return output;
}
