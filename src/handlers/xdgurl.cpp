#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QTemporaryFile>
#include <QMimeDatabase>
#include <QNetworkReply>

#include "../core/config.h"
#include "../core/network.h"
#include "../utility/file.h"
#include "../utility/json.h"
#include "../utility/package.h"

#include "xdgurl.h"

namespace Handlers {

XdgUrl::XdgUrl(const QString &xdgUrl, Core::Config *appConfig, Core::Config *userConfig, Core::Network *asyncNetwork, QObject *parent) :
    QObject(parent), _xdgUrl(xdgUrl), _appConfig(appConfig), _userConfig(userConfig), _asyncNetwork(asyncNetwork)
{
    _metadata = _parse();
    _destinations = _loadDestinations();
}

QJsonObject XdgUrl::_parse()
{
    QUrl url(_xdgUrl);
    QUrlQuery query(url);
    QJsonObject metadata;

    metadata["scheme"] = QString("xdg");
    metadata["command"] = QString("download");
    metadata["url"] = QString("");
    metadata["type"] = QString("downloads");
    metadata["filename"] = QString("");

    if (!url.scheme().isEmpty()) {
        metadata["scheme"] = url.scheme();
    }

    if (!url.host().isEmpty()) {
        metadata["command"] = url.host();
    }

    if (query.hasQueryItem("url") && !query.queryItemValue("url").isEmpty()) {
        metadata["url"] = query.queryItemValue("url", QUrl::FullyDecoded);
    }

    if (query.hasQueryItem("type") && !query.queryItemValue("type").isEmpty()) {
        metadata["type"] = query.queryItemValue("type", QUrl::FullyDecoded);
    }

    if (query.hasQueryItem("filename") && !query.queryItemValue("filename").isEmpty()) {
        metadata["filename"] = QUrl(query.queryItemValue("filename", QUrl::FullyDecoded)).fileName();
    }

    if (!metadata["url"].toString().isEmpty() && metadata["filename"].toString().isEmpty()) {
        metadata["filename"] = QUrl(metadata["url"].toString()).fileName();
    }

    return metadata;
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

QJsonObject XdgUrl::_loadDestinations()
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

void XdgUrl::_saveDownloadedFile(QNetworkReply *reply)
{
    QJsonObject result;

    QTemporaryFile temporaryFile;

    if (!temporaryFile.open() || temporaryFile.write(reply->readAll()) == -1) {
        result["error"] = QString("save_error");
        emit finished(Utility::Json::convertObjToStr(result));
        return;
    }

    QMimeDatabase mimeDb;
    QString mimeType = mimeDb.mimeTypeForFile(temporaryFile.fileName()).name();

    if (mimeType == "text/html" || mimeType == "application/xhtml+xml") {
        result["error"] = QString("filetype_error");
        emit finished(Utility::Json::convertObjToStr(result));
        return;
    }

    QString type = _metadata["type"].toString();
    QString destination = _destinations[type].toString();
    QString path = destination + "/" + _metadata["filename"].toString();

    Utility::File::makeDir(destination);
    Utility::File::remove(path); // Remove previous downloaded file

    if (!temporaryFile.copy(path)) {
        result["error"] = QString("save_error");
        emit finished(Utility::Json::convertObjToStr(result));
        return;
    }

    result["success"] = QString("download_success");
    result["destination"] = destination;
    result["path"] = path;
    emit finished(Utility::Json::convertObjToStr(result));
}

void XdgUrl::_installDownloadedFile(QNetworkReply *reply)
{
}

/**
 * Private slots
 */

void XdgUrl::_downloaded(QNetworkReply *reply)
{
    QJsonObject result;

    if (reply->error() != QNetworkReply::NoError) {
        result["error"] = QString("network_error");
        emit finished(Utility::Json::convertObjToStr(result));
        return;
    }

    // If the network reply has a refresh header, retry download
    if (reply->hasRawHeader("Refresh")) {
        QString refreshUrl = QString(reply->rawHeader("Refresh")).split("url=").last();
        if (refreshUrl.startsWith("/")) {
            refreshUrl = reply->url().authority() + refreshUrl;
        }
        _asyncNetwork->get(QUrl(refreshUrl));
        return;
    }

    if (_metadata["command"].toString() == "download") {
        _saveDownloadedFile(reply);
    }
    else if (_metadata["command"].toString() == "install") {
        _installDownloadedFile(reply);
    }
}

/**
 * Public slots
 */

QString XdgUrl::getXdgUrl()
{
    return _xdgUrl;
}

QString XdgUrl::getMetadata()
{
    return Utility::Json::convertObjToStr(_metadata);
}

bool XdgUrl::isValid()
{
    bool isValid = true;

    if (_metadata["scheme"].toString() != "xdg" && _metadata["scheme"].toString() != "xdgs") {
        isValid = false;
    }

    if (_metadata["command"].toString() != "download" && _metadata["command"].toString() != "install") {
        isValid = false;
    }

    if (!QUrl(_metadata["url"].toString()).isValid()) {
        isValid = false;
    }

    if (!_destinations.contains(_metadata["type"].toString())) {
        isValid = false;
    }

    if (_metadata["filename"].toString().isEmpty()) {
        isValid = false;
    }

    return isValid;
}

void XdgUrl::process()
{
    /**
     * xdgs scheme is a reserved name, so the process of xdgs
     * is the same process of the xdg scheme currently.
     */

    if (isValid()) {
        connect(_asyncNetwork, &Core::Network::finished, this, &XdgUrl::_downloaded);
        _asyncNetwork->get(QUrl(_metadata["url"].toString()));
    }
}

} // namespace Handlers
