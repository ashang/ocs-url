unix:!android {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    SRCDIR = src
    BINDIR = $${PREFIX}/bin
    DATADIR = $${PREFIX}/share

    target.path = $${BINDIR}

    desktopentry.files = $${SRCDIR}/desktop/$${TARGET}.desktop
    desktopentry.path = $${DATADIR}/applications

    icon.files = $${SRCDIR}/desktop/$${TARGET}.svg
    icon.path = $${DATADIR}/icons/hicolor/scalable/apps

    INSTALLS += target desktopentry icon
}

export(INSTALLS)
