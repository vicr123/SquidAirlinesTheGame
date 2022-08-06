#include "buildingobject.h"

#include "fuelobject.h"
#include <QPainter>
#include <QRandomGenerator64>

struct BuildingObjectPrivate {
        QRectF buildingRect;
        QColor color;
};

BuildingObject::BuildingObject(double x, QRandomGenerator64* random, QObject* parent) :
    GameObject{x, random, parent} {
    d = new BuildingObjectPrivate();

    d->buildingRect.setHeight(random->bounded(200));
    d->buildingRect.setWidth(random->bounded(50, 100));
    d->buildingRect.moveBottomLeft(QPoint(x, 300));

    d->color = QColor(random->bounded(20), random->bounded(20), random->bounded(20));
}

BuildingObject::~BuildingObject() {
    delete d;
}

void BuildingObject::draw(QPainter* painter, double xOffset) {
    painter->save();

    painter->fillRect(d->buildingRect.translated(-xOffset, 0), d->color);

    painter->restore();
}

void BuildingObject::tick(QPolygonF playerPosition, double xOffset) {
    if (playerPosition.intersects(d->buildingRect.translated(-xOffset, 0))) {
        emit triggerGameOver();
    }
}

QList<QSharedPointer<GameObject>> BuildingObject::supplementaryObjects() {
    switch (this->random()->bounded(10)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            break;
        case 7:
        case 8:
        case 9:
            return {
                GameObjectPtr(new FuelObject(d->buildingRect.center().x(), d->buildingRect.top() - 10, this->random()))};
    }

    return {};
}
