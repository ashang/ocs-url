#include <QtGlobal>
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

#include "utils/config.h"
#include "utils/network.h"
#include "handlers/xdgurl.h"

int main(int argc, char *argv[])
{
    // Init
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    utils::Config *config = new utils::Config(":/configs");
    utils::Network *network = new utils::Network(true);

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
    qmlContext->setContextProperty("xdgUrlHandler", new handlers::XdgUrl(xdgUrl, config, network));
    qmlAppEngine.load(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}
