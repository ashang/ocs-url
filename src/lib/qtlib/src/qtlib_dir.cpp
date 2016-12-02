/**
 * A library for Qt app
 *
 * LICENSE: The GNU Lesser General Public License, version 3.0
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0  The GNU Lesser General Public License, version 3.0
 * @link        https://github.com/akiraohgaki/qtlib
 */

#include "qtlib_dir.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

namespace qtlib {

Dir::Dir(const QString &path, QObject *parent)
    : QObject(parent), path_(path)
{}

Dir::Dir(const Dir &other, QObject *parent)
    : QObject(parent)
{
    setPath(other.path());
}

Dir &Dir::operator =(const Dir &other)
{
    setPath(other.path());
    return *this;
}

QString Dir::path() const
{
    return path_;
}

void Dir::setPath(const QString &path)
{
    path_ = path;
}

bool Dir::exists()
{
    QDir dir(path());
    return dir.exists();
}

QFileInfoList Dir::list()
{
    QDir dir(path());
    dir.setFilter(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
    //dir.setSorting(QDir::DirsFirst | QDir::Name);
    return dir.entryInfoList();
}

bool Dir::make()
{
    // This function will create all parent directories
    QDir dir(path());
    if (!dir.exists() && dir.mkpath(path())) {
        return true;
    }
    return false;
}

bool Dir::copy(const QString &newPath)
{
    // This function will copy files recursively
    return copyRecursively(path(), newPath);
}

bool Dir::move(const QString &newPath)
{
    QDir dir(path());
    return dir.rename(path(), newPath);
}

bool Dir::remove()
{
    // This function will remove files recursively
    QDir dir(path());
    return dir.removeRecursively();
}

QString Dir::rootPath()
{
    return QDir::rootPath();
}

QString Dir::tempPath()
{
    return QDir::tempPath();
}

QString Dir::homePath()
{
    return QDir::homePath();
}

QString Dir::genericDataPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
}

QString Dir::genericConfigPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
}

QString Dir::genericCachePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
}

QString Dir::kdehomePath()
{
    // KDE System Administration/Environment Variables
    // https://userbase.kde.org/KDE_System_Administration/Environment_Variables

    // KDE 4 maybe uses $KDEHOME
    QString kdehomePath = QString::fromLocal8Bit(qgetenv("KDEHOME").constData());
    if (kdehomePath.isEmpty()) {
        kdehomePath = homePath() + "/.kde";
    }
    return kdehomePath;
}

bool Dir::copyRecursively(const QString &srcPath, const QString &newPath)
{
    QFileInfo fileInfo(srcPath);
    if (fileInfo.isFile()) {
        QFile file(srcPath);
        if (file.copy(newPath)) {
            return true;
        }
    }
    else if (fileInfo.isDir()) {
        QDir newDir(newPath);
        QString newDirName = newDir.dirName();
        newDir.cdUp();
        if (newDir.mkdir(newDirName)) {
            QDir dir(srcPath);
            dir.setFilter(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
            QStringList entries = dir.entryList();
            foreach (const QString &entry, entries) {
                if (!copyRecursively(srcPath + "/" + entry, newPath + "/" + entry)) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

} // namespace qtlib
