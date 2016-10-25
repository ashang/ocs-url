/**
 * A library for Qt app
 *
 * LICENSE: The GNU Lesser General Public License, version 3.0
 *
 * @author      Akira Ohgaki <akiraohgaki@gmail.com>
 * @copyright   Akira Ohgaki
 * @license     https://opensource.org/licenses/LGPL-3.0  The GNU Lesser General Public License, version 3.0
 * @link        https://github.com/akiraohgaki/qt-libs
 */

#include "android.h"

#include <QAndroidJniObject>

namespace utils {

Android::Android(QObject *parent) : QObject(parent)
{}

bool Android::openApk(const QString &uri)
{
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    if (activity.isValid()) {
        QAndroidJniObject fileUri = QAndroidJniObject::fromString(uri);
        QAndroidJniObject parsedUri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", fileUri.object());
        QAndroidJniObject mimeType = QAndroidJniObject::fromString("application/vnd.android.package-archive");
        QAndroidJniObject activityKind = QAndroidJniObject::fromString("android.intent.action.VIEW");
        QAndroidJniObject intent("android/content/Intent", "(Ljava/lang/String;)V", activityKind.object());
        intent = intent.callObjectMethod("setDataAndType", "(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;", parsedUri.object(), mimeType.object());
        intent = intent.callObjectMethod("setFlags", "(I)Landroid/content/Intent;", 0x10000000); // 0x10000000 = FLAG_ACTIVITY_NEW_TASK
        activity.callObjectMethod("startActivity", "(Landroid/content/Intent;)V", intent.object());
        return true;
    }
    return false;
}

} // namespace utils
