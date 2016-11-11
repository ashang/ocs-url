#pragma once

#include <QObject>
#include <QJsonObject>

namespace qtlibs {
class Config;
class NetworkResource;
}

namespace handlers {

class XdgUrl : public QObject
{
    Q_OBJECT

public:
    explicit XdgUrl(const QString &xdgUrl, qtlibs::Config *config, QObject *parent = 0);

signals:
    void started();
    void finished(const QJsonObject &result);
    void error(const QJsonObject &result);
    void downloadProgress(const qint64 &bytesReceived, const qint64 &bytesTotal);

public slots:
    QString xdgUrl() const;
    QJsonObject metadata() const;

    void process();
    bool isValid();
    void openDestination();

private slots:
    void downloaded(qtlibs::NetworkResource *resource);

private:
    void parse();
    void loadDestinations();
    QString convertPathString(const QString &path);
    void saveDownloadedFile(qtlibs::NetworkResource *resource);
    void installDownloadedFile(qtlibs::NetworkResource *resource);

    QString xdgUrl_;
    qtlibs::Config *config_;
    QJsonObject metadata_;
    QJsonObject destinations_;
    QString destination_;
};

} // namespace handlers
