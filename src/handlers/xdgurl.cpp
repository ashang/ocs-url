#include <QDebug>

#include "../core/config.h"
#include "../core/network.h"
#include "../utility/file.h"
#include "../utility/json.h"

#include "xdgurl.h"

namespace Handlers {

XdgUrl::XdgUrl(const QString &xdgUrl, Core::Config *appConfig, Core::Config *userConfig, Core::Network *asyncNetwork, QObject *parent) :
    QObject(parent), _xdgUrl(xdgUrl), _appConfig(appConfig), _userConfig(userConfig), _asyncNetwork(asyncNetwork)
{
}

QJsonObject XdgUrl::_parse()
{
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
