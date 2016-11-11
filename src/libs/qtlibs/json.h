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

#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

namespace qtlibs {

class Json : public QObject
{
    Q_OBJECT

public:
    explicit Json(const QByteArray &json, QObject *parent = 0);
    explicit Json(const QString &string, QObject *parent = 0);
    explicit Json(const QJsonObject &object, QObject *parent = 0);
    explicit Json(const QJsonArray &array, QObject *parent = 0);

    QByteArray json() const;
    void setJson(const QByteArray &json);

    bool isValid();
    bool isObject();
    bool isArray();
    QByteArray toJson();
    QJsonObject toObject();
    QJsonArray toArray();

private:
    QByteArray json_;
};

} // namespace qtlibs
