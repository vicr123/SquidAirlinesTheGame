//
// Created by victo on 6/08/2022.
//

#include "Player.h"

#include <QPainter>
#include <QPainterPath>
#include <QRect>

struct PlayerPrivate {
        double y = 200;
        double target = 200;

        double fuel = 1;

        bool gameStarted = false;
        bool drawDead;
};

Player::Player(QObject* parent) :
    QObject(parent) {
    d = new PlayerPrivate();
}

Player::~Player() {
    delete d;
}

void Player::begin() {
    d->gameStarted = true;
}

void Player::draw(QPainter* painter) {
    painter->save();

    painter->setPen(Qt::transparent);
    painter->setBrush(d->drawDead ? Qt::red : Qt::white);
    painter->drawPath(this->planePath());

    painter->restore();
}

QPolygonF Player::poly() {
    return this->planePath().toFillPolygon();
}

double Player::angle() {
    // -180 + whatever
    auto distance = d->y - d->target;
    auto angle = -180 + distance / 2;
    if (angle > -135) angle = -135;
    if (angle < -225) angle = -225;
    return angle;
}

double Player::fuel() {
    return d->fuel;
}

void Player::addFuel(double fuel) {
    d->fuel += fuel;
}

void Player::tick(double xDistance) {
    // Move the aircraft
    QLineF flyLine(QPointF(50, d->y), QPointF(30, d->y));
    flyLine.setAngle(this->angle());
    flyLine.setLength(xDistance);
    d->y = flyLine.pointAt(-1).y();

    if (d->gameStarted) d->fuel -= 0.0001;
    if (d->fuel < 0) d->fuel = 0;
    if (d->fuel > 1) d->fuel = 1;
}

void Player::setTarget(int y) {
    d->target = y;
    if (d->target < 50) d->target = 50;
    if (d->target > 250) d->target = 250;
}

void Player::moveTarget(int y) {
    d->target += y;
    if (d->target < 50) d->target = 50;
    if (d->target > 250) d->target = 250;
}

void Player::setDrawDead(bool drawDead) {
    d->drawDead = drawDead;
}

QPainterPath Player::planePath() {
    QLineF flyLine(QPointF(50, d->y), QPointF(30, d->y));
    flyLine.setAngle(this->angle());

    QLineF wingLine(flyLine.pointAt(0.3), flyLine.pointAt(0.8));
    wingLine.setAngle(flyLine.angle() - 30);

    QPainterPath path;
    path.moveTo(flyLine.p1());
    path.lineTo(flyLine.p2());
    path.moveTo(wingLine.p1());
    path.lineTo(wingLine.p2());

    QPainterPathStroker stroker(QPen(Qt::white, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    return stroker.createStroke(path);
}
