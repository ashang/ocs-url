#include "../utility/file.h"
#include "../utility/json.h"

#include "config.h"

namespace Core {

Config::Config(const QString &configsDir, QObject *parent) :
    QObject(parent), configsDir_(configsDir)
{}

QJsonObject Config::get(const QString &name)
{
    QString configFile = configsDir_ + "/" + name + ".json";

    if (!cacheData_.contains(name)) {
        QString json = Utility::File::readText(configFile);
        if (json.isEmpty()) {
            json = "{}"; // Blank JSON data as default
        }
        cacheData_[name] = Utility::Json::convertStrToObj(json);
    }
    return cacheData_[name].toObject();
}

bool Config::set(const QString &name, const QJsonObject &jsonObj)
{
    QString configFile = configsDir_ + "/" + name + ".json";
    QString json = Utility::Json::convertObjToStr(jsonObj);

    Utility::File::makeDir(configsDir_);
    if (Utility::File::writeText(configFile, json)) {
        cacheData_[name] = jsonObj;
        return true;
    }
    return false;
}

} // namespace Core
