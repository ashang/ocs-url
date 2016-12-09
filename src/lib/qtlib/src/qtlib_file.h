/**
 * qtlib
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0
 * @link        https://github.com/akiraohgaki/qtlib
 */

#pragma once

#include <QObject>

namespace qtlib {

class File : public QObject
{
    Q_OBJECT

public:
    explicit File(const QString &path = "", QObject *parent = 0);

    File(const File &other, QObject *parent = 0);
    File &operator =(const File &other);

    QString path() const;
    void setPath(const QString &path);

    bool exists();
    QByteArray readData();
    bool writeData(const QByteArray &data);
    QString readText();
    bool writeText(const QString &data);
    bool copy(const QString &newPath);
    bool move(const QString &newPath);
    bool remove();

private:
    QString path_;
};

} // namespace qtlib
