//
// Created by victo on 5/08/2022.
//

#include "MainWindow.h"
#include "menus/MainMenu.h"
#include <QPainter>

struct MainWindowPrivate {
    QList<AbstractMenu*> menusToDraw;

    MainMenu* mainMenu;
};

MainWindow::MainWindow(QWidget *parent) : QOpenGLWidget(parent) {
    d = new MainWindowPrivate();
    this->resize(1024, 768);
    this->setMinimumSize(1024, 768);

    d->mainMenu = new MainMenu(this);
    d->menusToDraw.append(d->mainMenu);
}

MainWindow::~MainWindow() {
    delete d;
}

void MainWindow::paintEvent(QPaintEvent *e) {
    QOpenGLWidget::paintEvent(e);

    QLinearGradient sky(QPoint(0, this->height()), QPoint(0, 0));
    sky.setColorAt(0, QColor(0, 200, 255));
    sky.setColorAt(1, QColor(0, 100, 255));

    QPainter painter(this);
    painter.fillRect(QRect(0, 0, this->width(), this->height()), sky);

    for (auto menu : d->menusToDraw) {
        menu->drawMenu(&painter, QSize(this->width(), this->height()));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
}
