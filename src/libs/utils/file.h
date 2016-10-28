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

class QFileInfo;
typedef QList<QFileInfo> QFileInfoList;

namespace utils {

class File : public QObject
{
    Q_OBJECT

public:
    explicit File(QObject *parent = 0);

    static QString rootPath();
    static QString tempPath();
    static QString homePath();
    static QString genericDataPath();
    static QString genericConfigPath();
    static QString genericCachePath();
    static QString kdehomePath();
    static QFileInfoList readDir(const QString &path);
    static bool makeDir(const QString &path);
    static QString readText(const QString &path);
    static bool writeText(const QString &path, const QString &data);
    static QByteArray readBinary(const QString &path);
    static bool writeBinary(const QString &path, const QByteArray &data);
    static bool copy(const QString &path, const QString &targetPath);
    static bool move(const QString &path, const QString &targetPath);
    static bool remove(const QString &path);
};

} // namespace utils
