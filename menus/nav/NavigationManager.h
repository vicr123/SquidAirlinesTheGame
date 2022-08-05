//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_NAVIGATIONMANAGER_H
#define SQUIDAIRLINESTHEGAME_NAVIGATIONMANAGER_H

#include <QObject>

class NavigationElement;
class QPainter;
struct NavigationManagerPrivate;
class NavigationManager : public QObject {
    Q_OBJECT
public:
    explicit NavigationManager(QObject *parent = nullptr);
    ~NavigationManager();

    void addNavigationElement(NavigationElement* element);

    void paint(QPainter *painter, QRect rect);

    void moveToNext();
    void moveToPrev();
    void activate();

signals:
    void requestPaint();

private:
    NavigationManagerPrivate* d;
};


#endif//SQUIDAIRLINESTHEGAME_NAVIGATIONMANAGER_H
