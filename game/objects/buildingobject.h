#ifndef BUILDINGOBJECT_H
#define BUILDINGOBJECT_H

#include "gameobject.h"
#include <QObject>

struct BuildingObjectPrivate;
class BuildingObject : public GameObject {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit BuildingObject(double x, QRandomGenerator64* random, QObject* parent = nullptr);
        ~BuildingObject();

    private:
        BuildingObjectPrivate* d;

        // GameObject interface
    public:
        void draw(QPainter* painter, double xOffset);
        void tick(QPolygonF playerPosition, double xOffset);
        QList<QSharedPointer<GameObject>> supplementaryObjects();
};

#endif // BUILDINGOBJECT_H
