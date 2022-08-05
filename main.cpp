#include <QApplication>
#include <QPushButton>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Squid Airlines The Game");

    MainWindow mainwin;
    mainwin.show();

    return QApplication::exec();
}
