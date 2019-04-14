#ifndef WATERMARKFACTORY_H
#define WATERMARKFACTORY_H

#include <vector>
#include <memory>

#include <QJsonDocument>

#include "watermark.h"
#include "imgwatermark.h"
#include "textwatermark.h"
#include "lsbimgwatermark.h"


class WatermarkFactory
{
public:
    WatermarkFactory();
    std::vector<std::shared_ptr<Watermark>> loadWatermark(const QString& filepath);
private:
    std::shared_ptr<ImgWatermark> getImgWatermark(QJsonObject& json_obj);
    std::shared_ptr<TextWatermark> getTextWatermark(QJsonObject &json_obj);
    std::shared_ptr<LSBImgWatermark> getLSBImageWatermark(QJsonObject &json_obj);

};

#endif // WATERMARKFACTORY_H
