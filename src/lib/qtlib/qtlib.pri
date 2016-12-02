QT += \
    core \
    network

HEADERS += \
    $${PWD}/src/qtlib_file.h \
    $${PWD}/src/qtlib_dir.h \
    $${PWD}/src/qtlib_json.h \
    $${PWD}/src/qtlib_config.h \
    $${PWD}/src/qtlib_networkresource.h \
    $${PWD}/src/qtlib_ocsapi.h \
    $${PWD}/src/qtlib_package.h

SOURCES += \
    $${PWD}/src/qtlib_file.cpp \
    $${PWD}/src/qtlib_dir.cpp \
    $${PWD}/src/qtlib_json.cpp \
    $${PWD}/src/qtlib_config.cpp \
    $${PWD}/src/qtlib_networkresource.cpp \
    $${PWD}/src/qtlib_ocsapi.cpp \
    $${PWD}/src/qtlib_package.cpp

INCLUDEPATH += $${PWD}/src

unix:!ios:!android {
    DEFINES += QTLIB_UNIX
}

android {
    QT += androidextras
}
