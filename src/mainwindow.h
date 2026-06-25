#pragma once

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QStackedWidget>
#include <QKeyEvent>
#include <QDebug>
#include "gamelibrarywidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Capture low-level desktop keyboard intercepts smoothly
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void launchGame(const QString &filePath, const QString &platform);
    void returnToLibrary();

private:
    QStackedWidget *m_stackedWidget;
    GameLibraryWidget *m_libraryWidget;
    QOpenGLWidget *m_gameCanvas;
    bool m_isGameRunning;

    void sendKeyToEngine(int legacyKeyCode, bool isPressed);
    static int mapQtKeyToLegacy(int key);
};