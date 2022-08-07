#ifndef HEALTHOBJECT_H
#define HEALTHOBJECT_H

#include "gameobject.h"

struct HealthObjectPrivate;
class HealthObject : public GameObject {
        Q_OBJECT
    public:
        explicit HealthObject(double x, double y, QRandomGenerator64* random, QObject* parent = nullptr);
        ~HealthObject();

    private:
        HealthObjectPrivate* d;

        // GameObject interface
    public:
        void draw(QPainter* painter, double xOffset);
        void tick(QPolygonF playerPosition, double xOffset);
};

#endif // HEALTHOBJECT_H
