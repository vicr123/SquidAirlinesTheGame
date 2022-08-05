//
// Created by victo on 5/08/2022.
//

#include "AbstractMenu.h"
#include "MainMenu.h"
#include <QPainter>
#include <QSize>

AbstractMenu::AbstractMenu(QObject *parent) : QObject(parent) {

}
QRect AbstractMenu::middleRect(QSize size) {
    QRect geom(QPoint(0, 0), size);
    QRect mid(0, 0, 800, 600);
    mid.moveCenter(geom.center());
    return mid;
}