//
// Created by victo on 6/08/2022.
//

#include "Player.h"

#include <QPainter>
#include <QRect>

struct PlayerPrivate {
    double y = 200;

    double target = 200;
};

Player::Player(QObject* parent) :
    QObject(parent) {
    d = new PlayerPrivate();
}

Player::~Player() {
    delete d;
}
void Player::draw(QPainter* painter) {
    painter->save();

    QLineF flyLine(QPointF(50, d->y), QPointF(30, d->y));
    flyLine.setAngle(this->angle());

    painter->setPen(QPen(Qt::white, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(flyLine);

    QLineF wingLine(flyLine.pointAt(0.3), flyLine.pointAt(0.8));
    wingLine.setAngle(flyLine.angle() - 30);
    painter->drawLine(wingLine);


    painter->restore();
}

double Player::angle() {
    // -180 + whatever
    auto distance = d->y - d->target;
    auto angle = -180 + distance / 2;
    if (angle > -135) angle = -135;
    if (angle < -225) angle = -225;
    return angle;
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
