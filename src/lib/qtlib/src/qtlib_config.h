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

namespace qtlib {

class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(const QString &configDirPath = "", QObject *parent = 0);

    Config(const Config &other, QObject *parent = 0);
    Config &operator =(const Config &other);

    QString configDirPath() const;
    void setConfigDirPath(const QString &configDirPath);

    QJsonObject get(const QString &name);
    bool set(const QString &name, const QJsonObject &object);

private:
    QString configDirPath_;
};

} // namespace qtlib
