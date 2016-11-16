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

namespace qtlibs {

class Package : public QObject
{
    Q_OBJECT

public:
    explicit Package(const QString &path = "", QObject *parent = 0);

    Package(const Package &other, QObject *parent = 0);
    Package &operator =(const Package &other);

    QString path() const;
    void setPath(const QString &path);

#ifdef QTLIBS_UNIX
    bool installAsProgram(const QString &newPath);
    bool installAsFile(const QString &newPath);
    bool installAsArchive(const QString &destinationDirPath);
    bool installAsPlasmapkg(const QString &type = "plasmoid");
    bool uninstallAsPlasmapkg(const QString &type = "plasmoid");
#endif

#ifdef Q_OS_ANDROID
    bool installAsApk();
#endif

private:
#ifdef QTLIBS_UNIX
    bool execute(const QString &program, const QStringList &arguments);
#endif

    QString path_;
};

} // namespace qtlibs
