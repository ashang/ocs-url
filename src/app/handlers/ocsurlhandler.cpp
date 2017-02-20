#include "ocsurlhandler.h"

#include <QUrlQuery>
#include <QDesktopServices>

#include "qtlib_file.h"
#include "qtlib_dir.h"
#include "qtlib_networkresource.h"
#include "qtlib_package.h"

OcsUrlHandler::OcsUrlHandler(const QString &ocsUrl, const qtlib::Config &config, QObject *parent)
    : QObject(parent), ocsUrl_(ocsUrl), config_(config)
{
    parse();
    loadDestinations();
}

QString OcsUrlHandler::ocsUrl() const
{
    return ocsUrl_;
}

QJsonObject OcsUrlHandler::metadata() const
{
    return metadata_;
}

void OcsUrlHandler::process()
{
    // ocss scheme is a reserved name, so the process of ocss
    // is the same process of the ocs scheme currently.

    if (!isValid()) {
        QJsonObject result;
        result["status"] = QString("error_validation");
        result["message"] = QString("Invalid OCS-URL " + ocsUrl_);
        emit finishedWithError(result);
        return;
    }

    QString url = metadata_["url"].toString();
    qtlib::NetworkResource *resource = new qtlib::NetworkResource(url, QUrl(url), true, this);
    connect(resource, &qtlib::NetworkResource::downloadProgress, this, &OcsUrlHandler::downloadProgress);
    connect(resource, &qtlib::NetworkResource::finished, this, &OcsUrlHandler::networkResourceFinished);
    resource->get();
    emit started();
}

bool OcsUrlHandler::isValid()
{
    QString scheme = metadata_["scheme"].toString();
    QString command = metadata_["command"].toString();
    QString url = metadata_["url"].toString();
    QString type = metadata_["type"].toString();
    QString filename = metadata_["filename"].toString();

    // Still support xdg and xdgs schemes for backward compatibility
    if ((scheme == "ocs" || scheme == "ocss" || scheme == "xdg" || scheme == "xdgs")
            && (command == "download" || command == "install")
            && QUrl(url).isValid()
            && destinations_.contains(type)
            && !filename.isEmpty()) {
        return true;
    }
    return false;
}

void OcsUrlHandler::openDestination()
{
    QString type = metadata_["type"].toString();
    QDesktopServices::openUrl(QUrl("file://" + destinations_[type].toString()));
}

void OcsUrlHandler::networkResourceFinished(qtlib::NetworkResource *resource)
{
    if (!resource->isFinishedWithNoError()) {
        QJsonObject result;
        result["status"] = QString("error_network");
        result["message"] = resource->reply()->errorString();
        emit finishedWithError(result);
        resource->deleteLater();
        return;
    }

    if (metadata_["command"].toString() == "download") {
        saveDownloadedFile(resource);
    }
    else if (metadata_["command"].toString() == "install") {
        installDownloadedFile(resource);
    }
}

void OcsUrlHandler::parse()
{
    QUrl url(ocsUrl_);
    QUrlQuery query(url);

    metadata_["scheme"] = QString("ocs");
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

void OcsUrlHandler::loadDestinations()
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

QString OcsUrlHandler::convertPathString(const QString &path)
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

    return newPath;
}

void OcsUrlHandler::saveDownloadedFile(qtlib::NetworkResource *resource)
{
    QJsonObject result;

    QString type = metadata_["type"].toString();
    qtlib::Dir destDir(destinations_[type].toString());
    destDir.make();
    qtlib::File destFile(destDir.path() + "/" + metadata_["filename"].toString());

    if (!resource->saveData(destFile.path())) {
        result["status"] = QString("error_save");
        result["message"] = QString("Failed to save data as " + destFile.path());
        emit finishedWithError(result);
        resource->deleteLater();
        return;
    }

    result["status"] = QString("success_download");
    result["message"] = QString("The file has been stored into " + destDir.path());
    emit finishedWithSuccess(result);

    resource->deleteLater();
}

void OcsUrlHandler::installDownloadedFile(qtlib::NetworkResource *resource)
{
    QJsonObject result;

    qtlib::File tempFile(qtlib::Dir::tempPath() + "/" + metadata_["filename"].toString());

    if (!resource->saveData(tempFile.path())) {
        result["status"] = QString("error_save");
        result["message"] = QString("Failed to save data as " + tempFile.path());
        emit finishedWithError(result);
        resource->deleteLater();
        return;
    }

    qtlib::Package package(tempFile.path());
    QString type = metadata_["type"].toString();
    qtlib::Dir destDir(destinations_[type].toString());
    destDir.make();
    qtlib::File destFile(destDir.path() + "/" + metadata_["filename"].toString());

    if (type == "bin"
            && package.installAsProgram(destFile.path())) {
        result["message"] = QString("The file has been installed into " + destDir.path());
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
    else if (package.installAsArchive(destDir.path())) {
        result["message"] = QString("The archive file has been extracted into " + destDir.path());
    }
    else if (package.installAsFile(destFile.path())) {
        result["message"] = QString("The file has been installed into " + destDir.path());
    }
    else {
        result["status"] = QString("error_install");
        result["message"] = QString("Failed to installation");
        emit finishedWithError(result);
        tempFile.remove();
        return;
    }

    result["status"] = QString("success_install");
    emit finishedWithSuccess(result);

    tempFile.remove();
    resource->deleteLater();
}
