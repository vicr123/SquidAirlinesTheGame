#include "gameobject.h"

struct GameObjectPrivate {
    double x;
    double y;
    QRandomGenerator64* random;
};

GameObject::GameObject(double x, QRandomGenerator64* random, QObject* parent)
    : QObject{parent} {
    d = new GameObjectPrivate();
    d->x = x;
    d->random = random;
}
GameObject::~GameObject() {
    delete d;
}

double GameObject::x() {
    return d->x;
}

QRandomGenerator64* GameObject::random() {
    return d->random;
}
