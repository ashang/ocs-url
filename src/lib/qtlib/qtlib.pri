QT += \
    core \
    network

HEADERS += \
    $${PWD}/qtlib_file.h \
    $${PWD}/qtlib_dir.h \
    $${PWD}/qtlib_json.h \
    $${PWD}/qtlib_config.h \
    $${PWD}/qtlib_networkresource.h \
    $${PWD}/qtlib_ocsapi.h \
    $${PWD}/qtlib_package.h

SOURCES += \
    $${PWD}/qtlib_file.cpp \
    $${PWD}/qtlib_dir.cpp \
    $${PWD}/qtlib_json.cpp \
    $${PWD}/qtlib_config.cpp \
    $${PWD}/qtlib_networkresource.cpp \
    $${PWD}/qtlib_ocsapi.cpp \
    $${PWD}/qtlib_package.cpp

INCLUDEPATH += $${PWD}

unix:!ios:!android {
    DEFINES += QTLIB_UNIX
}

android {
    QT += androidextras
}
