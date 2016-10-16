#include <QJsonObject>
#include <QMimeDatabase>
#include <QProcess>

#include "package.h"

namespace Utility {

Package::Package(QObject *parent) : QObject(parent)
{}

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

        QProcess process;
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

        process.start(program, arguments);

        if (process.waitForFinished()) {
            process.waitForReadyRead();
            return true;
        }
    }

    return false;
}

bool Package::installPlasmapkg(const QString &path, const QString &type)
{
    QProcess process;
    QString program = "plasmapkg2"; // Use plasmapkg2 for now
    QStringList arguments;
    arguments << "-t" << type << "-i" << path;

    process.start(program, arguments);

    if (process.waitForFinished()) {
        return true;
    }

    return false;
}

bool Package::uninstallPlasmapkg(const QString &path, const QString &type)
{
    QProcess process;
    QString program = "plasmapkg2"; // Use plasmapkg2 for now
    QStringList arguments;
    arguments << "-t" << type << "-r" << path;

    process.start(program, arguments);

    if (process.waitForFinished()) {
        return true;
    }

    return false;
}

bool Package::installProgram(const QString &path, const QString &targetPath)
{
    QProcess process;
    QString program = "install";
    QStringList arguments;
    arguments << "-D" << "-m" << "755" << path << targetPath;

    process.start(program, arguments);

    if (process.waitForFinished()) {
        return true;
    }

    return false;
}

} // namespace Utility
