//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_NAVIGATIONELEMENT_H
#define SQUIDAIRLINESTHEGAME_NAVIGATIONELEMENT_H

#include <QObject>

class QPainter;
class NavigationElement : public QObject {
    Q_OBJECT
public:
    explicit NavigationElement(QObject *parent = nullptr);

    virtual void paint(QPainter *painter, QRect rect, bool hasFocus) = 0;
    virtual void activate();

signals:
    void requestPaint();
};


#endif//SQUIDAIRLINESTHEGAME_NAVIGATIONELEMENT_H
