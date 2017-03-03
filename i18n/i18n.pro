SOURCES += $$system(find ../app -type f -name "*.cpp" -or -name "*.qml" -or -name "*.js")

TRANSLATIONS += \
    messages.ts \
    messages.en_US.ts \
    messages.tr_TR.ts

include(i18n.pri)
