#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "network.h"

namespace Core {

Network::Network(const bool &async, QObject *parent) :
    QObject(parent), _async(async)
{
    _manager = new QNetworkAccessManager();
    connect(_manager, &QNetworkAccessManager::finished,
            this, &Network::_finished);
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
            this, &Network::_downloadProgress);
    if (!_async) {
        _eventLoop->exec();
    }
    return reply;
}

/**
 * Private slots
 */

void Network::_finished(QNetworkReply *reply)
{
    emit finished(reply);
}

void Network::_downloadProgress(const qint64 &received, const qint64 &total)
{
    emit downloadProgress(received, total);
}

} // namespace Core
