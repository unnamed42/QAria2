#include <QApplication>

#include "mainwindow.hpp"
#include "core.hpp"

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};

    Core::setup();

    MainWindow w;
    w.show();
    return app.exec();
}
