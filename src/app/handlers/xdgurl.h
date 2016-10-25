#pragma once

#include <QObject>
#include <QJsonObject>

class QNetworkReply;

namespace utils {
class Config;
class Network;
}

namespace handlers {

class XdgUrl : public QObject
{
    Q_OBJECT

public:
    explicit XdgUrl(const QString &xdgUrl, utils::Config *config, utils::Network *network, QObject *parent = 0);

signals:
    void started();
    void finished(const QJsonObject &result);
    void error(const QJsonObject &result);
    void downloadProgress(const qint64 &received, const qint64 &total);

public slots:
    void process();
    bool isValid();
    void openDestination();
    QString xdgUrl() const;
    QJsonObject metadata() const;

private slots:
    void downloaded(QNetworkReply *reply);

private:
    void parse();
    void loadDestinations();
    QString convertPathString(const QString &path);
    void saveDownloadedFile(QNetworkReply *reply);
    void installDownloadedFile(QNetworkReply *reply);

    QString xdgUrl_;
    utils::Config *config_;
    utils::Network *network_;

    QJsonObject metadata_;
    QJsonObject destinations_;
    QString destination_;
};

} // namespace handlers
