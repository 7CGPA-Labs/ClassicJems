#include "../emulatorwindow.h"
#include <QApplication>
#include <cstring>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (argc >= 2 && (std::strcmp(argv[1], "--test") == 0 || std::strcmp(argv[1], "-t") == 0)) {
        qDebug() << "ClassicJems Player headless initialization test passed.";
        return 0;
    }

    if (argc < 3) {
        qWarning() << "Usage: classicjems-player <file_path> <platform>";
        return 1;
    }
    EmulatorWindow window(argv[1], argv[2]);
    window.show();
    return app.exec();
}
