#ifndef THUNDERSTORMOBJECT_H
#define THUNDERSTORMOBJECT_H

#include "gameobject.h"

struct ThunderstormObjectPrivate;
class ThunderstormObject : public GameObject {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit ThunderstormObject(double x, QRandomGenerator64* random, QObject* parent = nullptr);
        ~ThunderstormObject();

    private:
        ThunderstormObjectPrivate* d;

        // GameObject interface
    public:
        void draw(QPainter* painter, double xOffset);
        void tick(QPolygonF playerPosition, double xOffset);
};

#endif // THUNDERSTORMOBJECT_H
