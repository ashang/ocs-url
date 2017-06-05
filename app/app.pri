CONFIG += c++11

QT += \
    core \
    gui \
    widgets \
    qml \
    quick \
    svg

HEADERS += \
    $${PWD}/src/handlers/confighandler.h \
    $${PWD}/src/handlers/ocsurlhandler.h

SOURCES += \
    $${PWD}/src/main.cpp \
    $${PWD}/src/handlers/confighandler.cpp \
    $${PWD}/src/handlers/ocsurlhandler.cpp

RESOURCES += \
    $${PWD}/configs/configs.qrc \
    $${PWD}/images/images.qrc \
    $${PWD}/qml/qml.qrc

INCLUDEPATH += $${PWD}/src
