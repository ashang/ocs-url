#include <QObject>
#include <QCoreApplication>
#include <QDebug>

#include "qtlib_file.h"
#include "qtlib_dir.h"
#include "qtlib_json.h"
#include "qtlib_config.h"
#include "qtlib_networkresource.h"
#include "qtlib_ocsapi.h"
#include "qtlib_package.h"

class Test : public QObject
{
public:
    Test() {}
    virtual ~Test() {}

    void start()
    {
        qDebug() << "Start";

        qtlib::NetworkResource *resource = new qtlib::NetworkResource(
                    "LGPL-3.0",
                    QUrl("https://api.opensource.org/license/LGPL-3.0"),
                    false,
                    this);
        connect(resource, &qtlib::NetworkResource::downloadProgress, this, &Test::downloadProgress);
        QJsonObject result = qtlib::Json(resource->get()->readData()).toObject();

        qDebug() << resource->id() << ":" << result["name"].toString();

        connect(resource, &qtlib::NetworkResource::finished, this, &Test::finished);
        resource->setId(result["name"].toString());
        resource->setUrl(QUrl(result["text"].toArray()[0].toObject()["url"].toString()));
        resource->setAsync(true);
        resource->get();
    }

public slots:
    void finished(qtlib::NetworkResource *resource)
    {
        QString path = qtlib::Dir::tempPath() + "/" + resource->url().fileName();
        resource->saveData(path);

        qDebug() << "Downloaded" << resource->id() << ":" << path;
        qDebug() << "Finished";

        resource->deleteLater();
        QCoreApplication::exit();
    }

    void downloadProgress(QString id, qint64 bytesReceived, qint64 bytesTotal)
    {
        qDebug() << "Progress" << id << ":" << bytesReceived << "/" << bytesTotal;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Test test;
    test.start();

    return app.exec();
}
