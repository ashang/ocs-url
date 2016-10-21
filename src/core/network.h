#ifndef CORE_NETWORK_H
#define CORE_NETWORK_H

#include <QObject>

class QEventLoop;
class QNetworkAccessManager;
class QNetworkReply;

namespace Core {

class Network : public QObject
{
    Q_OBJECT

private:
    bool async_;
    QNetworkAccessManager *manager_;
    QEventLoop *eventLoop_;

public:
    explicit Network(const bool &async = true, QObject *parent = 0);
    ~Network();

    QNetworkReply *head(const QUrl &uri);
    QNetworkReply *get(const QUrl &uri);

signals:
    void finished(QNetworkReply *reply);
    void downloadProgress(const qint64 &received, const qint64 &total);
};

} // namespace Core

#endif // CORE_NETWORK_H
