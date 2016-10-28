/**
 * A library for Qt app
 *
 * LICENSE: The GNU Lesser General Public License, version 3.0
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0  The GNU Lesser General Public License, version 3.0
 * @link        https://github.com/akiraohgaki/qt-libs
 */

#pragma once

#include <QObject>

class QEventLoop;
class QNetworkAccessManager;
class QNetworkReply;

namespace utils {

class Network : public QObject
{
    Q_OBJECT

public:
    explicit Network(const bool &async = true, QObject *parent = 0);
    ~Network();

    QNetworkReply *head(const QUrl &uri);
    QNetworkReply *get(const QUrl &uri);

signals:
    void finished(QNetworkReply *reply);
    void downloadProgress(const qint64 &received, const qint64 &total);

private:
    bool async_;
    QNetworkAccessManager *manager_;
    QEventLoop *eventLoop_;
};

} // namespace utils
