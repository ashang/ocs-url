QT += \
    core \
    network

HEADERS += \
    $${PWD}/file.h \
    $${PWD}/dir.h \
    $${PWD}/json.h \
    $${PWD}/config.h \
    $${PWD}/networkresource.h \
    $${PWD}/ocsapi.h \
    $${PWD}/package.h

SOURCES += \
    $${PWD}/file.cpp \
    $${PWD}/dir.cpp \
    $${PWD}/json.cpp \
    $${PWD}/config.cpp \
    $${PWD}/networkresource.cpp \
    $${PWD}/ocsapi.cpp \
    $${PWD}/package.cpp

unix:!ios:!android {
    DEFINES += QTLIBS_UNIX
}

android {
    QT += androidextras
}
