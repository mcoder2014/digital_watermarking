#include "mainwindow.h"
#include <QApplication>

#include <vector>
#include <memory>

#include <QDebug>
#include <QString>

#include "watermark/imgwatermark.h"
#include "watermark/watermarkfactory.h"

#include "watermark/tools.h"

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

    qDebug() << QString::fromStdString(watermarks[3]->toString());
    watermarks[3]->execute(mat, fftW);
    DEBUG_SAVE_MAT(fftW, "debug/fftImgWatermark.png");


    // Check
//    cv::Mat lsbCheck;
//    static_cast<LSBImgWatermark*>(watermarks[1].get())->checkWatermark(lsbW, lsbCheck);
//    DEBUG_SAVE_MAT(lsbCheck, "debug/lsbCheck.png");

    cv::Mat fftCheck;
    static_cast<FFTImgWatermark*>(watermarks[3].get())->checkWatermark(fftW, fftCheck);

    /*--------------------------*/
    return a.exec();
}


