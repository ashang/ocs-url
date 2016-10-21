#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include <QObject>
#include <QJsonObject>

namespace core {

class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(const QString &configsDir, QObject *parent = 0);

    QJsonObject get(const QString &name);
    bool set(const QString &name, const QJsonObject &jsonObj);

private:
    QString configsDir_;
    QJsonObject cacheData_;
};

} // namespace core

#endif // CORE_CONFIG_H
