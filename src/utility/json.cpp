#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "json.h"

namespace utility {

Json::Json(QObject *parent) : QObject(parent)
{}

QString Json::convertObjToStr(const QJsonObject &jsonObj)
{
    QJsonDocument jsonDoc(jsonObj);
    return QString::fromUtf8(jsonDoc.toJson());
}

QJsonObject Json::convertStrToObj(const QString &json)
{
    QJsonObject jsonObj;
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8(), &jsonError);
    if (jsonError.error == QJsonParseError::NoError && jsonDoc.isObject()) {
        jsonObj = jsonDoc.object();
    }
    return jsonObj;
}

bool Json::isValid(const QString &json)
{
    QJsonParseError jsonError;
    QJsonDocument::fromJson(json.toUtf8(), &jsonError);
    if (jsonError.error == QJsonParseError::NoError) {
        return true;
    }
    return false;
}

} // namespace utility
