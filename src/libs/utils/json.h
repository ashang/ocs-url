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

#pragma once

#include <QObject>

namespace utils {

class Json : public QObject
{
    Q_OBJECT

public:
    explicit Json(QObject *parent = 0);

    static QString convertObjToStr(const QJsonObject &jsonObj);
    static QJsonObject convertStrToObj(const QString &json);
    static bool isValid(const QString &json);
};

} // namespace utils
