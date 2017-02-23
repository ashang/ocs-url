unix:!ios:!android {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    SRCDIR = $${PWD}
    BINDIR = $${PREFIX}/bin
    DATADIR = $${PREFIX}/share

    target.path = $${BINDIR}

    desktop.files = $${SRCDIR}/desktop/$${TARGET}.desktop
    desktop.path = $${DATADIR}/applications

    icon.files = $${SRCDIR}/desktop/$${TARGET}.svg
    icon.path = $${DATADIR}/icons/hicolor/scalable/apps

    INSTALLS += target desktop icon
}

export(INSTALLS)
