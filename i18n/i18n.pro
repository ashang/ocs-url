SOURCES += $$system(find $${PWD}/../app -type f -name "*.cpp" -or -name "*.qml" -or -name "*.js")

TRANSLATIONS += \
    $${PWD}/messages.ts \
    $${PWD}/messages.en_US.ts \
    $${PWD}/messages.ja_JP.ts \
    $${PWD}/messages.tr_TR.ts \
    $${PWD}/messages.zh_TW.ts

include($${PWD}/i18n.pri)
