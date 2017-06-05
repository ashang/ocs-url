message("Please execute scripts/import.sh for build dependencies")

TARGET = ocs-url

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

DISTFILES += $${PWD}/README.md

include($${PWD}/lib/lib.pri)
include($${PWD}/app/app.pri)
include($${PWD}/desktop/desktop.pri)
include($${PWD}/i18n/i18n.pri)
include($${PWD}/pkg/pkg.pri)
include($${PWD}/scripts/scripts.pri)
include($${PWD}/deployment.pri)
