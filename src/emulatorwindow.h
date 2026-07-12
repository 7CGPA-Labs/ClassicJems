#pragma once

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QDebug>

class EmulatorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit EmulatorWindow(const QString &filePath, const QString &platform, QWidget *parent = nullptr);
    ~EmulatorWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QOpenGLWidget *m_gameCanvas;
    bool m_isGameRunning;
    QString m_filePath;
    QString m_platform;

    void sendKeyToEngine(int legacyKeyCode, bool isPressed);
    static int mapQtKeyToLegacy(int key);
};
