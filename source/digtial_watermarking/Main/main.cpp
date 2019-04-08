#include "mainwindow.h"
#include <QApplication>

#include <vector>
#include <memory>

#include "watermark/imgwatermark.h"
#include "watermark/watermarkfactory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /**-----------------------**/
    WatermarkFactory factory;
    auto watermarks = factory.loadWatermark("watermark.json");


    return a.exec();
}


