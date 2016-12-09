/**
 * qtlib
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0
 * @link        https://github.com/akiraohgaki/qtlib
 */

#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

namespace qtlib {

class Json : public QObject
{
    Q_OBJECT

public:
    explicit Json(const QByteArray &json = QByteArray(), QObject *parent = 0);
    explicit Json(const QJsonObject &object, QObject *parent = 0);
    explicit Json(const QJsonArray &array, QObject *parent = 0);

    Json(const Json &other, QObject *parent = 0);
    Json &operator =(const Json &other);

    QByteArray json() const;
    void setJson(const QByteArray &json);

    void fromObject(const QJsonObject &object);
    void fromArray(const QJsonArray &array);
    QByteArray toJson();
    QJsonObject toObject();
    QJsonArray toArray();
    bool isValid();
    bool isObject();
    bool isArray();

private:
    QByteArray json_;
};

} // namespace qtlib
