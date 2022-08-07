//
// Created by victo on 5/08/2022.
//

#include "GameSession.h"
#include "Player.h"
#include "objects/buildingobject.h"
#include "objects/tankobject.h"
#include "objects/thunderstormobject.h".h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QRandomGenerator64>
#include <QRect>
#include <QSize>
#include <QTimer>
#include <QVariantAnimation>
#include <QSvgRenderer>

Q_DECLARE_METATYPE(QRandomGenerator64)

struct GameSessionPrivate {
    QRandomGenerator64 random;
    QTimer* gameTimer;
    QTimer* speedTimer;

    Player* player;

    QTransform lastTransform;

    QList<GameObjectPtr> gameObjects;

    qreal x = 0;
    qreal speed = 1;

    qreal nextGen = -1;

    qreal hudOpacity = 0;
    bool gameStarted = false;

    qreal gameOverScrimOpacity = 0;

    QSvgRenderer fuel;
    QSvgRenderer health;
    QSvgRenderer healthLost;

    static QList<QMetaObject> gameObjectTypes;
};

QList<QMetaObject> GameSessionPrivate::gameObjectTypes = {
    BuildingObject::staticMetaObject, BuildingObject::staticMetaObject, BuildingObject::staticMetaObject, BuildingObject::staticMetaObject, BuildingObject::staticMetaObject, TankObject::staticMetaObject, ThunderstormObject::staticMetaObject, ThunderstormObject::staticMetaObject, ThunderstormObject::staticMetaObject
};

GameSession::GameSession(QObject* parent) :
    QObject(parent) {
    d = new GameSessionPrivate();

    d->player = new Player(&d->random, this);
    connect(d->player, &Player::triggerGameOver, this, &GameSession::triggerGameOver);

    d->gameTimer = new QTimer(this);
    d->gameTimer->setInterval(10);
    connect(d->gameTimer, &QTimer::timeout, this, &GameSession::tick);
    d->gameTimer->start();

    d->speedTimer = new QTimer(this);
    d->speedTimer->setInterval(10);
    connect(d->speedTimer, &QTimer::timeout, this, [this] {
        d->speed *= 1.00005;
    });

    d->fuel.load(QStringLiteral(":/fuel.svg"));
    d->health.load(QStringLiteral(":/health.svg"));
    d->healthLost.load(QStringLiteral(":/health-lost.svg"));
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

    d->lastTransform = painter->combinedTransform();

    //    for (int i = 0; i < 400; i++) {
    //        painter->drawLine(i, 0, i, static_cast<int>(i + d->x) % 400);
    //    }

    for (auto gameObject : d->gameObjects) {
        gameObject->draw(painter, d->x);
    }

    d->player->draw(painter);

    painter->restore();

    painter->save();
    painter->setOpacity(d->hudOpacity);

    // Draw the HUD
    QRectF fuelIconRect;
    fuelIconRect.setWidth(25);
    fuelIconRect.setHeight(25);
    fuelIconRect.moveTopLeft(playArea.topLeft() + QPoint(50, 50));

    if (d->player->fuel() < 0.2) {
        if ((static_cast<int>(d->x) / 50) % 2) {
            d->fuel.render(painter, fuelIconRect);
        }
    } else {
        d->fuel.render(painter, fuelIconRect);
    }

    QRectF fuelRect;
    fuelRect.setWidth(300);
    fuelRect.setHeight(25);
    fuelRect.moveTopLeft(fuelIconRect.topRight() + QPoint(25, 0));

    QRectF filledFuelRect = fuelRect;
    filledFuelRect.setWidth(fuelRect.width() * qMax(0.0, d->player->fuel()));
    painter->fillRect(filledFuelRect, Qt::red);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::transparent);
    painter->drawRect(fuelRect);

    QRectF health;
    health.setHeight(25);
    health.setWidth(25);
    health.moveLeft(fuelRect.left());
    health.moveTop(fuelRect.bottom() + 10);

    for (auto i = 1; i <= 5; i++) {
        if (d->player->health() >= i) {
            d->health.render(painter, health);
        } else {
            d->healthLost.render(painter, health);
        }

        health.moveLeft(health.right() + 10);
    }

    painter->restore();

    painter->save();
    painter->setOpacity(d->gameOverScrimOpacity);
    painter->fillRect(QRect(QPoint(0, 0), size), Qt::white);
    painter->restore();
}

