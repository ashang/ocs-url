#ifndef HANDLERS_XDGURL_H
#define HANDLERS_XDGURL_H

#include <QObject>
//#include <QJsonObject>

//class QNetworkReply;

namespace Core {
class Config;
class Network;
}

namespace Handlers {

class XdgUrl : public QObject
{
    Q_OBJECT

private:
    QString _xdgurl;
    Core::Config *_appConfig;
    Core::Config *_userConfig;
    Core::Network *_asyncNetwork;

public:
    explicit XdgUrl(const QString &xdgurl, Core::Config *appConfig, Core::Config *userConfig, Core::Network *asyncNetwork, QObject *parent = 0);

private:
    bool _download();
    bool _install();

private slots:

public slots:
    bool process();

signals:
    void downloaded(const QString &metadata);
    void installed(const QString &metadata);

};

} // namespace Handlers

#endif // HANDLERS_XDGURL_H
