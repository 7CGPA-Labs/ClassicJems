#include "../emulatorwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    if (argc < 3) {
        qWarning() << "Usage: classicjems-player <file_path> <platform>";
        return 1;
    }
    EmulatorWindow window(argv[1], argv[2]);
    window.show();
    return app.exec();
}
