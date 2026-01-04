#include <QApplication>

#include "mainwindow.hpp"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow w(&app);
    return app.exec();
}
