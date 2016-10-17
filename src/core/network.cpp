#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "network.h"

namespace Core {

Network::Network(const bool &async, QObject *parent) :
    QObject(parent), _async(async)
{
    _manager = new QNetworkAccessManager(this);
    connect(_manager, &QNetworkAccessManager::finished,
            this, &Network::finished);
    if (!_async) {
        _eventLoop = new QEventLoop();
        connect(_manager, &QNetworkAccessManager::finished,
                _eventLoop, &QEventLoop::quit);
    }
}

Network::~Network()
{
    _manager->deleteLater();
    if (!_async) {
        delete _eventLoop;
    }
}

QNetworkReply *Network::head(const QUrl &uri)
{
    QNetworkReply *reply = _manager->head(QNetworkRequest(uri));
    if (!_async) {
        _eventLoop->exec();
    }
    return reply;
}

QNetworkReply *Network::get(const QUrl &uri)
{
    QNetworkReply *reply = _manager->get(QNetworkRequest(uri));
    connect(reply, &QNetworkReply::downloadProgress,
            this, &Network::downloadProgress);
    if (!_async) {
        _eventLoop->exec();
    }
    return reply;
}

} // namespace Core
