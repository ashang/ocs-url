QT += \
    core \
    gui \
    qml \
    quick \
    svg

HEADERS += \
    $${PWD}/handlers/xdgurl.h

SOURCES += \
    $${PWD}/main.cpp \
    $${PWD}/handlers/xdgurl.cpp

RESOURCES += \
    $${PWD}/configs/configs.qrc \
    $${PWD}/qml/qml.qrc
