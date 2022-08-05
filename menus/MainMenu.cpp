//
// Created by victo on 5/08/2022.
//

#include "MainMenu.h"

#include <QPainter>
#include <QSize>

struct MainMenuPrivate {

};

MainMenu::MainMenu(QObject *parent) : AbstractMenu(parent) {
    d = new MainMenuPrivate();
}

MainMenu::~MainMenu() {
    delete d;
}

void MainMenu::drawMenu(QPainter *painter, QSize size) {
    painter->drawText(0, 100, "Menu goes here");
}
