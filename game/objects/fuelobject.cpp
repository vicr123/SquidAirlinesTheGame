#include "fuelobject.h"

#include <QPainter>
#include <QPolygonF>
#include <QSvgRenderer>
#include <QRandomGenerator64>

struct FuelObjectPrivate {
    QRectF objectRect;
    bool claimed = false;

    QSvgRenderer fuel;
};

FuelObject::FuelObject(double x, double y, QRandomGenerator64* random, QObject* parent) :
    GameObject{x, random, parent} {
    d = new FuelObjectPrivate();

    d->fuel.load(QStringLiteral(":/fuel.svg"));

    d->objectRect.setSize(QSize(10, 10));
    d->objectRect.moveCenter(QPoint(x, y));
}

FuelObject::~FuelObject() {
    delete d;
}

void FuelObject::draw(QPainter* painter, double xOffset) {
    if (d->claimed) return;
//    painter->fillRect(d->objectRect.translated(-xOffset, 0), Qt::red);
    d->fuel.render(painter, d->objectRect.translated(-xOffset, 0));
}

void FuelObject::tick(QPolygonF playerPosition, double xOffset) {
    if (d->claimed) return;
    if (playerPosition.intersects(d->objectRect.translated(-xOffset, 0))) {
        d->claimed = true;
        emit refuel(random()->bounded(0.2) + 0.1);
    }
}
