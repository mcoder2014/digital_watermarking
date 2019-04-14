#include "watermarkfactory.h"

#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

WatermarkFactory::WatermarkFactory()
{

}

std::vector<std::shared_ptr<Watermark> > WatermarkFactory::loadWatermark(const QString &filepath)
{
    QFile loadFile(filepath);
    std::vector<std::shared_ptr<Watermark>> watermarks;
    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "can't open json file: " << filepath;
        return watermarks;
    }
    QByteArray data = loadFile.readAll();
    loadFile.close();

    QJsonDocument loadDoc(QJsonDocument::fromJson(data));   // 读取json文档
    QJsonArray jsonArray = (loadDoc.object())["water_marking"].toArray();

    for(int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject obj = jsonArray[i].toObject();
        QString type = obj["type"].toString().toLower();
        if(type == QString("img"))
        {
            watermarks.push_back(this->getImgWatermark(obj));
        }
        else if(type == QString("text"))
        {

        }
        else if(type == QString("lsb"))
        {
            watermarks.push_back(this->getLSBImgWatermark(obj));
        }
        else if(type == QString("possion"))
        {
            watermarks.push_back(this->getPossionImgWatermark(obj));
        }
        else
        {
            qWarning() << "Type: " << type << "is not known!";
        }
    }

    return watermarks;
}

///
/// \brief WatermarkFactory::getImgWatermark
/// \param json_obj
/// \return
///
std::shared_ptr<ImgWatermark> WatermarkFactory::getImgWatermark(QJsonObject &json_obj)
{
    std::shared_ptr<ImgWatermark> p = std::make_shared<ImgWatermark>();    // 创建对象
    if(json_obj.contains("content") && json_obj["content"].isString())
    {
        p->setContent(json_obj["content"].toString().toStdString());
    }

    if(json_obj.contains("size"))
    {
        if(json_obj["size"].isArray())
        {
            p->setSize_width(json_obj["size"].toArray()[0].toInt());
            p->setSize_height(json_obj["size"].toArray()[1].toInt());
        }
    }
    if(json_obj.contains("relative"))
    {
        if(json_obj["relative"].isString())
        {
            QString rel = json_obj["relative"].toString().toLower();
            if(rel == QString("none"))
                p->setRelative(Watermark::NONE);
            else if(rel == QString("width"))
                p->setRelative(Watermark::WIDTH);
            else if(rel == QString("height"))
                p->setRelative(Watermark::HEIGHT);
        }
    }
    if(json_obj.contains("relative_size"))
    {
        if(json_obj["relative_size"].isDouble())
            p->setRelative_size(json_obj["relative_size"].toDouble());
    }
    if(json_obj.contains("pos"))
    {
        if(json_obj["pos"].isArray())
        {
            p->setPos(json_obj["pos"].toArray()[0].toDouble(),
                      json_obj["pos"].toArray()[1].toDouble());
        }

    }
    if(json_obj.contains("alpha"))
    {
        p->setAlpha(json_obj["alpha"].toInt());
    }
    if(json_obj.contains("relative_pos"))
    {
        if(json_obj["relative_pos"].isBool())
        {
            p->setRelative_pos(json_obj["relative_pos"].toBool());
        }
    }
    if(json_obj.contains("rotation"))
    {
        p->setRotation(json_obj["rotation"].toInt());
    }

    p->print();
    return p;
}

std::shared_ptr<TextWatermark> WatermarkFactory::getTextWatermark(QJsonObject &json_obj)
{

}

std::shared_ptr<PossionImgWatermark> WatermarkFactory::getPossionImgWatermark(QJsonObject &json_obj)
{
    std::shared_ptr<ImgWatermark> p = this->getImgWatermark(json_obj);
    std::shared_ptr<PossionImgWatermark> possion = std::make_shared<PossionImgWatermark>();

    possion->setPos(p->x(), p->y());
    possion->setAlpha(p->alpha());
    possion->setContent(p->content());
    possion->setRelative(p->relative());
    possion->setRotation(p->rotation());
    possion->setSize_width(p->size_width());
    possion->setSize_height(p->size_height());
    possion->setRelative_pos(p->relative_pos());
    possion->setRelative_size(p->relative_size());

    return possion;
}

std::shared_ptr<LSBImgWatermark> WatermarkFactory::getLSBImgWatermark(QJsonObject &json_obj)
{
    std::shared_ptr<ImgWatermark> p = this->getImgWatermark(json_obj);
    std::shared_ptr<LSBImgWatermark> lsb = std::make_shared<LSBImgWatermark>();
    if(json_obj.contains("bits"))
    {
        lsb->setBits(json_obj["bits"].toInt());
    }

    // Copy info, will change to copy constructor soon.
    lsb->setPos(p->x(), p->y());
    lsb->setAlpha(p->alpha());
    lsb->setContent(p->content());
    lsb->setRelative(p->relative());
    lsb->setRotation(p->rotation());
    lsb->setSize_width(p->size_width());
    lsb->setSize_height(p->size_height());
    lsb->setRelative_pos(p->relative_pos());
    lsb->setRelative_size(p->relative_size());

    return lsb;
}

