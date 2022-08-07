#include "tankobject.h"

#include <QRandomGenerator64>
#include <QPainter>
#include <QPainterPath>

struct TankObjectPrivate {
    QRect tankRect1;
    QRect tankRect2;

    double laserAngle = 60;
    int timeUntilFire;

    bool angleLeft = true;
    bool firing = false;
};

TankObject::TankObject(double x, QRandomGenerator64* random, QObject* parent)
    : GameObject{x, random, parent} {
    d = new TankObjectPrivate();

    d->timeUntilFire = random->bounded(150);
    d->laserAngle = random->bounded(60.0) + 60;
    d->angleLeft = random->bounded(1) == 1;

    d->tankRect1.setSize(QSize(50, 20));
    d->tankRect1.moveBottomLeft(QPoint(x, 300));

    d->tankRect2.setSize(QSize(20, 10));
    d->tankRect2.moveCenter(d->tankRect1.center());
    d->tankRect2.moveBottom(d->tankRect1.top());
}

TankObject::~TankObject() {
    delete d;
}

QLineF TankObject::laser() {
    QLineF laser;
    laser.setP1(d->tankRect2.center());
    laser.setP2(QPoint(0, 0));
    laser.setLength(500);
    laser.setAngle(d->laserAngle);
    return laser;
}

void TankObject::draw(QPainter* painter, double xOffset) {
    painter->save();

    painter->fillRect(d->tankRect1.translated(-xOffset, 0), Qt::red);
    painter->fillRect(d->tankRect2.translated(-xOffset, 0), Qt::red);

    painter->setPen(d->firing ? Qt::red : QColor(255, 0, 0, 50));
    painter->drawLine(this->laser().translated(-xOffset, 0));

    painter->restore();
}

void TankObject::tick(QPolygonF playerPosition, double xOffset) {
    if (d->angleLeft) {
        d->laserAngle += 0.1;
    } else {
        d->laserAngle -= 0.1;
    }

    if (d->laserAngle <= 60) d->angleLeft = true;
    if (d->laserAngle >= 120) d->angleLeft = false;

    d->timeUntilFire--;
    if (d->timeUntilFire == 0) {
        d->firing = !d->firing;
        d->timeUntilFire = d->firing ? 10 : 150;
    }

    if (d->firing) {
        auto laser = this->laser().translated(-xOffset, 0);
        QPolygonF poly;
        poly.append(laser.p1());
        poly.append(laser.p2());
        poly.append(laser.p1());
        if (playerPosition.intersects(poly)) {
            d->firing = false;
            d->timeUntilFire = 150;
            emit damage();
        }
    }
}
