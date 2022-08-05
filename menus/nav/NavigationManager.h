//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_NAVIGATIONMANAGER_H
#define SQUIDAIRLINESTHEGAME_NAVIGATIONMANAGER_H

#include <QObject>

class NavigationElement;
struct NavigationManagerPrivate;
class NavigationManager : public QObject {
    Q_OBJECT
public:
    explicit NavigationManager(QObject *parent = nullptr);
    ~NavigationManager();

    void addNavigationElement(NavigationElement* element);
private:
    NavigationManagerPrivate* d;
};


#endif//SQUIDAIRLINESTHEGAME_NAVIGATIONMANAGER_H
