#include "../utility/file.h"
#include "../utility/json.h"

#include "config.h"

namespace Core {

Config::Config(const QString &configsDir, QObject *parent) :
    QObject(parent), _configsDir(configsDir)
{}

QJsonObject Config::get(const QString &name)
{
    QString configFile = _configsDir + "/" + name + ".json";

    if (!_cacheData.contains(name)) {
        QString json = Utility::File::readText(configFile);
        if (json.isEmpty()) {
            json = "{}"; // Blank JSON data as default
        }
        _cacheData[name] = Utility::Json::convertStrToObj(json);
    }
    return _cacheData[name].toObject();
}

bool Config::set(const QString &name, const QJsonObject &jsonObj)
{
    QString configFile = _configsDir + "/" + name + ".json";
    QString json = Utility::Json::convertObjToStr(jsonObj);

    Utility::File::makeDir(_configsDir);
    if (Utility::File::writeText(configFile, json)) {
        _cacheData[name] = jsonObj;
        return true;
    }
    return false;
}

} // namespace Core
