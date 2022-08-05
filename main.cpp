#include <QApplication>
#include <QPushButton>

#include "MainWindow.h"
#include <QFontDatabase>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Squid Airlines The Game");

    auto id = QFontDatabase::addApplicationFont(":/IBMPlexSans-Regular.ttf");
    auto family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family, 15);
    a.setFont(font);

    MainWindow mainwin;
    mainwin.show();

    return QApplication::exec();
}
