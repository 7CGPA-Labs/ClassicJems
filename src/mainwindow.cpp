#include "mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_isGameRunning(false) {
    // Force a classic vertical smartphone aspect ratio on desktop window initialization
    setFixedSize(450, 800); 
    setWindowTitle("ClassicJems Emulator (Qt6)");

    m_stackedWidget = new QStackedWidget(this);
    m_libraryWidget = new GameLibraryWidget(this);
    m_gameCanvas = new QOpenGLWidget(this);
    m_gameCanvas->setStyleSheet("background-color: #111111;");

    m_stackedWidget->addWidget(m_libraryWidget);
    m_stackedWidget->addWidget(m_gameCanvas);

    setCentralWidget(m_stackedWidget);

    connect(m_libraryWidget, &GameLibraryWidget::gameSelected, this, &MainWindow::launchGame);
}

MainWindow::~MainWindow() {}

void MainWindow::launchGame(const QString &filePath, const QString &platform) {
    qDebug() << "Launching game ROM:" << filePath << " | Platform:" << platform;
    m_isGameRunning = true;
    m_stackedWidget->setCurrentIndex(1); // Switch to emulator canvas viewport
    m_gameCanvas->setFocus();
}

void MainWindow::returnToLibrary() {
    qDebug() << "Exiting emulation core, returning to launcher library.";
    m_isGameRunning = false;
    m_stackedWidget->setCurrentIndex(0); // Switch to ROM launchpad
    m_libraryWidget->setFocus();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (!m_isGameRunning) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    // Capture F1 key to dynamically exit back to the library menu
    if (event->key() == Qt::Key_F1) {
        returnToLibrary();
        return;
    }

    if (event->isAutoRepeat()) return; // Bypass native OS keyboard repeat delays

    int legacyKeyCode = mapQtKeyToLegacy(event->key());
    if (legacyKeyCode != -999) {
        sendKeyToEngine(legacyKeyCode, true);
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (!m_isGameRunning) {
        QMainWindow::keyReleaseEvent(event);
        return;
    }

    if (event->isAutoRepeat()) return; // Protect holding vectors during key lifts

    int legacyKeyCode = mapQtKeyToLegacy(event->key());
    if (legacyKeyCode != -999) {
        sendKeyToEngine(legacyKeyCode, false);
    } else {
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::sendKeyToEngine(int legacyKeyCode, bool isPressed) {
    // This logs the input vector before channeling directly into the engine threads
    qDebug() << "Key Event Map Index:" << legacyKeyCode << " | State Pressed:" << isPressed;
}

int MainWindow::mapQtKeyToLegacy(int key) {
    switch (key) {
        // W / A / S / D or Arrow Keys -> D-Pad (Up=1, Down=2, Left=3, Right=4)
        case Qt::Key_W:
        case Qt::Key_Up:
            return 1;
        case Qt::Key_S:
        case Qt::Key_Down:
            return 2;
        case Qt::Key_A:
        case Qt::Key_Left:
            return 3;
        case Qt::Key_D:
        case Qt::Key_Right:
            return 4;

        // Z / Enter -> Primary Select (KeyCode 5)
        case Qt::Key_Z:
        case Qt::Key_Return:
        case Qt::Key_Enter:
            return 5;

        // X / Escape -> Clear Input / Back (KeyCode 8)
        case Qt::Key_X:
        case Qt::Key_Escape:
            return 8;

        // Q / E -> LSK=-6, RSK=-7
        case Qt::Key_Q:
            return -6;
        case Qt::Key_E:
            return -7;

        // Standard Keyboard Row Digit 0-9
        case Qt::Key_0: return 0;
        case Qt::Key_1: return 1;
        case Qt::Key_2: return 2;
        case Qt::Key_3: return 3;
        case Qt::Key_4: return 4;
        case Qt::Key_5: return 5;
        case Qt::Key_6: return 6;
        case Qt::Key_7: return 7;
        case Qt::Key_8: return 8;
        case Qt::Key_9: return 9;

        default:
            return -999; // Unmapped key sentinel
    }
}