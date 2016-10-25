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

#include "config.h"

#include "file.h"
#include "json.h"

namespace utils {

Config::Config(const QString &configsDir, QObject *parent) :
    QObject(parent), configsDir_(configsDir)
{}

QJsonObject Config::get(const QString &name)
{
    QString configFile = configsDir_ + "/" + name + ".json";

    if (!cacheData_.contains(name)) {
        QString json = utils::File::readText(configFile);
        if (json.isEmpty()) {
            json = "{}"; // Blank JSON data as default
        }
        cacheData_[name] = utils::Json::convertStrToObj(json);
    }
    return cacheData_[name].toObject();
}

bool Config::set(const QString &name, const QJsonObject &jsonObj)
{
    QString configFile = configsDir_ + "/" + name + ".json";
    QString json = utils::Json::convertObjToStr(jsonObj);

    utils::File::makeDir(configsDir_);
    if (utils::File::writeText(configFile, json)) {
        cacheData_[name] = jsonObj;
        return true;
    }
    return false;
}

} // namespace utils
