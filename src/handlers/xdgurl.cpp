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

XdgUrl::XdgUrl(const QString &xdgUrl, Core::Config *config, Core::Network *network, QObject *parent) :
    QObject(parent), _xdgUrl(xdgUrl), _config(config), _network(network)
{
    _parse();
    _loadDestinations();

    connect(_network, &Core::Network::finished, this, &XdgUrl::_downloaded);
    connect(_network, &Core::Network::downloadProgress, this, &XdgUrl::downloadProgress);
}

void XdgUrl::_parse()
{
    QUrl url(_xdgUrl);
    QUrlQuery query(url);

    _metadata["scheme"] = QString("xdg");
    _metadata["command"] = QString("download");
    _metadata["url"] = QString("");
    _metadata["type"] = QString("downloads");
    _metadata["filename"] = QString("");

    if (!url.scheme().isEmpty()) {
        _metadata["scheme"] = url.scheme();
    }

    if (!url.host().isEmpty()) {
        _metadata["command"] = url.host();
    }

    if (query.hasQueryItem("url") && !query.queryItemValue("url").isEmpty()) {
        _metadata["url"] = query.queryItemValue("url", QUrl::FullyDecoded);
    }

    if (query.hasQueryItem("type") && !query.queryItemValue("type").isEmpty()) {
        _metadata["type"] = query.queryItemValue("type", QUrl::FullyDecoded);
    }

    if (query.hasQueryItem("filename") && !query.queryItemValue("filename").isEmpty()) {
        _metadata["filename"] = QUrl(query.queryItemValue("filename", QUrl::FullyDecoded)).fileName();
    }

    if (!_metadata["url"].toString().isEmpty() && _metadata["filename"].toString().isEmpty()) {
        _metadata["filename"] = QUrl(_metadata["url"].toString()).fileName();
    }
}

void XdgUrl::_loadDestinations()
{
    QJsonObject configDestinations = _config->get("destinations");
    QJsonObject configDestinationsAlias = _config->get("destinations_alias");

    foreach (const QString key, configDestinations.keys()) {
        _destinations[key] = _convertPathString(configDestinations[key].toString());
    }

    foreach (const QString key, configDestinationsAlias.keys()) {
        QString value = configDestinationsAlias[key].toString();
        if (_destinations.contains(value)) {
            _destinations[key] = _destinations.value(value);
        }
    }
}

QString XdgUrl::_convertPathString(const QString &path)
{
    QString newPath = path;

    if (newPath.contains("$HOME")) {
        newPath.replace("$HOME", Utility::File::homePath());
    }
    else if (newPath.contains("$XDG_DATA_HOME")) {
        newPath.replace("$XDG_DATA_HOME", Utility::File::xdgDataHomePath());
    }
    else if (newPath.contains("$KDEHOME")) {
        newPath.replace("$KDEHOME", Utility::File::kdehomePath());
    }

    return newPath;
}

void XdgUrl::_saveDownloadedFile(QNetworkReply *reply)
{
    QJsonObject result;

    QTemporaryFile temporaryFile;

    if (!temporaryFile.open() || temporaryFile.write(reply->readAll()) == -1) {
        result["status"] = QString("error_save");
        result["message"] = temporaryFile.errorString();
        emit error(Utility::Json::convertObjToStr(result));
        return;
    }

    QMimeDatabase mimeDb;
    QString mimeType = mimeDb.mimeTypeForFile(temporaryFile.fileName()).name();

    if (mimeType == "text/html" || mimeType == "application/xhtml+xml") {
        result["status"] = QString("error_filetype");
        result["message"] = QString("The file is unsupported file type " + mimeType);
        emit error(Utility::Json::convertObjToStr(result));
        return;
    }

    QString type = _metadata["type"].toString();
    QString destination = _destinations[type].toString();
    QString path = destination + "/" + _metadata["filename"].toString();

    Utility::File::makeDir(destination);
    Utility::File::remove(path); // Remove previous downloaded file

    if (!temporaryFile.copy(path)) {
        result["status"] = QString("error_save");
        result["message"] = temporaryFile.errorString();
        emit error(Utility::Json::convertObjToStr(result));
        return;
    }

    result["status"] = QString("success_download");
    result["message"] = QString("The file has been stored into " + destination);
    emit finished(Utility::Json::convertObjToStr(result));
}

