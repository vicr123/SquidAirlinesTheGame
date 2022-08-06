#ifndef FUELOBJECT_H
#define FUELOBJECT_H

#include "gameobject.h"

struct FuelObjectPrivate;
class FuelObject : public GameObject {
        Q_OBJECT
    public:
        explicit FuelObject(double x, double y, QRandomGenerator64* random, QObject* parent = nullptr);
        ~FuelObject();

    private:
        FuelObjectPrivate* d;

        // GameObject interface
    public:
        void draw(QPainter* painter, double xOffset);
        void tick(QPolygonF playerPosition, double xOffset);
};

#endif // FUELOBJECT_H
