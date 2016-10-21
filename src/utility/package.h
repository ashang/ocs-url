#ifndef UTILITY_PACKAGE_H
#define UTILITY_PACKAGE_H

#include <QObject>

namespace Utility {

class Package : public QObject
{
    Q_OBJECT

public:
    explicit Package(QObject *parent = 0);

    static bool installProgram(const QString &path, const QString &targetPath);
    static bool installFile(const QString &path, const QString &targetPath);
    static bool installPlasmapkg(const QString &path, const QString &type = "plasmoid");
    static bool uninstallPlasmapkg(const QString &path, const QString &type = "plasmoid");
    static bool uncompressArchive(const QString &path, const QString &targetDir);

private:
    static bool process_(const QString &program, const QStringList &arguments);
};

} // namespace Utility

#endif // UTILITY_PACKAGE_H
