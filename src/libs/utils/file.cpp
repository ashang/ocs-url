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

#include "file.h"

#include <QIODevice>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

namespace utils {

File::File(QObject *parent) : QObject(parent)
{}

QString File::rootPath()
{
    return QDir::rootPath();
}

QString File::tempPath()
{
    return QDir::tempPath();
}

QString File::homePath()
{
    return QDir::homePath();
}

QString File::genericDataPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
}

QString File::genericConfigPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
}

QString File::genericCachePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
}

QString File::kdehomePath()
{
    // KDE System Administration/Environment Variables
    // https://userbase.kde.org/KDE_System_Administration/Environment_Variables

    // KDE 4 maybe uses $KDEHOME
    QString path = QString::fromLocal8Bit(qgetenv("KDEHOME").constData());
    if (path.isEmpty()) {
        path = homePath() + "/.kde";
    }
    return path;
}

QFileInfoList File::readDir(const QString &path)
{
    QDir dir(path);
    dir.setFilter(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
    //dir.setSorting(QDir::DirsFirst | QDir::Name);
    return dir.entryInfoList();
}

bool File::makeDir(const QString &path)
{
    // This function will create all parent directories
    QDir dir(path);
    if (!dir.exists() && dir.mkpath(path)) {
        return true;
    }
    return false;
}

QString File::readText(const QString &path)
{
    QString data;
    QFile file(path);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        data = in.readAll();
        file.close();
    }
    return data;
}

bool File::writeText(const QString &path, const QString &data)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << data;
        file.close();
        return true;
    }
    return false;
}

QByteArray File::readBinary(const QString &path)
{
    QByteArray data;
    QFile file(path);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        data = file.readAll();
        file.close();
    }
    return data;
}

bool File::writeBinary(const QString &path, const QByteArray &data)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
        return true;
    }
    return false;
}

bool File::copy(const QString &path, const QString &targetPath)
{
    // This function will copy files recursively
    QFileInfo fileInfo(path);
    if (fileInfo.isFile()) {
        QFile file(path);
        if (file.copy(targetPath)) {
            return true;
        }
    }
    else if (fileInfo.isDir()) {
        QDir targetDir(targetPath);
        QString targetDirName = targetDir.dirName();
        targetDir.cdUp();
        if (targetDir.mkdir(targetDirName)) {
            QDir dir(path);
            dir.setFilter(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
            QStringList entries = dir.entryList();
            foreach (const QString &entry, entries) {
                if (!copy(path + "/" + entry, targetPath + "/" + entry)) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

bool File::move(const QString &path, const QString &targetPath)
{
    QFileInfo fileInfo(path);
    if (fileInfo.isFile()) {
        QFile file(path);
        if (file.rename(targetPath)) {
            return true;
        }
    }
    else if (fileInfo.isDir()) {
        QDir dir(path);
        if (dir.rename(path, targetPath)) {
            return true;
        }
    }
    return false;
}

bool File::remove(const QString &path)
{
    // This function will remove files recursively
    QFileInfo fileInfo(path);
    if (fileInfo.isFile()) {
        QFile file(path);
        if (file.remove()) {
            return true;
        }
    }
    else if (fileInfo.isDir()) {
        QDir dir(path);
        if (dir.removeRecursively()) {
            return true;
        }
    }
    return false;
}

} // namespace utils
