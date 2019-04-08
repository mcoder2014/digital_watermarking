#ifndef TOOLS_H
#define TOOLS_H

#include <QImage>
#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

QImage cvMat2QImage(const cv::Mat& mat);    // 转换格式
cv::Mat QImage2cvMat(QImage &image);         // 转换格式


#endif // TOOLS_H
