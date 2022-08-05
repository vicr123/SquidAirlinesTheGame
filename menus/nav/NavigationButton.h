//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_NAVIGATIONBUTTON_H
#define SQUIDAIRLINESTHEGAME_NAVIGATIONBUTTON_H


#include "NavigationElement.h"

struct NavigationButtonPrivate;
class NavigationButton : public NavigationElement {
    Q_OBJECT
public:
    explicit NavigationButton(QObject* parent = nullptr);
    ~NavigationButton();

    void setText(QString text);
    void setCenterPoint(QPoint centerPoint);

    void paint(QPainter *painter, QRect rect, bool hasFocus) override;
    void activate() override;

    signals:
        void activated();

private:
    NavigationButtonPrivate* d;
};


#endif//SQUIDAIRLINESTHEGAME_NAVIGATIONBUTTON_H
