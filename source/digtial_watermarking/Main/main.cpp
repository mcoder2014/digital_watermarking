#include "mainwindow.h"
#include <QApplication>

#include <vector>
#include <memory>

#include <QDebug>
#include <QString>

#include "watermark/imgwatermark.h"
#include "watermark/watermarkfactory.h"

#include "watermark/tools.h"

#include "watermark/watermarkencoder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /**-----------------------**/
    WatermarkFactory factory;
    auto watermarks = factory.loadWatermark("watermark.json");
    QImage src("testimg.jpg");
    cv::Mat mat = QImage2cvMat(src);

    cv::Mat imgW;
    cv::Mat lsbW;
    cv::Mat possionW;
    cv::Mat fftW;

//    qDebug() << QString::fromStdString(watermarks[0]->toString());
//    watermarks[0]->execute(mat,imgW);
//    DEBUG_SAVE_MAT(imgW, "debug/imgWatermark.png");

//    qDebug() << QString::fromStdString(watermarks[1]->toString());
//    watermarks[1]->execute(mat,lsbW);
//    DEBUG_SAVE_MAT(lsbW, "debug/lsbWatermark.png");

//    qDebug() << QString::fromStdString(watermarks[2]->toString());
//    watermarks[2]->execute(mat, possionW);
//    DEBUG_SAVE_MAT(possionW, "debug/possionImgWatermark.png");

    // Test image encoder
    QImage wa("watermarks/starbucks.png");
    cv::Mat watermark = QImage2cvMat(wa);
    cv::Mat painter = cv::Mat::zeros(2000,3000,CV_8UC4);

    int dst_x=500,dst_y=200;
    cv::Mat tmp = watermark.clone();

    cv::Mat Roi = painter(cv::Rect(dst_x,dst_y,tmp.size().width,tmp.size().height));
    float alpha = 1.0;       // 0-100

    // Reference: http://accu.cc/content/pil/watermark
    for(int i=0; i< tmp.rows; i++)
    {
        for(int j=0; j<tmp.cols; j++)
        {
            cv::Vec4b dst_pixel = Roi.at<cv::Vec4b>(i,j);
            cv::Vec4b wm_pixel = tmp.at<cv::Vec4b>(i,j);
            float real_alpha = alpha * (wm_pixel[3]/255);
            dst_pixel = (1-real_alpha)*dst_pixel + real_alpha * wm_pixel;
            Roi.at<cv::Vec4b>(i,j) = dst_pixel;
        }
    }
    DEBUG_SAVE_MAT(tmp, "debug/Without_encode.png");

    WatermarkEncoder encoder;
    cv::Mat water_mark_after_encode;
    cv::Mat water_mark_after_decode;

    encoder.encode(painter,water_mark_after_encode, 10);
    encoder.decode(water_mark_after_encode, water_mark_after_decode, 10);

    DEBUG_SAVE_MAT(water_mark_after_encode, "debug/After_encode.png");
    DEBUG_SAVE_MAT(water_mark_after_decode, "debug/after_decode.png");


    qDebug() << QString::fromStdString(watermarks[3]->toString());
//    watermarks[3]->execute(mat, fftW);
//    DEBUG_SAVE_MAT(fftW, "debug/fftImgWatermark.png");


    // Check
//    cv::Mat lsbCheck;
//    static_cast<LSBImgWatermark*>(watermarks[1].get())->checkWatermark(lsbW, lsbCheck);
//    DEBUG_SAVE_MAT(lsbCheck, "debug/lsbCheck.png");

    cv::Mat fftCheck;
//    static_cast<FFTImgWatermark*>(watermarks[3].get())->checkWatermark(fftW, fftCheck);

    /*--------------------------*/
    return a.exec();
}


