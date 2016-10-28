/**
 * A library for Qt app
 *
 * LICENSE: The GNU Lesser General Public License, version 3.0
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0  The GNU Lesser General Public License, version 3.0
 * @link        https://github.com/akiraohgaki/qt-libs
 */

#include "json.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

namespace utils {

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

} // namespace utils
