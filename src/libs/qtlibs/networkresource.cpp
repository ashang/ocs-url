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

#include "networkresource.h"

#include <QEventLoop>

#include "file.h"

namespace qtlibs {

NetworkResource::NetworkResource(const QString &name, const QUrl &url, const bool &async, QObject *parent)
    : QObject(parent), name_(name), url_(url), async_(async)
{
    setManager(new QNetworkAccessManager(this));
}

NetworkResource::~NetworkResource()
{
    manager()->deleteLater();
}

NetworkResource::NetworkResource(const NetworkResource &other)
{
    this->setParent(other.parent());
    setName(other.name());
    setUrl(other.url());
    setAsync(other.async());
    setRequest(other.request());
    setManager(other.manager());
    setReply(other.reply());
    setMethod(other.method());
}

NetworkResource &NetworkResource::operator =(const NetworkResource &other)
{
    manager()->deleteLater();

    this->setParent(other.parent());
    setName(other.name());
    setUrl(other.url());
    setAsync(other.async());
    setRequest(other.request());
    setManager(other.manager());
    setReply(other.reply());
    setMethod(other.method());
    return *this;
}

QString NetworkResource::name() const
{
    return name_;
}

void NetworkResource::setName(const QString &name)
{
    name_ = name;
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

void NetworkResource::setAsync(const bool &async)
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

NetworkResource *NetworkResource::head()
{
    setMethod("HEAD");
    QNetworkRequest networkRequest = request();
    networkRequest.setUrl(url());
    return send(async(), networkRequest);
}

NetworkResource *NetworkResource::get()
{
    setMethod("GET");
    QNetworkRequest networkRequest = request();
    networkRequest.setUrl(url());
    return send(async(), networkRequest);
}

QByteArray NetworkResource::readData()
{
    QByteArray data;
    if (reply()->isFinished()) {
        data = reply()->readAll();
    }
    return data;
}

bool NetworkResource::saveData(const QString &path)
{
    if (reply()->isFinished()) {
        return qtlibs::File(path).writeData(readData());
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
    if (reply()->error() == QNetworkReply::NoError) {
        // Check if redirection
        // Note: An auto redirection option is available since Qt 5.6
        QString newUrl;
        if (reply()->hasRawHeader("Location")) {
            newUrl = QString(reply()->rawHeader("Location"));
        }
        else if (reply()->hasRawHeader("Refresh")) {
            newUrl = QString(reply()->rawHeader("Refresh")).split("url=").last();
        }
        if (!newUrl.isEmpty()) {
            if (newUrl.startsWith("/")) {
                newUrl = reply()->url().authority() + newUrl;
            }
            QNetworkRequest networkRequest = request();
            networkRequest.setUrl(QUrl(newUrl));
            send(true, networkRequest);
            return;
        }
    }
    emit finished(this);
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

NetworkResource *NetworkResource::send(const bool &async, const QNetworkRequest &request)
{
    if (method() == "HEAD") {
        setReply(manager()->head(request));
    }
    else if (method() == "GET") {
        setReply(manager()->get(request));
        connect(reply(), &QNetworkReply::downloadProgress, this, &NetworkResource::downloadProgress);
    }
    connect(reply(), &QNetworkReply::finished, this, &NetworkResource::replyFinished);
    if (!async) {
        QEventLoop eventLoop;
        connect(this, &NetworkResource::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();
    }
    return this;
}

} // namespace qtlibs
