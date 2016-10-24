#ifndef UTILS_JSON_H
#define UTILS_JSON_H

#include <QObject>

namespace utils {

class Json : public QObject
{
    Q_OBJECT

public:
    explicit Json(QObject *parent = 0);

    static QString convertObjToStr(const QJsonObject &jsonObj);
    static QJsonObject convertStrToObj(const QString &json);
    static bool isValid(const QString &json);
};

} // namespace utils

#endif // UTILS_JSON_H
