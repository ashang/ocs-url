/**
 * A library for Qt app
 *
 * LICENSE: The GNU Lesser General Public License, version 3.0
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0  The GNU Lesser General Public License, version 3.0
 * @link        https://github.com/akiraohgaki/qtlibs
 */

#include "json.h"

#include <QJsonDocument>
#include <QJsonParseError>

namespace qtlibs {

Json::Json(const QByteArray &json, QObject *parent)
    : QObject(parent), json_(json)
{}

Json::Json(const QJsonObject &object, QObject *parent)
    : QObject(parent)
{
    fromObject(object);
}

Json::Json(const QJsonArray &array, QObject *parent)
    : QObject(parent)
{
    fromArray(array);
}

Json::Json(const Json &other)
{
    this->setParent(other.parent());
    setJson(other.json());
}

Json &Json::operator =(const Json &other)
{
    this->setParent(other.parent());
    setJson(other.json());
    return *this;
}

QByteArray Json::json() const
{
    return json_;
}

void Json::setJson(const QByteArray &json)
{
    json_ = json;
}

void Json::fromObject(const QJsonObject &object)
{
    QJsonDocument doc(object);
    setJson(doc.toJson());
}

void Json::fromArray(const QJsonArray &array)
{
    QJsonDocument doc(array);
    setJson(doc.toJson());
}

QByteArray Json::toJson()
{
    QJsonDocument doc = QJsonDocument::fromJson(json());
    return doc.toJson();
}

QJsonObject Json::toObject()
{
    QJsonDocument doc = QJsonDocument::fromJson(json());
    return doc.object();
}

QJsonArray Json::toArray()
{
    QJsonDocument doc = QJsonDocument::fromJson(json());
    return doc.array();
}

bool Json::isValid()
{
    QJsonParseError parseError;
    QJsonDocument::fromJson(json(), &parseError);
    if (parseError.error == QJsonParseError::NoError) {
        return true;
    }
    return false;
}

bool Json::isObject()
{
    QJsonDocument doc = QJsonDocument::fromJson(json());
    return doc.isObject();
}

bool Json::isArray()
{
    QJsonDocument doc = QJsonDocument::fromJson(json());
    return doc.isArray();
}

} // namespace qtlibs
