unix:!android {
    QMAKE_LFLAGS += -Wl,-rpath=\\\$\$ORIGIN
    QMAKE_LFLAGS += -Wl,-rpath=/usr/lib/$${TARGET}
    QMAKE_RPATH =
}
