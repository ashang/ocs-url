#include "network.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace core {

Network::Network(const bool &async, QObject *parent) :
    async_(async), QObject(parent)
{
    manager_ = new QNetworkAccessManager(this);
    connect(manager_, &QNetworkAccessManager::finished,
            this, &Network::finished);
    if (!async_) {
        eventLoop_ = new QEventLoop();
        connect(manager_, &QNetworkAccessManager::finished,
                eventLoop_, &QEventLoop::quit);
    }
}

Network::~Network()
{
    manager_->deleteLater();
    if (!async_) {
        delete eventLoop_;
    }
}

QNetworkReply *Network::head(const QUrl &uri)
{
    QNetworkReply *reply = manager_->head(QNetworkRequest(uri));
    if (!async_) {
        eventLoop_->exec();
    }
    return reply;
}

QNetworkReply *Network::get(const QUrl &uri)
{
    QNetworkReply *reply = manager_->get(QNetworkRequest(uri));
    connect(reply, &QNetworkReply::downloadProgress,
            this, &Network::downloadProgress);
    if (!async_) {
        eventLoop_->exec();
    }
    return reply;
}

} // namespace core
