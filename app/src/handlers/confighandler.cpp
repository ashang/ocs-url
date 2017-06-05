#include "confighandler.h"

#include <QStringList>
#include <QStandardPaths>

#include "qtlib_dir.h"

ConfigHandler::ConfigHandler(QObject *parent)
    : QObject(parent)
{
    appConfig_ = qtlib::Config(":/configs");
    importAppConfigApplication();
    importAppConfigInstallTypes();
}

QJsonObject ConfigHandler::getAppConfigApplication() const
{
    return appConfigApplication_;
}

QJsonObject ConfigHandler::getAppConfigInstallTypes() const
{
    return appConfigInstallTypes_;
}

void ConfigHandler::importAppConfigApplication()
{
    appConfigApplication_ = appConfig_.get("application");
}

void ConfigHandler::importAppConfigInstallTypes()
{
    auto installTypes = appConfig_.get("install_types");
    for (const auto &key : installTypes.keys()) {
        auto installtype = installTypes[key].toObject();
        installtype["destination"] = convertPathString(installtype["destination"].toString());
        installtype["generic_destination"] = convertPathString(installtype["generic_destination"].toString());
        installTypes[key] = installtype;
    }
    auto installTypesAlias = appConfig_.get("install_types_alias");
    for (const auto &key : installTypesAlias.keys()) {
        auto installTypeAlias = installTypesAlias[key].toObject();
        auto baseKey = installTypeAlias["base"].toString();
        if (installTypes.contains(baseKey)) {
            auto installType = installTypes[baseKey].toObject();
            installType["base"] = baseKey;
            installType["name"] = installTypeAlias["name"].toString();
            installTypes[key] = installType;
        }
    }
    appConfigInstallTypes_ = installTypes;
}

QString ConfigHandler::convertPathString(const QString &path) const
{
    auto newPath = path;
    if (newPath.contains("$HOME")) {
        newPath.replace("$HOME", qtlib::Dir::homePath());
    }
    else if (newPath.contains("$XDG_DOCUMENTS_DIR")) {
        newPath.replace("$XDG_DOCUMENTS_DIR", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    }
    else if (newPath.contains("$XDG_DOWNLOAD_DIR")) {
        newPath.replace("$XDG_DOWNLOAD_DIR", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    }
    else if (newPath.contains("$XDG_PICTURES_DIR")) {
        newPath.replace("$XDG_PICTURES_DIR", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    }
    else if (newPath.contains("$XDG_MUSIC_DIR")) {
        newPath.replace("$XDG_MUSIC_DIR", QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
    }
    else if (newPath.contains("$XDG_VIDEOS_DIR")) {
        newPath.replace("$XDG_VIDEOS_DIR", QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
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
