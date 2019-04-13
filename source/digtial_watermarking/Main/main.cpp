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

    watermarks[0]->execute(mat,mat);

    DEBUG_SAVE_MAT(mat, "debug/AfterWatermark.png");

    return a.exec();
}


