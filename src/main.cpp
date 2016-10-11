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
#include "utility/file.h"

int main(int argc, char *argv[])
{
    // Init
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // Qt 5.6 or higher
    QGuiApplication app(argc, argv);

    Core::Config *appConfig = new Core::Config(":/configs");
    QJsonObject appConfigApplication = appConfig->get("application");
    Core::Config *userConfig = new Core::Config(Utility::File::xdgConfigHomePath() + "/" + appConfigApplication["id"].toString());
    Core::Network *asyncNetwork = new Core::Network(true);

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
    clParser.addPositionalArgument("xdgurl", "XDG-URL");
    clParser.process(app);

    const QStringList args = clParser.positionalArguments();

    if (args.size() != 1) {
        clParser.showHelp(1);
    }

    const QString xdgUrl = args.at(0);

    // Setup QML
    QQmlApplicationEngine qmlAppEngine;
    QQmlContext *qmlContext = qmlAppEngine.rootContext();
    qmlContext->setContextProperty("xdgUrlHandler", new Handlers::XdgUrl(xdgUrl, appConfig, userConfig, asyncNetwork));
    qmlAppEngine.load(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
