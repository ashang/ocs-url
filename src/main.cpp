#include <QString>
#include <QStringList>
#include <QUrl>
#include <QJsonObject>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "core/config.h"
#include "core/network.h"
#include "handlers/xdgurl.h"

int main(int argc, char *argv[])
{
    // Init
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // Qt 5.6 or higher
    QGuiApplication app(argc, argv);
    Core::Config *config = new Core::Config(":/configs");
    Core::Network *network = new Core::Network(true);

    QJsonObject configApplication = config->get("application");

    app.setApplicationName(configApplication["name"].toString());
    app.setApplicationVersion(configApplication["version"].toString());
    app.setOrganizationName(configApplication["organization"].toString());
    app.setOrganizationDomain(configApplication["domain"].toString());
    app.setWindowIcon(QIcon::fromTheme(configApplication["id"].toString(), QIcon(configApplication["icon"].toString())));

    // Setup CLI
    QCommandLineParser clParser;
    clParser.setApplicationDescription(configApplication["description"].toString());
    clParser.addHelpOption();
    clParser.addVersionOption();
    clParser.addPositionalArgument("xdgurl", "XDG-URL");
    clParser.process(app);

    QStringList args = clParser.positionalArguments();

    if (args.size() != 1) {
        clParser.showHelp(1);
    }

    QString xdgUrl = args.at(0);

    // Setup QML
    QQmlApplicationEngine qmlAppEngine;
    QQmlContext *qmlContext = qmlAppEngine.rootContext();
    qmlContext->setContextProperty("xdgUrlHandler", new Handlers::XdgUrl(xdgUrl, config, network));
    qmlAppEngine.load(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
