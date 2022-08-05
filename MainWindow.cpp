//
// Created by victo on 5/08/2022.
//

#include "MainWindow.h"
#include "menus/MainMenu.h"
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>

struct MainWindowPrivate {
    QList<AbstractMenu*> menusToDraw;

    MainMenu* mainMenu;
    bool playing = false;
};

MainWindow::MainWindow(QWidget *parent) : QOpenGLWidget(parent) {
    d = new MainWindowPrivate();
    this->resize(1024, 768);
    this->setMinimumSize(1024, 768);

    auto surfaceFormat = this->format();
    surfaceFormat.setSamples(10);
    surfaceFormat.setDepthBufferSize(24);
    surfaceFormat.setStencilBufferSize(8);
    this->setFormat(surfaceFormat);

    d->mainMenu = new MainMenu(this);
    d->menusToDraw.append(d->mainMenu);

    connect(d->mainMenu, &MainMenu::requestPaint, this, [this] {
        this->update();
    });
    connect(d->mainMenu, &MainMenu::startGame, this, [this] {
        d->playing = true;
        QTimer::singleShot(3000, this, [this] {
            d->playing = false;
          d->mainMenu->showAgain();
        });
    });
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

    if (d->playing) {

    } else {
        switch (event->key()) {
            case Qt::Key_Left:
            case Qt::Key_Up:
                d->menusToDraw.last()->moveToPrev();
                break;
            case Qt::Key_Right:
            case Qt::Key_Down:
                d->menusToDraw.last()->moveToNext();
                break;
            case Qt::Key_Return:
                d->menusToDraw.last()->activate();
        }
//        d->menusToDraw.last()->
    }
}
