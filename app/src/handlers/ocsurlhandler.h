#pragma once

#include <QObject>
#include <QJsonObject>

namespace qtlib {
class NetworkResource;
}

class ConfigHandler;

class OcsUrlHandler : public QObject
{
    Q_OBJECT

public:
    explicit OcsUrlHandler(const QString &ocsUrl, ConfigHandler *configHandler, QObject *parent = 0);

signals:
    void started();
    void finishedWithSuccess(QJsonObject result);
    void finishedWithError(QJsonObject result);
    void downloadProgress(QString id, qint64 bytesReceived, qint64 bytesTotal);

public slots:
    QString ocsUrl() const;
    QJsonObject metadata() const;

    void process();
    bool isValid();
    void openDestination();

private slots:
    void networkResourceFinished(qtlib::NetworkResource *resource);

private:
    void parse();
    void saveDownloadedFile(qtlib::NetworkResource *resource);
    void installDownloadedFile(qtlib::NetworkResource *resource);

    QString ocsUrl_;
    ConfigHandler *configHandler_;
    QJsonObject metadata_;
};
