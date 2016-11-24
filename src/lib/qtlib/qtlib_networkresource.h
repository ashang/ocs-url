/**
 * A library for Qt app
 *
 * LICENSE: The GNU Lesser General Public License, version 3.0
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0  The GNU Lesser General Public License, version 3.0
 * @link        https://github.com/akiraohgaki/qtlib
 */

#pragma once

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace qtlib {

class NetworkResource : public QObject
{
    Q_OBJECT

public:
    explicit NetworkResource(const QString &id = "", const QUrl &url = QUrl(), bool async = true, QObject *parent = 0);
    ~NetworkResource();

    NetworkResource(const NetworkResource &other, QObject *parent = 0);
    NetworkResource &operator =(const NetworkResource &other);

    QString id() const;
    void setId(const QString &id);
    QUrl url() const;
    void setUrl(const QUrl &url);
    bool async() const;
    void setAsync(bool async);
    QNetworkRequest request() const;
    void setRequest(const QNetworkRequest &request);
    QNetworkAccessManager *manager() const;
    QNetworkReply *reply() const;
    QString method() const;
    QString contentType() const;
    QByteArray contentData() const;

    NetworkResource *head();
    NetworkResource *get();
    NetworkResource *post(const QByteArray &contentData, const QString &contentType);
    NetworkResource *post(const QUrlQuery &contentData);
    NetworkResource *put(const QByteArray &contentData, const QString &contentType);
    NetworkResource *put(const QUrlQuery &contentData);
    NetworkResource *deleteResource();
    bool isFinishedWithNoError();
    QByteArray readData();
    bool saveData(const QString &path);

signals:
    void finished(NetworkResource *resource);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

public slots:
    void abort();

private slots:
    void replyFinished();

private:
    void setManager(QNetworkAccessManager *manager);
    void setReply(QNetworkReply *reply);
    void setMethod(const QString &method);
    void setContentType(const QString &contentType);
    void setContentData(const QByteArray &contentData);

    NetworkResource *send(const QUrl &url, bool async);

    QString id_;
    QUrl url_;
    bool async_;
    QNetworkRequest request_;
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QString method_;
    QString contentType_;
    QByteArray contentData_;
};

} // namespace qtlib
