#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include <QObject>
#include <QJsonObject>

namespace Core {

class Config : public QObject
{
    Q_OBJECT

private:
    QString _configsDir;
    QJsonObject _cacheData;

public:
    explicit Config(const QString &configsDir, QObject *parent = 0);

    QJsonObject get(const QString &name);
    bool set(const QString &name, const QJsonObject &jsonObj);
};

} // namespace Core

#endif // CORE_CONFIG_H
