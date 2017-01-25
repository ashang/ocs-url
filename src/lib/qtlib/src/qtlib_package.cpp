/**
 * qtlib
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0
 * @link        https://github.com/akiraohgaki/qtlib
 */

#include "qtlib_package.h"

#ifdef QTLIB_UNIX
#include <QJsonObject>
#include <QMimeDatabase>
#include <QProcess>
#endif

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#endif

namespace qtlib {

Package::Package(const QString &path, QObject *parent)
    : QObject(parent), path_(path)
{}

Package::Package(const Package &other, QObject *parent)
    : QObject(parent)
{
    setPath(other.path());
}

Package &Package::operator =(const Package &other)
{
    setPath(other.path());
    return *this;
}

QString Package::path() const
{
    return path_;
}

void Package::setPath(const QString &path)
{
    path_ = path;
}

#ifdef QTLIB_UNIX
bool Package::installAsProgram(const QString &newPath)
{
    QStringList arguments;
    arguments << "-m" << "755" << "-p" << path() << newPath;
    return execute("install", arguments);
}

bool Package::installAsFile(const QString &newPath)
{
    QStringList arguments;
    arguments << "-m" << "644" << "-p" << path() << newPath;
    return execute("install", arguments);
}

bool Package::installAsArchive(const QString &destinationDirPath)
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

    QString mimeType = QMimeDatabase().mimeTypeForFile(path()).name();

    if (archiveTypes.contains(mimeType)) {
        QString archiveType = archiveTypes[mimeType].toString();
        QString program;
        QStringList arguments;
        if (archiveType == "tar") {
            program = "tar";
            arguments << "-xf" << path() << "-C" << destinationDirPath;
        }
        else if (archiveType == "zip") {
            program = "unzip";
            arguments << "-o" << path() << "-d" << destinationDirPath;
        }
        else if (archiveType == "7z") {
            program = "7z";
            arguments << "x" << path() << "-o" + destinationDirPath; // No space between -o and directory
        }
        else if (archiveType == "rar") {
            program = "unrar";
            arguments << "e" << path() << destinationDirPath;
        }
        return execute(program, arguments);
    }
    return false;
}

bool Package::installAsPlasmapkg(const QString &type)
{
    QStringList arguments;
    arguments << "-t" << type << "-i" << path();
    return execute("plasmapkg2", arguments);
}

bool Package::uninstallAsPlasmapkg(const QString &type)
{
    QStringList arguments;
    arguments << "-t" << type << "-r" << path();
    return execute("plasmapkg2", arguments);
}
#endif

#ifdef Q_OS_ANDROID
bool Package::installAsApk()
{
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    if (activity.isValid()) {
        QString filePath = path();
        if (filePath.startsWith("file://", Qt::CaseInsensitive)) {
            filePath.replace("file://localhost", "", Qt::CaseInsensitive);
            filePath.replace("file://", "", Qt::CaseInsensitive);
        }

        QAndroidJniObject fileUri = QAndroidJniObject::fromString("file://" + filePath);
        QAndroidJniObject uri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", fileUri.object());
        QAndroidJniObject mimeType = QAndroidJniObject::fromString("application/vnd.android.package-archive");

        QAndroidJniObject ACTION_VIEW = QAndroidJniObject::getStaticObjectField("android/content/Intent", "ACTION_VIEW", "Ljava/lang/String");
        QAndroidJniObject FLAG_ACTIVITY_NEW_TASK = QAndroidJniObject::getStaticObjectField("android/content/Intent", "FLAG_ACTIVITY_NEW_TASK", "Ljava/lang/Integer");

        QAndroidJniObject intent("android/content/Intent", "(Ljava/lang/String;)V", ACTION_VIEW.object());
        intent = intent.callObjectMethod("setDataAndType", "(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;", uri.object(), mimeType.object());
        intent = intent.callObjectMethod("setFlags", "(I)Landroid/content/Intent;", FLAG_ACTIVITY_NEW_TASK.object());

        activity.callMethod<void>("startActivity", "(Landroid/content/Intent;)V", intent.object());
        return true;
    }
    return false;
}
#endif

#ifdef QTLIB_UNIX
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
#endif

} // namespace qtlib
