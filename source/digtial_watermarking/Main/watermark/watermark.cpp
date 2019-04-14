#include "watermark.h"

Watermark::Watermark():_is_update(false)
{

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
