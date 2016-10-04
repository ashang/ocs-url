#ifndef HANDLERS_XDGURL_H
#define HANDLERS_XDGURL_H

#include <QObject>
#include <QJsonObject>

//class QNetworkReply;

namespace Core {
class Config;
class Network;
}

namespace Handlers {

class XdgUrl : public QObject
{
    Q_OBJECT

private:
    QString _xdgUrl;
    Core::Config *_appConfig;
    Core::Config *_userConfig;
    Core::Network *_asyncNetwork;

    QJsonObject _meta;
    QJsonObject _destinations;
    QJsonObject _archiveTypes;

public:
    explicit XdgUrl(const QString &xdgUrl, Core::Config *appConfig, Core::Config *userConfig, Core::Network *asyncNetwork, QObject *parent = 0);

private:
    QJsonObject _parse();
    QString _convertPathString(const QString &path);
    QJsonObject _importDestinations();
    QJsonObject _importArchiveTypes();
    bool _installPlasmapkg(const QString &path, const QString &type = "plasmoid");
    bool _uncompressArchive(const QString &path, const QString &targetDir);
    bool _download();
    bool _install();

private slots:

public slots:
    QString getXdgUrl();
    bool isValid();
    bool process();

signals:
    void downloaded(const QString &metadata);
    void installed(const QString &metadata);

};

} // namespace Handlers

#endif // HANDLERS_XDGURL_H
