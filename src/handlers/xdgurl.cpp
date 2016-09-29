#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QMimeDatabase>
#include <QProcess>

#include "../core/config.h"
#include "../core/network.h"
#include "../utility/file.h"
#include "../utility/json.h"

#include "xdgurl.h"

namespace Handlers {

XdgUrl::XdgUrl(const QString &xdgUrl, Core::Config *appConfig, Core::Config *userConfig, Core::Network *asyncNetwork, QObject *parent) :
    QObject(parent), _xdgUrl(xdgUrl), _appConfig(appConfig), _userConfig(userConfig), _asyncNetwork(asyncNetwork)
{
    _meta = _parse();
    _destinations = _importDestinations();
    _archiveTypes = _importArchiveTypes();
}

QJsonObject XdgUrl::_parse()
{
    QUrl url(_xdgUrl);
    QUrlQuery query(url);
    QJsonObject meta;

    meta["scheme"] = QString("xdg");
    meta["command"] = QString("download");
    meta["url"] = QString("");
    meta["type"] = QString("downloads");
    meta["filename"] = QString("");

    if (!url.scheme().isEmpty()) {
        meta["scheme"] = url.scheme();
    }

    if (!url.host().isEmpty()) {
        meta["command"] = url.host();
    }

    if (query.hasQueryItem("url") && !query.queryItemValue("url").isEmpty()) {
        meta["url"] = query.queryItemValue("url", QUrl::FullyDecoded);
    }

    if (query.hasQueryItem("type") && !query.queryItemValue("type").isEmpty()) {
        meta["type"] = query.queryItemValue("type", QUrl::FullyDecoded);
    }

    if (query.hasQueryItem("filename") && !query.queryItemValue("filename").isEmpty()) {
        meta["filename"] = QUrl(query.queryItemValue("filename", QUrl::FullyDecoded)).fileName();
    }

    if (!meta["url"].toString().isEmpty() && meta["filename"].toString().isEmpty()) {
        meta["filename"] = QUrl(meta["url"].toString()).fileName();
    }

    return meta;
}

QString XdgUrl::_convertPathString(const QString &path)
{
    QString newPath = path;

    if (newPath.contains("$HOME")) {
        newPath.replace("$HOME", Utility::File::homePath());
    }
    else if (newPath.contains("$XDG_DATA")) {
        newPath.replace("$XDG_DATA", Utility::File::xdgDataHomePath());
    }
    else if (newPath.contains("$KDE_DATA")) {
        newPath.replace("$KDE_DATA", Utility::File::kdeDataHomePath());
    }

    return newPath;
}

QJsonObject XdgUrl::_importDestinations()
{
    QJsonObject destinations;
    QJsonObject appConfigDestinations = _appConfig->get("destinations");
    QJsonObject appConfigDestinationsAlias = _appConfig->get("destinations_alias");
    QJsonObject userConfigDestinations = _userConfig->get("destinations");
    QJsonObject userConfigDestinationsAlias = _userConfig->get("destinations_alias");

    foreach (const QString key, appConfigDestinations.keys()) {
        destinations[key] = _convertPathString(appConfigDestinations[key].toString());
    }

    foreach (const QString key, appConfigDestinationsAlias.keys()) {
        QString value = appConfigDestinationsAlias[key].toString();
        if (destinations.contains(value)) {
            destinations[key] = destinations.value(value);
        }
    }

    if (!userConfigDestinations.isEmpty()) {
        foreach (const QString key, userConfigDestinations.keys()) {
            destinations[key] = _convertPathString(userConfigDestinations[key].toString());
        }
    }

    if (!userConfigDestinationsAlias.isEmpty()) {
        foreach (const QString key, userConfigDestinationsAlias.keys()) {
            QString value = userConfigDestinationsAlias[key].toString();
            if (destinations.contains(value)) {
                destinations[key] = destinations.value(value);
            }
        }
    }

    return destinations;
}

QJsonObject XdgUrl::_importArchiveTypes()
{
    QJsonObject archiveTypes;
    QJsonObject appConfigArchiveTypes = _appConfig->get("archive_types");
    QJsonObject userConfigArchiveTypes = _userConfig->get("archive_types");

    archiveTypes = appConfigArchiveTypes;

    if (!userConfigArchiveTypes.isEmpty()) {
        foreach (const QString key, userConfigArchiveTypes.keys()) {
            archiveTypes[key] = userConfigArchiveTypes.value(key);
        }
    }

    return archiveTypes;
}

bool XdgUrl::_installPlasmapkg(const QString &path, const QString &type)
{
    QProcess process;
    QStringList arguments;
    arguments << "-t" << type << "-i" << path;

    process.start("plasmapkg2", arguments);

    if (process.waitForFinished()) {
        return true;
    }

    return false;
}

bool XdgUrl::_uncompressArchive(const QString &path, const QString &targetDir)
{
    QMimeDatabase mimeDb;
    QString mimeType = mimeDb.mimeTypeForFile(path).name();
    QString archiveType;

    QProcess process;
    QString program;
    QStringList arguments;

    if (_archiveTypes.contains(mimeType)) {
        archiveType = _archiveTypes[mimeType].toString();

        if (archiveType == "tar") {
            program = "tar";
            arguments << "-xf" << path << "-C" << targetDir;
        }
        else if (archiveType == "zip") {
            program = "unzip";
            arguments << "-o" << path << "-d" << targetDir;
        }
        else if (archiveType == "7z") {
            program = "7z";
            arguments << "x" << path << "-o" + targetDir; // No space between -o and directory
        }
        else if (archiveType == "rar") {
            program = "unrar";
            arguments << "e" << path << targetDir;
        }

        process.start(program, arguments);

        if (process.waitForFinished()) {
            process.waitForReadyRead();
            return true;
        }
    }

    return false;
}

bool XdgUrl::_download()
{
    return true;
}

bool XdgUrl::_install()
{
    return true;
}

/**
 * Private slots
 */

/**
 * Public slots
 */

bool XdgUrl::isValid()
{
    bool isValid = true;

    if (_meta["scheme"].toString() != "xdg" && _meta["scheme"].toString() != "xdgs") {
        isValid = false;
    }

    if (_meta["command"].toString() != "download" && _meta["command"].toString() != "install") {
        isValid = false;
    }

    if (!QUrl(_meta["url"].toString()).isValid()) {
        isValid = false;
    }

    if (!_destinations.contains(_meta["type"].toString())) {
        isValid = false;
    }

    if (_meta["filename"].toString().isEmpty()) {
        isValid = false;
    }

    return isValid;
}

bool XdgUrl::process()
{
    return true;
}

} // namespace Handlers
