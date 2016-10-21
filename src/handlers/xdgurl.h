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
    QString xdgUrl_;
    Core::Config *config_;
    Core::Network *network_;

    QJsonObject metadata_;
    QJsonObject destinations_;
    QString destination_;

public:
    explicit XdgUrl(const QString &xdgUrl, Core::Config *config, Core::Network *network, QObject *parent = 0);

private:
    void parse_();
    void loadDestinations_();
    QString convertPathString_(const QString &path);
    void saveDownloadedFile_(QNetworkReply *reply);
    void installDownloadedFile_(QNetworkReply *reply);

public slots:
    void process();
    void openDestination();
    bool isValid();
    QString getXdgUrl();
    QJsonObject getMetadata();

private slots:
    void downloaded_(QNetworkReply *reply);

signals:
    void started();
    void finished(const QJsonObject &result);
    void error(const QJsonObject &result);
    void downloadProgress(const qint64 &received, const qint64 &total);
};

} // namespace Handlers

#endif // HANDLERS_XDGURL_H
