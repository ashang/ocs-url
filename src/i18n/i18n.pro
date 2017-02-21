SOURCES += $$system(find ../app -type f -name "*.cpp" -or -name "*.qml" -or -name "*.js")

TRANSLATIONS += \
    messages.ts \
    messages.en_US.ts

RESOURCES += i18n.qrc
