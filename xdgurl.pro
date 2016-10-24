TARGET = xdgurl

TEMPLATE = app

QT += \
    core \
    gui \
    widgets \
    qml \
    quick \
    svg \
    network

CONFIG += \
    c++11

SOURCES += \
    src/app/main.cpp \
    src/app/handlers/xdgurl.cpp \
    src/libs/utils/config.cpp \
    src/libs/utils/network.cpp \
    src/libs/utils/file.cpp \
    src/libs/utils/json.cpp \
    src/libs/utils/package.cpp

HEADERS += \
    src/app/handlers/xdgurl.h \
    src/libs/utils/config.h \
    src/libs/utils/network.h \
    src/libs/utils/file.h \
    src/libs/utils/json.h \
    src/libs/utils/package.h

RESOURCES += \
    src/app/configs/configs.qrc \
    src/app/qml/qml.qrc \
    src/desktop/desktop.qrc

# Additional RPATH
#include(rpath.pri)

# Deployment rules
include(deployment.pri)
