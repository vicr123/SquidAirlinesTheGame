//
// Created by victo on 5/08/2022.
//

#include "MainWindow.h"
#include "menus/MainMenu.h"
#include <QPainter>

struct MainWindowPrivate {
    AbstractMenu* activeMenu;
};

MainWindow::MainWindow(QWidget *parent) : QOpenGLWidget(parent) {
    d = new MainWindowPrivate();
    this->resize(800, 600);
    this->setMinimumSize(800, 600);

    d->activeMenu = new MainMenu(this);
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

    if (d->activeMenu) {
        d->activeMenu->drawMenu(&painter, QSize(this->width(), this->height()));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
}
