TARGET = ocs-url

TEMPLATE = app

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

DISTFILES += $${PWD}/README.md

include($${PWD}/lib/lib.pri)
include($${PWD}/app/app.pri)
include($${PWD}/desktop/desktop.pri)
include($${PWD}/i18n/i18n.pri)
include($${PWD}/pkg/pkg.pri)
include($${PWD}/scripts/scripts.pri)
include($${PWD}/deployment.pri)
