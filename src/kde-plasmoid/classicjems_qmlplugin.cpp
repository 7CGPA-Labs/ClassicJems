#include "classicjems_qmlplugin.h"
#include "../emulatorwindow.h"
#include <qqml.h>
#include <QObject>
#include <QDebug>

class ClassicJemsLauncher : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void launch(const QString &filePath, const QString &platform) {
        qDebug() << "QML Request to launch ROM:" << filePath << "on platform:" << platform;
        EmulatorWindow *window = new EmulatorWindow(filePath, platform);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();
        window->raise();
        window->activateWindow();
    }
};

void ClassicJemsQmlPlugin::registerTypes(const char *uri) {
    qmlRegisterType<ClassicJemsLauncher>(uri, 1, 0, "ClassicJemsLauncher");
}

#include "classicjems_qmlplugin.moc"
