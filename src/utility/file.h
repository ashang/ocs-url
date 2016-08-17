#ifndef UTILITY_FILE_H
#define UTILITY_FILE_H

#include <QObject>

class QFileInfo;
typedef QList<QFileInfo> QFileInfoList;

namespace Utility {

class File : public QObject
{
    Q_OBJECT

public:
    explicit File(QObject *parent = 0);

    static QString rootPath();
    static QString tempPath();
    static QString homePath();
    static QString xdgDataHomePath();
    static QString xdgConfigHomePath();
    static QString xdgCacheHomePath();
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

} // namespace Utility

#endif // UTILITY_FILE_H
