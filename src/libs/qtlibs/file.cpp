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

#include "file.h"

#include <QIODevice>
#include <QTextStream>
#include <QFile>

namespace qtlibs {

File::File(const QString &path, QObject *parent)
    : QObject(parent), path_(path)
{}

File::File(const File &other)
{
    this->setParent(other.parent());
    setPath(other.path());
}

File &File::operator =(const File &other)
{
    this->setParent(other.parent());
    setPath(other.path());
    return *this;
}

QString File::path() const
{
    return path_;
}

void File::setPath(const QString &path)
{
    path_ = path;
}

bool File::exists()
{
    QFile file(path());
    return file.exists();
}

QByteArray File::readData()
{
    QByteArray data;
    QFile file(path());
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        data = file.readAll();
        file.close();
    }
    return data;
}

bool File::writeData(const QByteArray &data)
{
    QFile file(path());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
        return true;
    }
    return false;
}

QString File::readText()
{
    QString data;
    QFile file(path());
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        data = in.readAll();
        file.close();
    }
    return data;
}

bool File::writeText(const QString &data)
{
    QFile file(path());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << data;
        file.close();
        return true;
    }
    return false;
}

bool File::copy(const QString &newPath)
{
    QFile file(path());
    return file.copy(newPath);
}

bool File::move(const QString &newPath)
{
    QFile file(path());
    return file.rename(newPath);
}

bool File::remove()
{
    QFile file(path());
    return file.remove();
}

} // namespace qtlibs
