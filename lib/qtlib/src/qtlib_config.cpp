/**
 * qtlib
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0
 * @link        https://github.com/akiraohgaki/qtlib
 */

#include "qtlib_config.h"

#include "qtlib_file.h"
#include "qtlib_dir.h"
#include "qtlib_json.h"

namespace qtlib {

Config::Config(const QString &configDirPath, QObject *parent)
    : QObject(parent), configDirPath_(configDirPath)
{}

Config::Config(const Config &other, QObject *parent)
    : QObject(parent)
{
    setConfigDirPath(other.configDirPath());
}

Config &Config::operator =(const Config &other)
{
    setConfigDirPath(other.configDirPath());
    return *this;
}

QString Config::configDirPath() const
{
    return configDirPath_;
}

void Config::setConfigDirPath(const QString &configDirPath)
{
    configDirPath_ = configDirPath;
}

QJsonObject Config::get(const QString &name)
{
    QString configFilePath = configDirPath() + "/" + name + ".json";
    QByteArray json = qtlib::File(configFilePath).readData();
    if (json.isEmpty()) {
        json = QString("{}").toUtf8(); // Blank JSON data as default
    }
    return qtlib::Json(json).toObject();
}

bool Config::set(const QString &name, const QJsonObject &object)
{
    QString configFilePath = configDirPath() + "/" + name + ".json";
    QByteArray json = qtlib::Json(object).toJson();
    qtlib::Dir(configDirPath()).make();
    return qtlib::File(configFilePath).writeData(json);
}

} // namespace qtlib
