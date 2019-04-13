#-------------------------------------------------
#
# Project created by QtCreator 2019-04-07T21:31:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Main
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../bin       # 生成的文件所在的目录
MOC_DIR = ./moc        # Q_OBJECT转换后的类
RCC_DIR = ./rcc        # .qrc文件转换后的位置
OBJECTS_DIR = ./tmp    # 编译中间文件的位置
UI_DIR = ./ui          # ui_xxx 文件存放位置

CONFIG += c++11

# include
INCLUDEPATH += $$PWD \
    E:/libs/opencv-3.4.5/mingw730_release/install/include

# libs

CONFIG(debug,debug|release){
LIBS+= \
    -LE:/libs/opencv-3.4.5/mingw730_debug/install/x64/mingw/lib -lopencv_core345d -lopencv_imgproc345d

}
CONFIG(release,debug|release){
LIBS+= \
    -LE:/libs/opencv-3.4.5/mingw730_release/install/x64/mingw/lib -lopencv_world345
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    watermark/watermark.cpp \
    watermark/imgwatermark.cpp \
    watermark/textwatermark.cpp \
    watermark/watermarkfactory.cpp \
    watermark/tools.cpp \
    watermark/lsbimgwatermark.cpp \
    watermark/unvisiblewatermark.cpp

HEADERS += \
        mainwindow.h \
    watermark/watermark.h \
    watermark/imgwatermark.h \
    watermark/textwatermark.h \
    watermark/watermarkfactory.h \
    watermark/tools.h \
    watermark/lsbimgwatermark.h \
    watermark/unvisiblewatermark.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
