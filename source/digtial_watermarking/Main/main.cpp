#include "mainwindow.h"
#include <QApplication>

#include <vector>
#include <memory>

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
    watermarks[0]->execute(mat,imgW);
    watermarks[1]->execute(mat,lsbW);

    DEBUG_SAVE_MAT(imgW, "debug/imgWatermark.png");
    DEBUG_SAVE_MAT(lsbW, "debug/lsbWatermark.png");

    cv::Mat lsbCheck;
    static_cast<LSBImgWatermark*>(watermarks[1].get())->checkWatermark(lsbW, lsbCheck);
    DEBUG_SAVE_MAT(lsbCheck, "debug/lsbCheck.png");

    return a.exec();
}


