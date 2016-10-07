#ifndef HANDLERS_XDGURL_H
#define HANDLERS_XDGURL_H

#include <QObject>
#include <QJsonObject>

class QNetworkReply;

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

    QJsonObject _metadata;
    QJsonObject _destinations;
    QJsonObject _archiveTypes;

public:
    explicit XdgUrl(const QString &xdgUrl, Core::Config *appConfig, Core::Config *userConfig, Core::Network *asyncNetwork, QObject *parent = 0);

private:
    QJsonObject _parse();
    QString _convertPathString(const QString &path);
    QJsonObject _loadDestinations();
    QJsonObject _loadArchiveTypes();
    bool _installPlasmapkg(const QString &path, const QString &type = "plasmoid");
    bool _uncompressArchive(const QString &path, const QString &targetDir);

private slots:
    void _saveDownloadedFile(QNetworkReply *reply);
    void _installDownloadedFile(QNetworkReply *reply);

public slots:
    QString getXdgUrl();
    QString getMetadata();
    bool isValid();
    void process();

signals:
    void downloaded(const QString &result);
    void installed(const QString &result);

};

} // namespace Handlers

#endif // HANDLERS_XDGURL_H
