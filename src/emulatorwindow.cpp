#include "emulatorwindow.h"

EmulatorWindow::EmulatorWindow(const QString &filePath, const QString &platform, QWidget *parent)
    : QMainWindow(parent),
      m_isGameRunning(true),
      m_filePath(filePath),
      m_platform(platform)
{
    // Force a classic vertical smartphone aspect ratio on desktop window initialization
    setFixedSize(450, 800); 
    setWindowTitle(QString("ClassicJems - Play: %1 (%2)").arg(filePath).arg(platform));

    // Initialize the OpenGL canvas container where the core engines will render frames
    m_gameCanvas = new QOpenGLWidget(this);
    m_gameCanvas->setStyleSheet("background-color: #111111;");

    setCentralWidget(m_gameCanvas);
    m_gameCanvas->setFocus();

    qDebug() << "Initializing emulator core for J2ME/Symbian ROM:" << m_filePath << "Platform:" << m_platform;
}

EmulatorWindow::~EmulatorWindow() {
    qDebug() << "Terminating emulator core session.";
}

void EmulatorWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_F1) {
        close();
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

void EmulatorWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return; // Protect holding vectors during key lifts

    int legacyKeyCode = mapQtKeyToLegacy(event->key());
    if (legacyKeyCode != -999) {
        sendKeyToEngine(legacyKeyCode, false);
    } else {
        QMainWindow::keyReleaseEvent(event);
    }
}

void EmulatorWindow::sendKeyToEngine(int legacyKeyCode, bool isPressed) {
    // This logs the input vector before channeling directly into the engine threads
    qDebug() << "Key Event Map Index:" << legacyKeyCode << " | State Pressed:" << isPressed;
}

int EmulatorWindow::mapQtKeyToLegacy(int key) {
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
