#ifndef TOOLS_H
#define TOOLS_H

#include <QImage>
#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

QImage cvMat2QImage(const cv::Mat& mat);    // 转换格式
cv::Mat QImage2cvMat(QImage &image);         // 转换格式

void DEBUG_SAVE_MAT(const cv::Mat& mat, std::string savepath);


#endif // TOOLS_H
