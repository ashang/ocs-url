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

#include "package.h"

#include <QJsonObject>
#include <QMimeDatabase>
#include <QProcess>

namespace utils {

Package::Package(QObject *parent) : QObject(parent)
{}

bool Package::installProgram(const QString &path, const QString &targetPath)
{
    QString program = "install";
    QStringList arguments;
    arguments << "-m" << "755" << "-p" << path << targetPath;
    return execute(program, arguments);
}

bool Package::installFile(const QString &path, const QString &targetPath)
{
    QString program = "install";
    QStringList arguments;
    arguments << "-m" << "644" << "-p" << path << targetPath;
    return execute(program, arguments);
}

bool Package::installPlasmapkg(const QString &path, const QString &type)
{
    QString program = "plasmapkg2";
    QStringList arguments;
    arguments << "-t" << type << "-i" << path;
    return execute(program, arguments);
}

bool Package::uninstallPlasmapkg(const QString &path, const QString &type)
{
    QString program = "plasmapkg2";
    QStringList arguments;
    arguments << "-t" << type << "-r" << path;
    return execute(program, arguments);
}

bool Package::uncompressArchive(const QString &path, const QString &targetDir)
{
    QJsonObject archiveTypes;
    archiveTypes["application/x-tar"] = QString("tar");
    archiveTypes["application/x-gzip"] = QString("tar");
    archiveTypes["application/gzip"] = QString("tar");
    archiveTypes["application/x-bzip"] = QString("tar");
    archiveTypes["application/x-bzip2"] = QString("tar");
    archiveTypes["application/x-xz"] = QString("tar");
    archiveTypes["application/x-lzma"] = QString("tar");
    archiveTypes["application/x-lzip"] = QString("tar");
    archiveTypes["application/x-compressed-tar"] = QString("tar");
    archiveTypes["application/x-bzip-compressed-tar"] = QString("tar");
    archiveTypes["application/x-bzip2-compressed-tar"] = QString("tar");
    archiveTypes["application/x-xz-compressed-tar"] = QString("tar");
    archiveTypes["application/x-lzma-compressed-tar"] = QString("tar");
    archiveTypes["application/x-lzip-compressed-tar"] = QString("tar");
    archiveTypes["application/zip"] = QString("zip");
    archiveTypes["application/x-7z-compressed"] = QString("7z");
    archiveTypes["application/x-rar"] = QString("rar");
    archiveTypes["application/x-rar-compressed"] = QString("rar");

    QMimeDatabase mimeDb;
    QString mimeType = mimeDb.mimeTypeForFile(path).name();

    if (archiveTypes.contains(mimeType)) {
        QString archiveType = archiveTypes[mimeType].toString();

        QString program;
        QStringList arguments;

        if (archiveType == "tar") {
            program = "tar";
            arguments << "-xf" << path << "-C" << targetDir;
        }
        else if (archiveType == "zip") {
            program = "unzip";
            arguments << "-o" << path << "-d" << targetDir;
        }
        else if (archiveType == "7z") {
            program = "7z";
            arguments << "x" << path << "-o" + targetDir; // No space between -o and directory
        }
        else if (archiveType == "rar") {
            program = "unrar";
            arguments << "e" << path << targetDir;
        }

        return execute(program, arguments);
    }

    return false;
}

bool Package::execute(const QString &program, const QStringList &arguments)
{
    QProcess process;
    process.start(program, arguments);
    if (process.waitForFinished()) {
        process.waitForReadyRead();
        return true;
    }
    return false;
}

} // namespace utils
