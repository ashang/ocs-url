#pragma once

#include <QObject>
#include <QJsonObject>

#include "qtlib_config.h"

class ConfigHandler : public QObject
{
    Q_OBJECT

public:
    explicit ConfigHandler(QObject *parent = 0);

public slots:
    QJsonObject getAppConfigApplication();
    QJsonObject getAppConfigInstallTypes();

private:
    QString convertPathString(const QString &path);

    qtlib::Config appConfig_;
    QJsonObject appConfigApplication_;
    QJsonObject appConfigInstallTypes_;
};
