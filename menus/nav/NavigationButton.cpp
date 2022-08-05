//
// Created by victo on 5/08/2022.
//

#include "NavigationButton.h"

#include <QApplication>
#include <QRect>
#include <QPainter>

struct NavigationButtonPrivate {
    QString text;
    QPoint centerPoint;
};

NavigationButton::NavigationButton(QObject *parent) {
    d = new NavigationButtonPrivate();
}
NavigationButton::~NavigationButton() {
    delete d;
}

void NavigationButton::paint(QPainter *painter, QRect rect, bool hasFocus) {
    QRect textRect;
    textRect.setWidth(painter->fontMetrics().horizontalAdvance(d->text));
    textRect.setHeight(painter->fontMetrics().height());
    textRect.moveCenter(rect.topLeft() + d->centerPoint);

    painter->fillRect(textRect.adjusted(-10, -10, 10, 10),  hasFocus ? Qt::red : Qt::white);
    painter->drawText(textRect, Qt::AlignCenter, d->text);
}

void NavigationButton::setText(QString text) {
    d->text = text;
}

void NavigationButton::setCenterPoint(QPoint centerPoint) {
    d->centerPoint = centerPoint;
}
void NavigationButton::activate() {
    NavigationElement::activate();
    emit this->activated();
}
