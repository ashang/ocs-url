#ifndef HANDLERS_XDGURL_H
#define HANDLERS_XDGURL_H

#include <QObject>
#include <QJsonObject>

class QNetworkReply;

namespace core {
class Config;
class Network;
}

namespace handlers {

class XdgUrl : public QObject
{
    Q_OBJECT

public:
    explicit XdgUrl(const QString &xdgUrl, core::Config *config, core::Network *network, QObject *parent = 0);

signals:
    void started();
    void finished(const QJsonObject &result);
    void error(const QJsonObject &result);
    void downloadProgress(const qint64 &received, const qint64 &total);

public slots:
    void process();
    void openDestination();
    bool isValid();
    QString getXdgUrl();
    QJsonObject getMetadata();

private slots:
    void downloaded_(QNetworkReply *reply);

private:
    void parse_();
    void loadDestinations_();
    QString convertPathString_(const QString &path);
    void saveDownloadedFile_(QNetworkReply *reply);
    void installDownloadedFile_(QNetworkReply *reply);

    QString xdgUrl_;
    core::Config *config_;
    core::Network *network_;

    QJsonObject metadata_;
    QJsonObject destinations_;
    QString destination_;
};

} // namespace handlers

#endif // HANDLERS_XDGURL_H
