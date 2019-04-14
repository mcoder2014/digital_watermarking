#ifndef WATERMARKFACTORY_H
#define WATERMARKFACTORY_H

#include <vector>
#include <memory>

#include <QJsonDocument>

#include "watermark.h"
#include "imgwatermark.h"
#include "textwatermark.h"
#include "lsbimgwatermark.h"
#include "possionimgwatermark.h"
#include "fftimgwatermark.h"


class WatermarkFactory
{
public:
    WatermarkFactory();
    std::vector<std::shared_ptr<Watermark>> loadWatermark(const QString& filepath);
private:
    std::shared_ptr<ImgWatermark> getImgWatermark(QJsonObject& json_obj);
    std::shared_ptr<TextWatermark> getTextWatermark(QJsonObject &json_obj);
    std::shared_ptr<LSBImgWatermark> getLSBImgWatermark(QJsonObject &json_obj);
    std::shared_ptr<PossionImgWatermark> getPossionImgWatermark(QJsonObject &json_obj);

};

#endif // WATERMARKFACTORY_H
