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

#pragma once

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>

namespace qtlibs {

class OcsApi : public QObject
{
    Q_OBJECT

public:
    explicit OcsApi(const QString &id = "", const QUrl &baseUrl = QUrl(), const QString &userName = "", const QString &password = "", QObject *parent = 0);

    OcsApi(const OcsApi &other, QObject *parent = 0);
    OcsApi &operator =(const OcsApi &other);

    QString id() const;
    void setId(const QString &id);
    QUrl baseUrl() const;
    void setBaseUrl(const QUrl &baseUrl);
    QString userName() const;
    void setUserName(const QString &userName);
    QString password() const;
    void setPassword(const QString &password);

    QJsonObject getConfig();
    QJsonObject checkPerson();
    QJsonObject getPersonDataSet(const QUrlQuery &query = QUrlQuery());
    QJsonObject getPersonData(const QString &personId);
    QJsonObject getPersonSelf();
    QJsonObject getContentCategories();
    QJsonObject getContentDataSet(const QUrlQuery &query = QUrlQuery());
    QJsonObject getContentData(const QString &contentId);
    QJsonObject getContentDownload(const QString &contentId, const QString &itemId);

    static QJsonArray getProviderFile(const QUrl &url);

private:
    QString id_;
    QUrl baseUrl_;
    QString userName_;
    QString password_;
};

} // namespace qtlibs
