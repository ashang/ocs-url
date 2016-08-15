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
    src/core/config.cpp \
    src/core/network.cpp \
    src/handlers/item.cpp \
    src/utility/file.cpp \
    src/utility/json.cpp

HEADERS += \
    src/core/config.h \
    src/core/network.h \
    src/handlers/item.h \
    src/utility/file.h \
    src/utility/json.h

RESOURCES += \
    src/qml.qrc \
    src/configs.qrc

DISTFILES += \
    README.md

# Additional RPATH
#include(rpath.pri)

# Deployment rules
include(deployment.pri)
