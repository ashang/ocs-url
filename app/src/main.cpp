#include <QString>
#include <QStringList>
#include <QUrl>
#include <QJsonObject>
#include <QTranslator>
#include <QLocale>
#include <QCommandLineParser>
#include <QApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "handlers/confighandler.h"
#include "handlers/ocsurlhandler.h"

int main(int argc, char *argv[])
{
    // Init
    QApplication app(argc, argv);

    auto *configHandler = new ConfigHandler();
    auto appConfigApplication = configHandler->getAppConfigApplication();

    app.setApplicationName(appConfigApplication["name"].toString());
    app.setApplicationVersion(appConfigApplication["version"].toString());
    app.setOrganizationName(appConfigApplication["organization"].toString());
    app.setOrganizationDomain(appConfigApplication["domain"].toString());
    app.setWindowIcon(QIcon::fromTheme(appConfigApplication["id"].toString(), QIcon(appConfigApplication["icon"].toString())));

    // Setup translator
    QTranslator translator;
    if (translator.load(QLocale(), "messages", ".", ":/i18n")) {
        app.installTranslator(&translator);
    }

    // Setup CLI
    QCommandLineParser clParser;
    clParser.setApplicationDescription(appConfigApplication["description"].toString());
    clParser.addHelpOption();
    clParser.addVersionOption();
    clParser.addPositionalArgument("OCS-URL", "OCS-URL that starts with ocs://");
    clParser.process(app);

    auto args = clParser.positionalArguments();

    if (args.size() != 1) {
        clParser.showHelp(1);
    }

    auto ocsUrl = QString(args.at(0));

    // Setup QML
    QQmlApplicationEngine qmlAppEngine;
    auto *qmlContext = qmlAppEngine.rootContext();
    configHandler->setParent(&qmlAppEngine);
    qmlContext->setContextProperty("configHandler", configHandler);
    qmlContext->setContextProperty("ocsUrlHandler", new OcsUrlHandler(ocsUrl, configHandler, &qmlAppEngine));
    qmlAppEngine.load(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
