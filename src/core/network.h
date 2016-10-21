#ifndef CORE_NETWORK_H
#define CORE_NETWORK_H

#include <QObject>

class QEventLoop;
class QNetworkAccessManager;
class QNetworkReply;

namespace core {

class Network : public QObject
{
    Q_OBJECT

public:
    explicit Network(const bool &async = true, QObject *parent = 0);
    ~Network();

    QNetworkReply *head(const QUrl &uri);
    QNetworkReply *get(const QUrl &uri);

signals:
    void finished(QNetworkReply *reply);
    void downloadProgress(const qint64 &received, const qint64 &total);

private:
    bool async_;
    QNetworkAccessManager *manager_;
    QEventLoop *eventLoop_;
};

} // namespace core

#endif // CORE_NETWORK_H
