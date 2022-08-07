#include "thunderstormobject.h"

#include <QRandomGenerator64>
#include <QPainter>
#include <QPainterPath>

struct ThunderstormObjectPrivate {
    QList<QPainterPath> storms;

    double strength;
    double y;

    bool zapping = false;
    int zap = 50;
    QPointF zapAt;
};

ThunderstormObject::ThunderstormObject(double x, QRandomGenerator64* random, QObject* parent)
    : GameObject{x, random, parent} {
    d = new ThunderstormObjectPrivate();

    d->y = random->bounded(180.0) + 100.0;
    d->strength = random->bounded(0.6) + 0.2;

    auto circles = random->bounded(10);
    for (auto i = 0; i < circles; i++) {
        //Generate a reasonable random circle
        auto yDelta = random->bounded(10.0) - 15;
        auto xDelta = random->bounded(30.0) - 15;

        auto center = QPointF(x, d->y) + QPointF(xDelta, yDelta);
        auto radius = random->bounded(7.0) + 3.0;

        QPainterPath path;
        path.addEllipse(center, radius, radius);
        d->storms.append(path);
    }
}

ThunderstormObject::~ThunderstormObject() {
    delete d;
}


void ThunderstormObject::draw(QPainter* painter, double xOffset) {
    painter->save();

    painter->setPen(Qt::transparent);
    painter->setBrush(QColor(150, 150, 150, d->strength * 255));
    for (auto path : d->storms) {
        painter->drawPath(path.translated(-xOffset, 0));
    }

    if (d->zapping) {
        QPolygonF poly;
        poly.append(QPointF(3, 0));
        poly.append(QPointF(0, 7));
        poly.append(QPointF(5, 7));
        poly.append(QPointF(3, 14));

        poly.translate(d->zapAt - QPointF(3, 5) + QPointF(-xOffset, 0));
        painter->setPen(QPen(QColor(255, 150, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(Qt::transparent);
        painter->drawPolyline(poly);
    }

    painter->restore();
}

void ThunderstormObject::tick(QPolygonF playerPosition, double xOffset) {
    d->zap--;
    if (d->zap == 0) {
        d->zapping = !d->zapping;
        if (d->zapping) {
            auto yDelta = this->random()->bounded(10.0) - 15;
            auto xDelta = this->random()->bounded(30.0) - 15;

            d->zapAt = QPointF(this->x(), d->y) + QPointF(xDelta, yDelta);
            d->zap = 10;
        } else {
            d->zap = this->random()->bounded(20, 100);
        }
    }

    for (auto cloud : d->storms) {
        if (playerPosition.intersects(cloud.translated(-xOffset, 0).toFillPolygon())) {
            emit stun(100 * d->strength);
        }
    }
}