QPointF GameSession::toGameCoordinates(QPointF windowCoordinates) {
    return d->lastTransform.inverted().map(windowCoordinates);
}

void GameSession::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Up:
            d->player->moveTarget(-10);
            break;
        case Qt::Key_Down:
            d->player->moveTarget(10);
            break;
        case Qt::Key_Escape:
            d->gameTimer->stop();
            d->speedTimer->stop();
            emit paused();
            break;
    }
}

void GameSession::mouseMoveEvent(QMouseEvent* event) {
    d->player->setTarget(this->toGameCoordinates(event->pos()).y());
}

quint64 GameSession::distanceTravelled() {
    return d->x;
}

void GameSession::genObjects() {
    if (d->nextGen < 0) return;
    if (d->nextGen > d->x) return;

    const double genLength = 500;

    std::function<void(QList<GameObjectPtr>)> pushObject = [this, &pushObject](QList<GameObjectPtr> objects) {
        for (auto obj : objects) {
            connect(obj.data(), &GameObject::triggerGameOver, this, &GameSession::triggerGameOver);
            connect(obj.data(), &GameObject::damage, d->player, &Player::damage);
            connect(obj.data(), &GameObject::heal, d->player, &Player::heal);
            connect(obj.data(), &GameObject::refuel, d->player, &Player::addFuel);
            connect(obj.data(), &GameObject::stun, d->player, &Player::stun);

            d->gameObjects.append(obj);
            pushObject(obj->supplementaryObjects());
        }
    };

    // Generate some objects
    int objects = d->random.bounded(20);
    for (auto i = 0; i < objects; i++) {
        auto x = d->random.bounded(genLength);
        x += d->x + genLength;

        auto typeIndex = d->random.bounded(d->gameObjectTypes.length());
        auto type = d->gameObjectTypes.at(typeIndex);
        auto* inst = qobject_cast<GameObject*>(type.newInstance(Q_ARG(double, x), Q_ARG(QRandomGenerator64*, &d->random)));
        pushObject({GameObjectPtr(inst)});
    }
    d->nextGen += genLength;
}

void GameSession::triggerGameOver() {
    d->gameTimer->stop();

    auto* anim = new QVariantAnimation(this);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setDuration(2000);
    anim->setEasingCurve(QEasingCurve::Linear);
    connect(anim, &QVariantAnimation::valueChanged, this, [this](QVariant value) {
        d->gameOverScrimOpacity = value.toReal();
        emit requestPaint();
    });
    connect(anim, &QVariantAnimation::finished, this, [this, anim] {
        anim->deleteLater();
        emit gameSessionEnded();
    });
    anim->start();

    //d->player->setDrawDead(true);
}

void GameSession::resumeAfterPause() {
    d->gameTimer->start();
    d->speedTimer->start();
}

void GameSession::begin() {
    d->gameStarted = true;
    d->player->begin();
    d->speedTimer->start();

    d->nextGen = d->x;
    this->genObjects();

    auto* anim = new QVariantAnimation(this);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setDuration(250);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    connect(anim, &QVariantAnimation::valueChanged, this, [this](QVariant value) {
        d->hudOpacity = value.toReal();
        emit requestPaint();
    });
    connect(anim, &QVariantAnimation::finished, anim, &QVariantAnimation::deleteLater);
    anim->start();
}

void GameSession::tick() {
    d->x += d->speed;
    d->player->tick(d->speed);

    if (!d->gameObjects.isEmpty()) {
        //Remove old game objects
        if (d->gameObjects.first()->x() < d->x - 1000) d->gameObjects.takeFirst();
    }

    d->player->setDrawDead(false);
    auto playerPosition = d->player->poly();
    for (auto gameObject : d->gameObjects) {
        gameObject->tick(playerPosition, d->x);
    }

    if (d->gameStarted) {
        if (d->player->health() <= 2 || d->player->fuel() < 0.2) {
            emit changeAudioState(AudioEngine::State::GameDanger);
        } else {
            emit changeAudioState(AudioEngine::State::Game5H);
        }
    }

    this->genObjects();
    emit requestPaint();
}
