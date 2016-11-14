/**
 * A library for Qt app
 *
 * LICENSE: The GNU Lesser General Public License, version 3.0
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0  The GNU Lesser General Public License, version 3.0
 * @link        https://github.com/akiraohgaki/qtlibs
 */

#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace qtlibs {

class NetworkResource : public QObject
{
    Q_OBJECT

public:
    explicit NetworkResource(const QString &name = QString(), const QUrl &url = QUrl(), const bool &async = true, QObject *parent = 0);
    ~NetworkResource();

    NetworkResource(const NetworkResource &other);
    NetworkResource &operator =(const NetworkResource &other);

    QString name() const;
    void setName(const QString &name);
    QUrl url() const;
    void setUrl(const QUrl &url);
    bool async() const;
    void setAsync(const bool &async);
    QNetworkRequest request() const;
    void setRequest(const QNetworkRequest &request);
    QNetworkAccessManager *manager() const;
    QNetworkReply *reply() const;
    QString method() const;

    NetworkResource *head();
    NetworkResource *get();
    QByteArray readData();
    bool saveData(const QString &path);

signals:
    void finished(NetworkResource *resource);
    void downloadProgress(const qint64 &bytesReceived, const qint64 &bytesTotal);

public slots:
    void abort();

private slots:
    void replyFinished();

private:
    void setManager(QNetworkAccessManager *manager);
    void setReply(QNetworkReply *reply);
    void setMethod(const QString &method);

    NetworkResource *send(const bool &async, const QNetworkRequest &request);

    QString name_;
    QUrl url_;
    bool async_;
    QNetworkRequest request_;
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QString method_;
};

} // namespace qtlibs
