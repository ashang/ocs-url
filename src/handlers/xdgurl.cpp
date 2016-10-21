#include "xdgurl.h"

#include <QUrl>
#include <QUrlQuery>
#include <QTemporaryFile>
#include <QNetworkReply>
#include <QDesktopServices>

#include "../core/config.h"
#include "../core/network.h"
#include "../utility/file.h"
#include "../utility/package.h"

namespace handlers {

XdgUrl::XdgUrl(const QString &xdgUrl, core::Config *config, core::Network *network, QObject *parent) :
    QObject(parent), xdgUrl_(xdgUrl), config_(config), network_(network)
{
    parse();
    loadDestinations();

    connect(network_, &core::Network::finished, this, &XdgUrl::downloaded);
    connect(network_, &core::Network::downloadProgress, this, &XdgUrl::downloadProgress);
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
        result["message"] = QString("Invalid XDG-URL " + xdgUrl_);
        emit error(result);
        return;
    }

    network_->get(QUrl(metadata_["url"].toString()));
    emit started();
}

bool XdgUrl::isValid()
{
    QString scheme = metadata_["scheme"].toString();
    QString command = metadata_["command"].toString();
    QString url = metadata_["url"].toString();
    QString type = metadata_["type"].toString();
    QString filename = metadata_["filename"].toString();

    if ((scheme == "xdg" || scheme == "xdgs")
            && (command == "download" || command == "install")
            && QUrl(url).isValid()
            && destinations_.contains(type)
            && !filename.isEmpty()) {
        return true;
    }

    return false;
}

void XdgUrl::openDestination()
{
    if (!destination_.isEmpty()) {
        QDesktopServices::openUrl(QUrl("file://" + destination_));
    }
}

QString XdgUrl::xdgUrl() const
{
    return xdgUrl_;
}

QJsonObject XdgUrl::metadata() const
{
    return metadata_;
}

void XdgUrl::downloaded(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QJsonObject result;
        result["status"] = QString("error_network");
        result["message"] = reply->errorString();
        emit error(result);
        return;
    }

    if (reply->hasRawHeader("Location")) {
        QString redirectUrl = QString(reply->rawHeader("Location"));
        if (redirectUrl.startsWith("/")) {
            redirectUrl = reply->url().authority() + redirectUrl;
        }
        network_->get(QUrl(redirectUrl));
        return;
    }

    if (reply->hasRawHeader("Refresh")) {
        QString refreshUrl = QString(reply->rawHeader("Refresh")).split("url=").last();
        if (refreshUrl.startsWith("/")) {
            refreshUrl = reply->url().authority() + refreshUrl;
        }
        network_->get(QUrl(refreshUrl));
        return;
    }

    if (metadata_["command"].toString() == "download") {
        saveDownloadedFile(reply);
    }
    else if (metadata_["command"].toString() == "install") {
        installDownloadedFile(reply);
    }
}

void XdgUrl::parse()
{
    QUrl url(xdgUrl_);
    QUrlQuery query(url);

    metadata_["scheme"] = QString("xdg");
    metadata_["command"] = QString("download");
    metadata_["url"] = QString("");
    metadata_["type"] = QString("downloads");
    metadata_["filename"] = QString("");

    if (!url.scheme().isEmpty()) {
        metadata_["scheme"] = url.scheme();
    }

    if (!url.host().isEmpty()) {
        metadata_["command"] = url.host();
    }

    if (query.hasQueryItem("url") && !query.queryItemValue("url").isEmpty()) {
        metadata_["url"] = query.queryItemValue("url", QUrl::FullyDecoded);
    }

    if (query.hasQueryItem("type") && !query.queryItemValue("type").isEmpty()) {
        metadata_["type"] = query.queryItemValue("type", QUrl::FullyDecoded);
    }

    if (query.hasQueryItem("filename") && !query.queryItemValue("filename").isEmpty()) {
        metadata_["filename"] = QUrl(query.queryItemValue("filename", QUrl::FullyDecoded)).fileName();
    }

    if (!metadata_["url"].toString().isEmpty() && metadata_["filename"].toString().isEmpty()) {
        metadata_["filename"] = QUrl(metadata_["url"].toString()).fileName();
    }
}

