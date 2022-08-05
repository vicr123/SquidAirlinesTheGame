//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_MAINMENU_H
#define SQUIDAIRLINESTHEGAME_MAINMENU_H

#include "AbstractMenu.h"

struct MainMenuPrivate;
class MainMenu : public AbstractMenu {
public:
    explicit MainMenu(QObject* parent = nullptr);
    ~MainMenu();

    void drawMenu(QPainter *painter, QSize size) override;

private:
    MainMenuPrivate* d;
};


#endif//SQUIDAIRLINESTHEGAME_MAINMENU_H
