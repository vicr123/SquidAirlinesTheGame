//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_ABSTRACTMENU_H
#define SQUIDAIRLINESTHEGAME_ABSTRACTMENU_H

#include <QObject>

class QPainter;
class QSize;
class AbstractMenu : public QObject {
    Q_OBJECT
public:
    explicit AbstractMenu(QObject *parent = nullptr);

    virtual void drawMenu(QPainter* painter, QSize size) = 0;

};


#endif//SQUIDAIRLINESTHEGAME_ABSTRACTMENU_H