void XdgUrl::loadDestinations()
{
    QJsonObject configDestinations = config_->get("destinations");
    QJsonObject configDestinationsAlias = config_->get("destinations_alias");

    foreach (const QString key, configDestinations.keys()) {
        destinations_[key] = convertPathString(configDestinations[key].toString());
    }

    foreach (const QString key, configDestinationsAlias.keys()) {
        QString value = configDestinationsAlias[key].toString();
        if (destinations_.contains(value)) {
            destinations_[key] = destinations_.value(value);
        }
    }
}

QString XdgUrl::convertPathString(const QString &path)
{
    QString newPath = path;

    if (newPath.contains("$HOME")) {
        newPath.replace("$HOME", utility::File::homePath());
    }
    else if (newPath.contains("$XDG_DATA_HOME")) {
        newPath.replace("$XDG_DATA_HOME", utility::File::xdgDataHomePath());
    }
    else if (newPath.contains("$KDEHOME")) {
        newPath.replace("$KDEHOME", utility::File::kdehomePath());
    }

    return newPath;
}

void XdgUrl::saveDownloadedFile(QNetworkReply *reply)
{
    QJsonObject result;

    QTemporaryFile temporaryFile;

    if (!temporaryFile.open() || temporaryFile.write(reply->readAll()) == -1) {
        result["status"] = QString("error_save");
        result["message"] = temporaryFile.errorString();
        emit error(result);
        return;
    }

    QString type = metadata_["type"].toString();
    QString destination = destinations_[type].toString();
    QString path = destination + "/" + metadata_["filename"].toString();

    utility::File::makeDir(destination);
    utility::File::remove(path); // Remove previous downloaded file

    if (!temporaryFile.copy(path)) {
        result["status"] = QString("error_save");
        result["message"] = temporaryFile.errorString();
        emit error(result);
        return;
    }

    destination_ = destination;

    result["status"] = QString("success_download");
    result["message"] = QString("The file has been stored into " + destination);
    emit finished(result);
}

void XdgUrl::installDownloadedFile(QNetworkReply *reply)
{
    QJsonObject result;

    QTemporaryFile temporaryFile;

    if (!temporaryFile.open() || temporaryFile.write(reply->readAll()) == -1) {
        result["status"] = QString("error_save");
        result["message"] = temporaryFile.errorString();
        emit error(result);
        return;
    }

    QString type = metadata_["type"].toString();
    QString destination = destinations_[type].toString();
    QString path = destination + "/" + metadata_["filename"].toString();

    utility::File::makeDir(destination);
    utility::File::remove(path); // Remove previous downloaded file

    if (type == "bin"
            && utility::Package::installProgram(temporaryFile.fileName(), path)) {
        result["message"] = QString("The program has been installed into " + destination);
    }
    else if ((type == "plasma_plasmoids" || type == "plasma4_plasmoids" || type == "plasma5_plasmoids")
             && utility::Package::installPlasmapkg(temporaryFile.fileName(), "plasmoid")) {
        result["message"] = QString("The plasmoid has been installed");
    }
    else if ((type == "plasma_look_and_feel" || type == "plasma5_look_and_feel")
             && utility::Package::installPlasmapkg(temporaryFile.fileName(), "lookandfeel")) {
        result["message"] = QString("The plasma look and feel has been installed");
    }
    else if ((type == "plasma_desktopthemes" || type == "plasma5_desktopthemes")
             && utility::Package::installPlasmapkg(temporaryFile.fileName(), "theme")) {
        result["message"] = QString("The plasma desktop theme has been installed");
    }
    else if (type == "kwin_effects"
             && utility::Package::installPlasmapkg(temporaryFile.fileName(), "kwineffect")) {
        result["message"] = QString("The KWin effect has been installed");
    }
    else if (type == "kwin_scripts"
             && utility::Package::installPlasmapkg(temporaryFile.fileName(), "kwinscript")) {
        result["message"] = QString("The KWin script has been installed");
    }
    else if (type == "kwin_tabbox"
             && utility::Package::installPlasmapkg(temporaryFile.fileName(), "windowswitcher")) {
        result["message"] = QString("The KWin window switcher has been installed");
    }
    else if (utility::Package::uncompressArchive(temporaryFile.fileName(), destination)) {
        result["message"] = QString("The archive file has been uncompressed into " + destination);
    }
    else if (temporaryFile.copy(path)) {
        result["message"] = QString("The file has been stored into " + destination);
    }
    else {
        result["status"] = QString("error_install");
        result["message"] = temporaryFile.errorString();
        emit error(result);
        return;
    }

    destination_ = destination;

    result["status"] = QString("success_install");
    emit finished(result);
}

} // namespace handlers
