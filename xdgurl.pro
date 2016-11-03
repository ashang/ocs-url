TARGET = xdgurl

TEMPLATE = app

QT += \
    core \
    gui \
    qml \
    quick \
    svg \
    network

CONFIG += c++11

SOURCES += \
    $${PWD}/src/app/main.cpp \
    $${PWD}/src/app/handlers/xdgurl.cpp \
    $${PWD}/src/libs/utils/config.cpp \
    $${PWD}/src/libs/utils/network.cpp \
    $${PWD}/src/libs/utils/file.cpp \
    $${PWD}/src/libs/utils/json.cpp \
    $${PWD}/src/libs/utils/package.cpp

HEADERS += \
    $${PWD}/src/app/handlers/xdgurl.h \
    $${PWD}/src/libs/utils/config.h \
    $${PWD}/src/libs/utils/network.h \
    $${PWD}/src/libs/utils/file.h \
    $${PWD}/src/libs/utils/json.h \
    $${PWD}/src/libs/utils/package.h

RESOURCES += \
    $${PWD}/src/app/configs/configs.qrc \
    $${PWD}/src/app/qml/qml.qrc \
    $${PWD}/src/desktop/desktop.qrc

DISTFILES += \
    $${PWD}/README.md \
    $${PWD}/src/desktop/xdgurl.desktop \
    $${PWD}/src/desktop/appimage-desktopintegration \
    $${PWD}/pkg/build.sh \
    $${PWD}/pkg/ubuntu/debian/changelog \
    $${PWD}/pkg/ubuntu/debian/compat \
    $${PWD}/pkg/ubuntu/debian/control \
    $${PWD}/pkg/ubuntu/debian/copyright \
    $${PWD}/pkg/ubuntu/debian/rules \
    $${PWD}/pkg/ubuntu/debian/xdgurl.install \
    $${PWD}/pkg/fedora/xdgurl.spec \
    $${PWD}/pkg/arch/PKGBUILD

# Additional RPATH
#include($${PWD}/rpath.pri)

# Deployment rules
include($${PWD}/deployment.pri)
