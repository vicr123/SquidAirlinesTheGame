//
// Created by victo on 5/08/2022.
//

#include "GameSession.h"
#include "Player.h"
#include <QSize>
#include <QRect>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>

struct GameSessionPrivate {
    QTimer* gameTimer;
    QTimer* speedTimer;

    Player* player;

    qreal x = 0;
    qreal speed = 1;
};

GameSession::GameSession(QObject* parent) : QObject(parent) {
    d = new GameSessionPrivate();

    d->player = new Player(this);

    d->gameTimer = new QTimer(this);
    d->gameTimer->setInterval(10);
    connect(d->gameTimer, &QTimer::timeout, this, &GameSession::tick);
    d->gameTimer->start();

    d->speedTimer = new QTimer(this);
    d->speedTimer->setInterval(10);
    connect(d->speedTimer, &QTimer::timeout, this, [this] {
        d->speed *= 1.00001;
    });
    d->speedTimer->start();
}

GameSession::~GameSession() {
    delete d;
}

void GameSession::drawScreen(QPainter* painter, QSize size) {
    QSize playSize(8, 6);
    playSize.scale(size, Qt::KeepAspectRatio);

    QRect playArea;
    playArea.setSize(playSize);
    playArea.moveCenter(QRect(QPoint(0, 0), size).center());

    painter->save();
    painter->setWindow(QRect(0, 0, 400, 300));
    painter->setViewport(playArea);
    for (int i = 0; i < 400; i++) {
        painter->drawLine(i, 0, i, static_cast<int>(i + d->x) % 400);
    }

    d->player->draw(painter);

    painter->restore();

//    painter->fillRect(playArea, Qt::yellow);
}

void GameSession::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Up:
            d->player->moveTarget(-10);
            break;
        case Qt::Key_Down:
            d->player->moveTarget(10);
            break;
    }
}

void GameSession::mouseMoveEvent(QMouseEvent* event) {
    d->player->setTarget(event->pos().y());
}

void GameSession::begin() {
    //QTimer::singleShot(3000, this, [this] {
    //    emit gameSessionEnded();
    //});
}

void GameSession::tick() {
    d->x += d->speed;
    emit requestPaint();
}
