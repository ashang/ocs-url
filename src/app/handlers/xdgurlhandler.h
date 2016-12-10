#pragma once

#include <QObject>
#include <QJsonObject>

#include "qtlib_config.h"

namespace qtlib {
class NetworkResource;
}

class XdgUrlHandler : public QObject
{
    Q_OBJECT

public:
    explicit XdgUrlHandler(const QString &xdgUrl, const qtlib::Config &config, QObject *parent = 0);

signals:
    void started();
    void finishedWithSuccess(QJsonObject result);
    void finishedWithError(QJsonObject result);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

public slots:
    QString xdgUrl() const;
    QJsonObject metadata() const;

    void process();
    bool isValid();
    void openDestination();

private slots:
    void networkResourceFinished(qtlib::NetworkResource *resource);

private:
    void parse();
    void loadDestinations();
    QString convertPathString(const QString &path);
    void saveDownloadedFile(qtlib::NetworkResource *resource);
    void installDownloadedFile(qtlib::NetworkResource *resource);

    QString xdgUrl_;
    qtlib::Config config_;
    QJsonObject metadata_;
    QJsonObject destinations_;
};
