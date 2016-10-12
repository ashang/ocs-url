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

XdgUrl::XdgUrl(const QString &xdgUrl, Core::Config *config, Core::Network *network, QObject *parent) :
    QObject(parent), _xdgUrl(xdgUrl), _config(config), _network(network)
{
    _parse();
    _loadDestinations();

    connect(_network, &Core::Network::finished, this, &XdgUrl::_downloaded);
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
    else if (newPath.contains("$XDG_DATA")) {
        newPath.replace("$XDG_DATA", Utility::File::xdgDataHomePath());
    }
    else if (newPath.contains("$KDE_DATA")) {
        newPath.replace("$KDE_DATA", Utility::File::kdeDataHomePath());
    }

    return newPath;
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

    if ((type == "plasma_plasmoids" || type == "plasma4_plasmoids" || type == "plasma5_plasmoids")
            && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "plasmoid")) {
        qInfo() << "The plasmoid has been installed";
    }
    else if ((type == "plasma_look_and_feel" || type == "plasma5_look_and_feel")
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "lookandfeel")) {
        qInfo() << "The plasma look and feel has been installed";
    }
    else if ((type == "plasma_desktopthemes" || type == "plasma5_desktopthemes")
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "theme")) {
        qInfo() << "The plasma desktop theme has been installed";
    }
    else if (type == "kwin_effects"
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "kwineffect")) {
        qInfo() << "The KWin effect has been installed";
    }
    else if (type == "kwin_scripts"
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "kwinscript")) {
        qInfo() << "The KWin script has been installed";
    }
    else if (type == "kwin_tabbox"
             && Utility::Package::installPlasmapkg(temporaryFile.fileName(), "windowswitcher")) {
        qInfo() << "The KWin window switcher has been installed";
    }
    else if (Utility::Package::uncompressArchive(temporaryFile.fileName(), destination)) {
        qInfo() << "The archive file has been uncompressed into" << destination;
    }
    else if (temporaryFile.copy(path)) {
        qInfo() << "Saved the file as" << path;
    }
    else {
        result["error"] = QString("install_error");
        emit finished(Utility::Json::convertObjToStr(result));
        return;
    }

    result["success"] = QString("install_success");
    result["destination"] = destination;
    result["path"] = path;
    emit finished(Utility::Json::convertObjToStr(result));
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

    if (isValid()) {
        _network->get(QUrl(_metadata["url"].toString()));
    }
}

} // namespace Handlers
