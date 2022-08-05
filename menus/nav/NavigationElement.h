//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_NAVIGATIONELEMENT_H
#define SQUIDAIRLINESTHEGAME_NAVIGATIONELEMENT_H

#include <QObject>
class NavigationElement : public QObject {
    Q_OBJECT
public:
    explicit NavigationElement(QObject *parent = nullptr);
};


#endif//SQUIDAIRLINESTHEGAME_NAVIGATIONELEMENT_H