void XdgUrl::_installDownloadedFile(QNetworkReply *reply)
{
    QJsonObject result;

    QTemporaryFile temporaryFile;

    if (!temporaryFile.open() || temporaryFile.write(reply->readAll()) == -1) {
        result["status"] = QString("error_save");
        result["message"] = temporaryFile.errorString();
        emit error(Utility::Json::convertObjToStr(result));
        return;
    }

    QMimeDatabase mimeDb;
    QString mimeType = mimeDb.mimeTypeForFile(temporaryFile.fileName()).name();

    if (mimeType == "text/html" || mimeType == "application/xhtml+xml") {
        result["status"] = QString("error_filetype");
        result["message"] = QString("The file is unsupported file type " + mimeType);
        emit error(Utility::Json::convertObjToStr(result));
        return;
    }

    QString type = _metadata["type"].toString();
    QString destination = _destinations[type].toString();
    QString path = destination + "/" + _metadata["filename"].toString();

    Utility::File::makeDir(destination);
    Utility::File::remove(path); // Remove previous downloaded file

    if ((type == "plasma_plasmoids" || type == "plasma4_plasmoids" || type == "plasma5_plasmoids")
            && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "plasmoid")) {
        result["message"] = QString("The plasmoid has been installed");
    }
    else if ((type == "plasma_look_and_feel" || type == "plasma5_look_and_feel")
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "lookandfeel")) {
        result["message"] = QString("The plasma look and feel has been installed");
    }
    else if ((type == "plasma_desktopthemes" || type == "plasma5_desktopthemes")
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "theme")) {
        result["message"] = QString("The plasma desktop theme has been installed");
    }
    else if (type == "kwin_effects"
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "kwineffect")) {
        result["message"] = QString("The KWin effect has been installed");
    }
    else if (type == "kwin_scripts"
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "kwinscript")) {
        result["message"] = QString("The KWin script has been installed");
    }
    else if (type == "kwin_tabbox"
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "windowswitcher")) {
        result["message"] = QString("The KWin window switcher has been installed");
    }
    else if (type == "appimages"
             && Utility::Package::installAppimage(temporaryFile.fileName(), path)) {
        result["message"] = QString("The Appimage has been installed into " + destination);
    }
    else if (Utility::Package::uncompressArchive(temporaryFile.fileName(), destination)) {
        result["message"] = QString("The archive file has been uncompressed into " + destination);
    }
    else if (temporaryFile.copy(path)) {
        result["message"] = QString("The file has been stored into " + destination);
    }
    else {
        result["status"] = QString("error_install");
        result["message"] = temporaryFile.errorString();
        emit error(Utility::Json::convertObjToStr(result));
        return;
    }

    result["status"] = QString("success_install");
    emit finished(Utility::Json::convertObjToStr(result));
}

/**
 * Private slots
 */

void XdgUrl::_downloaded(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QJsonObject result;
        result["status"] = QString("error_network");
        result["message"] = reply->errorString();
        emit error(Utility::Json::convertObjToStr(result));
        return;
    }

    if (reply->hasRawHeader("Location")) {
        QString redirectUrl = QString(reply->rawHeader("Location"));
        if (redirectUrl.startsWith("/")) {
            redirectUrl = reply->url().authority() + redirectUrl;
        }
        _network->get(QUrl(redirectUrl));
        return;
    }

    if (reply->hasRawHeader("Refresh")) {
        QString refreshUrl = QString(reply->rawHeader("Refresh")).split("url=").last();
        if (refreshUrl.startsWith("/")) {
            refreshUrl = reply->url().authority() + refreshUrl;
        }
        _network->get(QUrl(refreshUrl));
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
    QString scheme = _metadata["scheme"].toString();
    QString command = _metadata["command"].toString();
    QString url = _metadata["url"].toString();
    QString type = _metadata["type"].toString();
    QString filename = _metadata["filename"].toString();

    if ((scheme == "xdg" || scheme == "xdgs")
            && (command == "download" || command == "install")
            && QUrl(url).isValid()
            && _destinations.contains(type)
            && !filename.isEmpty()) {
        return true;
    }

    return false;
}

void XdgUrl::process()
{
    /**
     * xdgs scheme is a reserved name, so the process of xdgs
     * is the same process of the xdg scheme currently.
     */

    if (!isValid()) {
        QJsonObject result;
        result["status"] = QString("error_validation");
        result["message"] = QString("Invalid XDG-URL " + _xdgUrl);
        emit error(Utility::Json::convertObjToStr(result));
        return;
    }

    _network->get(QUrl(_metadata["url"].toString()));
    emit started();
}

} // namespace Handlers
