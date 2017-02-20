TARGET = ocs-url

TEMPLATE = app

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += src/desktop/desktop.qrc

DISTFILES += \
    README.md \
    src/desktop/ocs-url.desktop \
    pkg/build.sh \
    pkg/ubuntu/debian/changelog \
    pkg/ubuntu/debian/compat \
    pkg/ubuntu/debian/control \
    pkg/ubuntu/debian/copyright \
    pkg/ubuntu/debian/rules \
    pkg/ubuntu/debian/ocs-url.install \
    pkg/fedora/ocs-url.spec \
    pkg/arch/PKGBUILD \
    pkg/snap/snapcraft.yaml \
    pkg/snap/setup/gui/ocs-url.desktop \
    pkg/appimage/appimage-desktopintegration

include(deployment.pri)

include(src/lib/qtlib/qtlib.pri)

include(src/app/app.pri)
