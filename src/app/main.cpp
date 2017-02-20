#include <QString>
#include <QStringList>
#include <QUrl>
#include <QJsonObject>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "handlers/confighandler.h"
#include "handlers/ocsurlhandler.h"

int main(int argc, char *argv[])
{
    // Init
    QGuiApplication app(argc, argv);

    ConfigHandler *configHandler = new ConfigHandler();
    QJsonObject appConfigApplication = configHandler->getAppConfigApplication();

    app.setApplicationName(appConfigApplication["name"].toString());
    app.setApplicationVersion(appConfigApplication["version"].toString());
    app.setOrganizationName(appConfigApplication["organization"].toString());
    app.setOrganizationDomain(appConfigApplication["domain"].toString());
    app.setWindowIcon(QIcon::fromTheme(appConfigApplication["id"].toString(), QIcon(appConfigApplication["icon"].toString())));

    // Setup CLI
    QCommandLineParser clParser;
    clParser.setApplicationDescription(appConfigApplication["description"].toString());
    clParser.addHelpOption();
    clParser.addVersionOption();
    clParser.addPositionalArgument("OCS-URL", "OCS-URL that starts with ocs://");
    clParser.process(app);

    QStringList args = clParser.positionalArguments();

    if (args.size() != 1) {
        clParser.showHelp(1);
    }

    QString ocsUrl = args.at(0);

    // Setup QML
    QQmlApplicationEngine qmlAppEngine;
    QQmlContext *qmlContext = qmlAppEngine.rootContext();
    configHandler->setParent(&qmlAppEngine);
    qmlContext->setContextProperty("configHandler", configHandler);
    qmlContext->setContextProperty("ocsUrlHandler", new OcsUrlHandler(ocsUrl, configHandler, &qmlAppEngine));
    qmlAppEngine.load(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
