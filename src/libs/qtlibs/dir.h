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
#include <QFileInfoList>

namespace qtlibs {

class Dir : public QObject
{
    Q_OBJECT

public:
    explicit Dir(const QString &path = QString(), QObject *parent = 0);

    Dir(const Dir &other);
    Dir &operator =(const Dir &other);

    QString path() const;
    void setPath(const QString &path);

    bool exists();
    QFileInfoList list();
    bool make();
    bool copy(const QString &newPath);
    bool move(const QString &newPath);
    bool remove();

    static QString rootPath();
    static QString tempPath();
    static QString homePath();
    static QString genericDataPath();
    static QString genericConfigPath();
    static QString genericCachePath();
    static QString kdehomePath();

private:
    bool copyRecursively(const QString &srcPath, const QString &newPath);

    QString path_;
};

} // namespace qtlibs
