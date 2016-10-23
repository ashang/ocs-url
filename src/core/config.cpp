#include "config.h"

#include "../utility/file.h"
#include "../utility/json.h"

namespace core {

Config::Config(const QString &configsDir, QObject *parent) :
    configsDir_(configsDir), QObject(parent)
{}

QJsonObject Config::get(const QString &name)
{
    QString configFile = configsDir_ + "/" + name + ".json";

    if (!cacheData_.contains(name)) {
        QString json = utility::File::readText(configFile);
        if (json.isEmpty()) {
            json = "{}"; // Blank JSON data as default
        }
        cacheData_[name] = utility::Json::convertStrToObj(json);
    }
    return cacheData_[name].toObject();
}

bool Config::set(const QString &name, const QJsonObject &jsonObj)
{
    QString configFile = configsDir_ + "/" + name + ".json";
    QString json = utility::Json::convertObjToStr(jsonObj);

    utility::File::makeDir(configsDir_);
    if (utility::File::writeText(configFile, json)) {
        cacheData_[name] = jsonObj;
        return true;
    }
    return false;
}

} // namespace core
