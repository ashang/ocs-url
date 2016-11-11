include(src/app/app.pri)

include(src/libs/qtlibs/qtlibs.pri)

include(deployment.pri)

INCLUDEPATH += \
    src/app \
    src/libs

RESOURCES += \
    src/desktop/desktop.qrc

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
