include(src/lib/qtlib/qtlib.pri)

include(src/app/app.pri)

TARGET = xdgurl

TEMPLATE = app

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += src/desktop/desktop.qrc

DISTFILES += \
    README.md \
    src/desktop/xdgurl.desktop \
    src/desktop/appimage-desktopintegration \
    pkg/build.sh \
    pkg/ubuntu/debian/changelog \
    pkg/ubuntu/debian/compat \
    pkg/ubuntu/debian/control \
    pkg/ubuntu/debian/copyright \
    pkg/ubuntu/debian/rules \
    pkg/ubuntu/debian/xdgurl.install \
    pkg/fedora/xdgurl.spec \
    pkg/arch/PKGBUILD

include(deployment.pri)
