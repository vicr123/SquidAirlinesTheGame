#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QSharedPointer>

class QPainter;
class QRandomGenerator64;
struct GameObjectPrivate;
class GameObject : public QObject {
        Q_OBJECT
    public:
        explicit GameObject(double x, QRandomGenerator64* random, QObject* parent = nullptr);
        ~GameObject();

        virtual void draw(QPainter* painter, double xOffset) = 0;
        virtual void tick(QPolygonF playerPosition, double xOffset) = 0;
        virtual QList<QSharedPointer<GameObject>> supplementaryObjects();

        double x();
        QRandomGenerator64* random();

    signals:
        void triggerGameOver();
        void refuel(double amount);
        void damage();

    private:
        GameObjectPrivate* d;
};

typedef QSharedPointer<GameObject> GameObjectPtr;

#endif // GAMEOBJECT_H
