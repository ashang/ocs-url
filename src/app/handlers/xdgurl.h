#pragma once

#include <QObject>
#include <QJsonObject>

#include "qtlibs/config.h"

namespace qtlibs {
class NetworkResource;
}

namespace handlers {

class XdgUrl : public QObject
{
    Q_OBJECT

public:
    explicit XdgUrl(const QString &xdgUrl, const qtlibs::Config &config, QObject *parent = 0);

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
    void networkResourceFinished(qtlibs::NetworkResource *resource);

private:
    void parse();
    void loadDestinations();
    QString convertPathString(const QString &path);
    void saveDownloadedFile(qtlibs::NetworkResource *resource);
    void installDownloadedFile(qtlibs::NetworkResource *resource);

    QString xdgUrl_;
    qtlibs::Config config_;
    QJsonObject metadata_;
    QJsonObject destinations_;
    QString destination_;
};

} // namespace handlers
