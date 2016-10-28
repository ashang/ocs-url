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
#include <QJsonObject>

namespace utils {

class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(const QString &configsDir, QObject *parent = 0);

    QJsonObject get(const QString &name);
    bool set(const QString &name, const QJsonObject &jsonObj);

private:
    QString configsDir_;
    QJsonObject cacheData_;
};

} // namespace utils
