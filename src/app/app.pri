QT += \
    core \
    gui \
    qml \
    quick \
    svg

HEADERS += \
    $${PWD}/handlers/xdgurlhandler.h

SOURCES += \
    $${PWD}/main.cpp \
    $${PWD}/handlers/xdgurlhandler.cpp

RESOURCES += \
    $${PWD}/configs/configs.qrc \
    $${PWD}/qml/qml.qrc
