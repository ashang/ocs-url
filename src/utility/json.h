#ifndef UTILITY_JSON_H
#define UTILITY_JSON_H

#include <QObject>

namespace Utility {

class Json : public QObject
{
    Q_OBJECT

public:
    explicit Json(QObject *parent = 0);

    static bool isValid(const QString &json);
    static QJsonObject convertStrToObj(const QString &json);
    static QString convertObjToStr(const QJsonObject &jsonObj);
};

} // namespace Utility

#endif // UTILITY_JSON_H
