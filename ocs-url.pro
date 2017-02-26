TARGET = ocs-url

TEMPLATE = app

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

DISTFILES += README.md

include(lib/lib.pri)
include(app/app.pri)
include(desktop/desktop.pri)
include(i18n/i18n.pri)
include(pkg/pkg.pri)
include(scripts/scripts.pri)

include(deployment.pri)
