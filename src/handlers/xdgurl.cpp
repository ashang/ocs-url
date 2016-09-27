#include <QDebug>
#include <QUrl>
#include <QUrlQuery>

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
        meta["filename"] = query.queryItemValue("filename", QUrl::FullyDecoded).split("?").at(0);
    }

    if (!meta["url"].toString().isEmpty() && meta["filename"].toString().isEmpty()) {
        meta["filename"] = QUrl(meta["url"].toString()).fileName();
    }

    return meta;
}

QJsonObject XdgUrl::_importDestinations()
{
    QJsonObject destinations;
    QJsonObject appConfigDestinations = _appConfig->get("destinations");
    QJsonObject appConfigDestinationsAlias = _appConfig->get("destinations_alias");

    foreach (const QString key, appConfigDestinations.keys()) {
        QString value = appConfigDestinations[key].toString();
        if (value.startsWith("$HOME")) {
            destinations[key] = value.replace("$HOME", Utility::File::homePath());
        }
        else if (value.startsWith("$XDG_DATA")) {
            destinations[key] = value.replace("$XDG_DATA", Utility::File::xdgDataHomePath());
        }
        else if (value.startsWith("$KDE_DATA")) {
            destinations[key] = value.replace("$KDE_DATA", Utility::File::kdeDataHomePath());
        }
    }

    foreach (const QString key, appConfigDestinationsAlias.keys()) {
        QString value = appConfigDestinationsAlias[key].toString();
        if (destinations.contains(value)) {
            destinations[key] = destinations.value(value);
        }
    }

    return destinations;
}

bool XdgUrl::_installPlasmapkg(const QString &path, const QString &type)
{
    return true;
}

bool XdgUrl::_uncompressArchive(const QString &path, const QString &targetDir)
{
    return true;
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

bool XdgUrl::process()
{
    return true;
}

} // namespace Handlers
