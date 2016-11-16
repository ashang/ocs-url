#include "xdgurl.h"

#include <QUrlQuery>
#include <QDesktopServices>

#include "qtlibs/file.h"
#include "qtlibs/dir.h"
#include "qtlibs/networkresource.h"
#include "qtlibs/package.h"

namespace handlers {

XdgUrl::XdgUrl(const QString &xdgUrl, const qtlibs::Config &config, QObject *parent)
    : QObject(parent), xdgUrl_(xdgUrl), config_(config)
{
    parse();
    loadDestinations();
}

QString XdgUrl::xdgUrl() const
{
    return xdgUrl_;
}

QJsonObject XdgUrl::metadata() const
{
    return metadata_;
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
        emit finishedWithError(result);
        return;
    }

    QString url = metadata_["url"].toString();
    qtlibs::NetworkResource *resource = new qtlibs::NetworkResource(url, QUrl(url), true, this);
    connect(resource, &qtlibs::NetworkResource::downloadProgress, this, &XdgUrl::downloadProgress);
    connect(resource, &qtlibs::NetworkResource::finished, this, &XdgUrl::networkResourceFinished);
    resource->get();
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

void XdgUrl::networkResourceFinished(qtlibs::NetworkResource *resource)
{
    if (resource->reply()->error() != QNetworkReply::NoError) {
        QJsonObject result;
        result["status"] = QString("error_network");
        result["message"] = resource->reply()->errorString();
        emit finishedWithError(result);
        return;
    }

    if (metadata_["command"].toString() == "download") {
        saveDownloadedFile(resource);
    }
    else if (metadata_["command"].toString() == "install") {
        installDownloadedFile(resource);
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
    QJsonObject configDestinations = config_.get("destinations");
    QJsonObject configDestinationsAlias = config_.get("destinations_alias");

    foreach (const QString &key, configDestinations.keys()) {
        destinations_[key] = convertPathString(configDestinations[key].toString());
    }

    foreach (const QString &key, configDestinationsAlias.keys()) {
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
        newPath.replace("$HOME", qtlibs::Dir::homePath());
    }
    else if (newPath.contains("$XDG_DATA_HOME")) {
        newPath.replace("$XDG_DATA_HOME", qtlibs::Dir::genericDataPath());
    }
    else if (newPath.contains("$KDEHOME")) {
        newPath.replace("$KDEHOME", qtlibs::Dir::kdehomePath());
    }

    return newPath;
}

void XdgUrl::saveDownloadedFile(qtlibs::NetworkResource *resource)
{
    QJsonObject result;

    QString type = metadata_["type"].toString();
    QString destination = destinations_[type].toString();
    QString path = destination + "/" + metadata_["filename"].toString();

    qtlibs::Dir(destination).make();

    if (!resource->saveData(path)) {
        result["status"] = QString("error_save");
        result["message"] = QString("Failed to save data as " + path);
        emit finishedWithError(result);
        return;
    }

    destination_ = destination;

    result["status"] = QString("success_download");
    result["message"] = QString("The file has been stored into " + destination);
    emit finishedWithSuccess(result);
}

void XdgUrl::installDownloadedFile(qtlibs::NetworkResource *resource)
{
    QJsonObject result;

    QString tempPath = qtlibs::Dir::tempPath() + "/" + metadata_["filename"].toString();

    if (!resource->saveData(tempPath)) {
        result["status"] = QString("error_save");
        result["message"] = QString("Failed to save data as " + tempPath);
        emit finishedWithError(result);
        return;
    }

    qtlibs::Package package(tempPath);
    qtlibs::File tempFile(tempPath);

    QString type = metadata_["type"].toString();
    QString destination = destinations_[type].toString();
    QString path = destination + "/" + metadata_["filename"].toString();

    qtlibs::Dir(destination).make();

    if (type == "bin"
            && package.installAsProgram(path)) {
        result["message"] = QString("The file has been installed into " + destination);
    }
    else if ((type == "plasma_plasmoids" || type == "plasma4_plasmoids" || type == "plasma5_plasmoids")
             && package.installAsPlasmapkg("plasmoid")) {
        result["message"] = QString("The plasmoid has been installed");
    }
    else if ((type == "plasma_look_and_feel" || type == "plasma5_look_and_feel")
             && package.installAsPlasmapkg("lookandfeel")) {
        result["message"] = QString("The plasma look and feel has been installed");
    }
    else if ((type == "plasma_desktopthemes" || type == "plasma5_desktopthemes")
             && package.installAsPlasmapkg("theme")) {
        result["message"] = QString("The plasma desktop theme has been installed");
    }
    else if (type == "kwin_effects"
             && package.installAsPlasmapkg("kwineffect")) {
        result["message"] = QString("The KWin effect has been installed");
    }
    else if (type == "kwin_scripts"
             && package.installAsPlasmapkg("kwinscript")) {
        result["message"] = QString("The KWin script has been installed");
    }
    else if (type == "kwin_tabbox"
             && package.installAsPlasmapkg("windowswitcher")) {
        result["message"] = QString("The KWin window switcher has been installed");
    }
    else if (package.installAsArchive(destination)) {
        result["message"] = QString("The archive file has been extracted into " + destination);
    }
    else if (package.installAsFile(path)) {
        result["message"] = QString("The file has been installed into " + destination);
    }
    else {
        tempFile.remove();
        result["status"] = QString("error_install");
        result["message"] = QString("Failed to installation");
        emit finishedWithError(result);
        return;
    }

    tempFile.remove();

    destination_ = destination;

    result["status"] = QString("success_install");
    emit finishedWithSuccess(result);
}

} // namespace handlers
