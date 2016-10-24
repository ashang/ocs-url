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
    src/main.cpp \
    src/handlers/xdgurl.cpp \
    src/utils/config.cpp \
    src/utils/network.cpp \
    src/utils/file.cpp \
    src/utils/json.cpp \
    src/utils/package.cpp

HEADERS += \
    src/handlers/xdgurl.h \
    src/utils/config.h \
    src/utils/network.h \
    src/utils/file.h \
    src/utils/json.h \
    src/utils/package.h

RESOURCES += \
    src/configs/configs.qrc \
    src/qml/qml.qrc \
    src/desktop/desktop.qrc

# Additional RPATH
#include(rpath.pri)

# Deployment rules
include(deployment.pri)
