#include "healthobject.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QRandomGenerator64>

struct HealthObjectPrivate {
    QRectF objectRect;
    bool claimed = false;

    QSvgRenderer health;

};

HealthObject::HealthObject(double x, double y, QRandomGenerator64* random, QObject* parent)
    : GameObject{x, random, parent} {
    d = new HealthObjectPrivate();
    d->health.load(QStringLiteral(":/health.svg"));

    d->objectRect.setSize(QSize(10, 10));
    d->objectRect.moveCenter(QPoint(x, y));
}

HealthObject::~HealthObject() {
    delete d;
}


void HealthObject::draw(QPainter* painter, double xOffset) {
    if (d->claimed) return;
    d->health.render(painter, d->objectRect.translated(-xOffset, 0));
}

void HealthObject::tick(QPolygonF playerPosition, double xOffset) {
    if (d->claimed) return;
    if (playerPosition.intersects(d->objectRect.translated(-xOffset, 0))) {
        d->claimed = true;
        emit heal();
    }
}
