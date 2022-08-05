//
// Created by victo on 5/08/2022.
//

#include "NavigationManager.h"

struct NavigationManagerPrivate {
    QList<NavigationElement*> elements;
};

NavigationManager::NavigationManager(QObject *parent) : QObject(parent) {
    d = new NavigationManagerPrivate();
}

NavigationManager::~NavigationManager() {
    delete d;
}

void NavigationManager::addNavigationElement(NavigationElement *element) {
    d->elements.append(element);
}
