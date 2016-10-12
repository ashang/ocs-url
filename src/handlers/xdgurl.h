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
    Core::Config *_config;
    Core::Network *_network;

    QJsonObject _metadata;
    QJsonObject _destinations;

public:
    explicit XdgUrl(const QString &xdgUrl, Core::Config *config, Core::Network *network, QObject *parent = 0);

private:
    void _parse();
    void _loadDestinations();
    QString _convertPathString(const QString &path);
    void _saveDownloadedFile(QNetworkReply *reply);
    void _installDownloadedFile(QNetworkReply *reply);

private slots:
    void _downloaded(QNetworkReply *reply);

public slots:
    QString getXdgUrl();
    QString getMetadata();
    bool isValid();
    void process();

signals:
    void finished(const QString &result);
};

} // namespace Handlers

#endif // HANDLERS_XDGURL_H
