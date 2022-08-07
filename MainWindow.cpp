//
// Created by victo on 5/08/2022.
//

#include "MainWindow.h"
#include "game/GameSession.h"
#include "menus/MainMenu.h"
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include "game/audioengine.h"

struct MainWindowPrivate {
    QList<AbstractMenu*> menusToDraw;

    AudioEngine* audio;

    QPointer<GameSession> session;
    MainMenu* mainMenu;
    bool playing = false;
};

MainWindow::MainWindow(QWidget* parent) : QOpenGLWidget(parent) {
    d = new MainWindowPrivate();
    this->resize(1024, 768);
    this->setMouseTracking(true);
    this->setMinimumSize(1024, 768);

    d->audio = new AudioEngine(this);

    auto surfaceFormat = this->format();
    surfaceFormat.setSamples(10);
    surfaceFormat.setDepthBufferSize(24);
    surfaceFormat.setStencilBufferSize(8);
    this->setFormat(surfaceFormat);

    this->prepareNewGameSession();

    d->mainMenu = new MainMenu(this);
    d->menusToDraw.append(d->mainMenu);

    connect(d->mainMenu, &MainMenu::requestPaint, this, [this] {
        this->update();
    });
    connect(d->mainMenu, &MainMenu::startGame, this, [this] {
        d->playing = true;
        d->session->begin();
        d->audio->startGame();
    });
}

MainWindow::~MainWindow() {
    delete d;
}

void MainWindow::paintEvent(QPaintEvent* e) {
    QOpenGLWidget::paintEvent(e);

    QLinearGradient sky(QPoint(0, this->height()), QPoint(0, 0));
    sky.setColorAt(0, QColor(0, 200, 255));
    sky.setColorAt(1, QColor(0, 100, 255));

    QPainter painter(this);
    painter.fillRect(QRect(0, 0, this->width(), this->height()), sky);

    if (d->session) {
        d->session->drawScreen(&painter, QSize(this->width(), this->height()));
    }

    for (auto menu : d->menusToDraw) {
        menu->drawMenu(&painter, QSize(this->width(), this->height()));
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    QWidget::keyPressEvent(event);

    if (d->playing) {
        d->session->keyPressEvent(event);
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

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (d->playing) {
        d->session->mouseMoveEvent(event);
    }
}

void MainWindow::prepareNewGameSession() {
    if (d->session) {
        d->session->deleteLater();
    }

    d->session = new GameSession();
    connect(d->session, &GameSession::requestPaint, this, [this] {
        this->update();
    });
    connect(d->session, &GameSession::gameSessionEnded, this, [this] {
        d->playing = false;
        d->mainMenu->showAgain();
        d->audio->endGame();
        this->prepareNewGameSession();
    });
}
