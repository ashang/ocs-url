#ifndef UTILITY_PACKAGE_H
#define UTILITY_PACKAGE_H

#include <QObject>

namespace Utility {

class Package : public QObject
{
    Q_OBJECT

public:
    explicit Package(QObject *parent = 0);

    static bool uncompressArchive(const QString &path, const QString &targetDir);
    static bool installPlasmapkg(const QString &path, const QString &type = "plasmoid");
    static bool uninstallPlasmapkg(const QString &path, const QString &type = "plasmoid");
    static bool installProgram(const QString &path, const QString &targetPath);
};

} // namespace Utility

#endif // UTILITY_PACKAGE_H
