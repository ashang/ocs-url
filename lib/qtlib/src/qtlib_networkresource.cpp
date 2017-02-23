/**
 * qtlib
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0
 * @link        https://github.com/akiraohgaki/qtlib
 */

#include "qtlib_networkresource.h"

#include <QEventLoop>

#include "qtlib_file.h"

namespace qtlib {

NetworkResource::NetworkResource(const QString &id, const QUrl &url, bool async, QObject *parent)
    : QObject(parent), id_(id), url_(url), async_(async)
{
    setManager(new QNetworkAccessManager(this));
}

NetworkResource::~NetworkResource()
{
    manager()->deleteLater();
}

NetworkResource::NetworkResource(const NetworkResource &other, QObject *parent)
    : QObject(parent)
{
    setId(other.id());
    setUrl(other.url());
    setAsync(other.async());
    setRequest(other.request());
    setManager(new QNetworkAccessManager(this));
}

NetworkResource &NetworkResource::operator =(const NetworkResource &other)
{
    setId(other.id());
    setUrl(other.url());
    setAsync(other.async());
    setRequest(other.request());
    return *this;
}

QString NetworkResource::id() const
{
    return id_;
}

void NetworkResource::setId(const QString &id)
{
    id_ = id;
}

QUrl NetworkResource::url() const
{
    return url_;
}

void NetworkResource::setUrl(const QUrl &url)
{
    url_ = url;
}

bool NetworkResource::async() const
{
    return async_;
}

void NetworkResource::setAsync(bool async)
{
    async_ = async;
}

QNetworkRequest NetworkResource::request() const
{
    return request_;
}

void NetworkResource::setRequest(const QNetworkRequest &request)
{
    request_ = request;
}

QNetworkAccessManager *NetworkResource::manager() const
{
    return manager_;
}

QNetworkReply *NetworkResource::reply() const
{
    return reply_;
}

QString NetworkResource::method() const
{
    return method_;
}

QString NetworkResource::contentType() const
{
    return contentType_;
}

QByteArray NetworkResource::contentData() const
{
    return contentData_;
}

NetworkResource *NetworkResource::head()
{
    setMethod("HEAD");
    return send(url(), async());
}

NetworkResource *NetworkResource::get()
{
    setMethod("GET");
    return send(url(), async());
}

NetworkResource *NetworkResource::post(const QByteArray &contentData, const QString &contentType)
{
    setMethod("POST");
    setContentType(contentType);
    setContentData(contentData);
    return send(url(), async());
}

NetworkResource *NetworkResource::post(const QUrlQuery &contentData)
{
    setMethod("POST");
    setContentType("application/x-www-form-urlencoded");
    setContentData(contentData.toString(QUrl::FullyEncoded).toUtf8());
    return send(url(), async());
}

NetworkResource *NetworkResource::put(const QByteArray &contentData, const QString &contentType)
{
    setMethod("PUT");
    setContentType(contentType);
    setContentData(contentData);
    return send(url(), async());
}

NetworkResource *NetworkResource::put(const QUrlQuery &contentData)
{
    setMethod("PUT");
    setContentType("application/x-www-form-urlencoded");
    setContentData(contentData.toString(QUrl::FullyEncoded).toUtf8());
    return send(url(), async());
}

NetworkResource *NetworkResource::deleteResource()
{
    setMethod("DELETE");
    return send(url(), async());
}

bool NetworkResource::isFinishedWithNoError()
{
    if (reply()->isFinished() && reply()->error() == QNetworkReply::NoError) {
        return true;
    }
    return false;
}

QByteArray NetworkResource::readData()
{
    QByteArray data;
    if (isFinishedWithNoError()) {
        data = reply()->readAll();
    }
    return data;
}

bool NetworkResource::saveData(const QString &path)
{
    if (isFinishedWithNoError()) {
        return qtlib::File(path).writeData(reply()->readAll());
    }
    return false;
}

void NetworkResource::abort()
{
    if (reply()->isRunning()) {
        reply()->abort();
    }
}

void NetworkResource::replyFinished()
{
    if (isFinishedWithNoError()) {
        // Check if redirection
        // Note: An auto redirection option is available since Qt 5.6
        QUrl redirectUrl;
        if (reply()->hasRawHeader("Location")) {
            redirectUrl.setUrl(QString(reply()->rawHeader("Location")));
        }
        else if (reply()->hasRawHeader("Refresh")) {
            redirectUrl.setUrl(QString(reply()->rawHeader("Refresh")).split("url=").last());
        }
        if (!redirectUrl.isEmpty()) {
            if (redirectUrl.isRelative()) {
                redirectUrl = reply()->url().resolved(redirectUrl);
            }
            reply()->deleteLater();
            send(redirectUrl, true);
            return;
        }
    }
    emit finished(this);
}

void NetworkResource::replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit downloadProgress(id(), bytesReceived, bytesTotal);
}

void NetworkResource::replyUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    emit uploadProgress(id(), bytesSent, bytesTotal);
}

void NetworkResource::setManager(QNetworkAccessManager *manager)
{
    manager_ = manager;
}

void NetworkResource::setReply(QNetworkReply *reply)
{
    reply_ = reply;
}

void NetworkResource::setMethod(const QString &method)
{
    method_ = method;
}

void NetworkResource::setContentType(const QString &contentType)
{
    contentType_ = contentType;
}

void NetworkResource::setContentData(const QByteArray &contentData)
{
    contentData_ = contentData;
}

NetworkResource *NetworkResource::send(const QUrl &url, bool async)
{
    QNetworkRequest networkRequest = request();
    networkRequest.setUrl(url);
    if (method() == "HEAD") {
        setReply(manager()->head(networkRequest));
    }
    else if (method() == "GET") {
        setReply(manager()->get(networkRequest));
    }
    else if (method() == "POST") {
        networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(contentType()));
        setReply(manager()->post(networkRequest, contentData()));
    }
    else if (method() == "PUT") {
        networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(contentType()));
        setReply(manager()->put(networkRequest, contentData()));
    }
    else if (method() == "DELETE") {
        setReply(manager()->deleteResource(networkRequest));
    }
    else {
        Q_ASSERT(false);
    }
    connect(reply(), &QNetworkReply::finished, this, &NetworkResource::replyFinished);
    connect(reply(), &QNetworkReply::downloadProgress, this, &NetworkResource::replyDownloadProgress);
    connect(reply(), &QNetworkReply::uploadProgress, this, &NetworkResource::replyUploadProgress);
    if (!async) {
        QEventLoop eventLoop;
        connect(this, &NetworkResource::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();
    }
    return this;
}

} // namespace qtlib
