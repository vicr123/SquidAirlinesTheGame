#include "buildingobject.h"

#include "fuelobject.h"
#include "healthobject.h"
#include <QPainter>
#include <QRandomGenerator64>
#include <QSvgRenderer>

struct BuildingObjectPrivate {
    QRectF buildingRect;
    QColor color;

    QSvgRenderer buildingLogo;
};

BuildingObject::BuildingObject(double x, QRandomGenerator64* random, QObject* parent) :
    GameObject{x, random, parent} {
    d = new BuildingObjectPrivate();

    d->buildingRect.setHeight(random->bounded(200));
    d->buildingRect.setWidth(random->bounded(50, 100));
    d->buildingRect.moveBottomLeft(QPoint(x, 300));

    d->color = QColor(random->bounded(20), random->bounded(20), random->bounded(20));

    switch (random->bounded(5)) {
        case 0:
            d->buildingLogo.load(QStringLiteral(":/frivoloco.svg"));
            break;
        default:
            break;
    }
}

BuildingObject::~BuildingObject() {
    delete d;
}

void BuildingObject::draw(QPainter* painter, double xOffset) {
    painter->save();

    painter->fillRect(d->buildingRect.translated(-xOffset, 0), d->color);

    if (d->buildingLogo.isValid()) {
        auto logoSize = d->buildingRect.size();
        if (logoSize.width() > 30 && logoSize.height() > 30) {
            QRectF rect;
            rect.setSize(QSizeF(d->buildingLogo.defaultSize()).scaled(d->buildingRect.size() - QSize(20, 20), Qt::KeepAspectRatio));
            rect.moveTopLeft(d->buildingRect.topLeft() + QPointF(10, 10) + QPointF(-xOffset, 0));
            d->buildingLogo.render(painter, rect);
        }
    }

    painter->restore();
}

void BuildingObject::tick(QPolygonF playerPosition, double xOffset) {
    if (playerPosition.intersects(d->buildingRect.translated(-xOffset, 0))) {
        emit triggerGameOver();
    }
}

QList<QSharedPointer<GameObject>> BuildingObject::supplementaryObjects() {
    switch (this->random()->bounded(50)) {
        case 7:
        case 8:
        case 9:
        case 0:
        case 1:
        case 2:
            return {
                GameObjectPtr(new FuelObject(d->buildingRect.center().x(), d->buildingRect.top() - 10, this->random()))};
        case 10:
            return {
                GameObjectPtr(new HealthObject(d->buildingRect.center().x(), d->buildingRect.top() - 10, this->random()))};
        default:
            break;
    }

    return {};
}
