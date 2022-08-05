//
// Created by victo on 5/08/2022.
//

#include "NavigationManager.h"
#include "NavigationElement.h"

#include <QRect>

struct NavigationManagerPrivate {
    int currentEl = 0;
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
    connect(element, &NavigationElement::requestPaint, this, &NavigationManager::requestPaint);
}

void NavigationManager::paint(QPainter *painter, QRect rect) {
    for (auto i = 0; i < d->elements.length(); i++) d->elements.at(i)->paint(painter, rect, d->currentEl == i);
}

void NavigationManager::moveToNext() {
    d->currentEl++;
    if (d->elements.length() == d->currentEl) d->currentEl = 0;
    emit requestPaint();
}

void NavigationManager::moveToPrev() {
    d->currentEl--;
    if (d->currentEl == -1) d->currentEl = d->elements.length() - 1;
    emit requestPaint();
}
void NavigationManager::activate() {
    if (d->currentEl < 0) return;
    if (d->currentEl >= d->elements.length()) return;

    d->elements.at(d->currentEl)->activate();
}
