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

bool XdgUrl::_installPlasmapkg(const QString &path, const QString &type)
{
}

bool XdgUrl::_uncompressArchive(const QString &path, const QString &targetDir)
{
}

bool XdgUrl::_download()
{
}

bool XdgUrl::_install()
{
}

/**
 * Private slots
 */

/**
 * Public slots
 */

bool XdgUrl::process()
{
}

} // namespace Handlers
