QT += \
    core \
    gui \
    qml \
    quick \
    svg

HEADERS += \
    $${PWD}/handlers/ocsurlhandler.h

SOURCES += \
    $${PWD}/main.cpp \
    $${PWD}/handlers/ocsurlhandler.cpp

RESOURCES += \
    $${PWD}/configs/configs.qrc \
    $${PWD}/qml/qml.qrc

INCLUDEPATH += $${PWD}
