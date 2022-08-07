#ifndef TANKOBJECT_H
#define TANKOBJECT_H

#include "gameobject.h"

struct TankObjectPrivate;
class TankObject : public GameObject {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit TankObject(double x, QRandomGenerator64* random, QObject* parent = nullptr);
        ~TankObject();

        QLineF laser();

    private:
        TankObjectPrivate* d;

        // GameObject interface
    public:
        void draw(QPainter* painter, double xOffset);
        void tick(QPolygonF playerPosition, double xOffset);
};

#endif // TANKOBJECT_H
