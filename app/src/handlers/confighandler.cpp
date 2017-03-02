#include "confighandler.h"

#include <QStringList>

#include "qtlib_dir.h"

ConfigHandler::ConfigHandler(QObject *parent)
    : QObject(parent)
{
    appConfig_ = qtlib::Config(":/configs");
}

QJsonObject ConfigHandler::getAppConfigApplication()
{
    if (appConfigApplication_.isEmpty()) {
        appConfigApplication_ = appConfig_.get("application");
    }
    return appConfigApplication_;
}

QJsonObject ConfigHandler::getAppConfigInstallTypes()
{
    if (appConfigInstallTypes_.isEmpty()) {
        QJsonObject installTypes = appConfig_.get("install_types");
        foreach (const QString &key, installTypes.keys()) {
            QJsonObject installtype = installTypes[key].toObject();
            installtype["destination"] = convertPathString(installtype["destination"].toString());
            installtype["generic_destination"] = convertPathString(installtype["generic_destination"].toString());
            installTypes[key] = installtype;
        }
        QJsonObject installTypesAlias = appConfig_.get("install_types_alias");
        foreach (const QString &key, installTypesAlias.keys()) {
            QJsonObject installTypeAlias = installTypesAlias[key].toObject();
            QString baseKey = installTypeAlias["base"].toString();
            if (installTypes.contains(baseKey)) {
                QJsonObject installType = installTypes[baseKey].toObject();
                installType["base"] = baseKey;
                installType["name"] = installTypeAlias["name"].toString();
                installTypes[key] = installType;
            }
        }
        appConfigInstallTypes_ = installTypes;
    }
    return appConfigInstallTypes_;
}

QString ConfigHandler::convertPathString(const QString &path)
{
    QString newPath = path;
    if (newPath.contains("$HOME")) {
        newPath.replace("$HOME", qtlib::Dir::homePath());
    }
    else if (newPath.contains("$XDG_DATA_HOME")) {
        newPath.replace("$XDG_DATA_HOME", qtlib::Dir::genericDataPath());
    }
    else if (newPath.contains("$KDEHOME")) {
        newPath.replace("$KDEHOME", qtlib::Dir::kdehomePath());
    }
    else if (newPath.contains("$APP_DATA")) {
        newPath.replace("$APP_DATA", qtlib::Dir::genericDataPath() + "/" + getAppConfigApplication()["id"].toString());
    }
    return newPath;
}
