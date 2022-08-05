//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_MAINMENU_H
#define SQUIDAIRLINESTHEGAME_MAINMENU_H

#include "AbstractMenu.h"

struct MainMenuPrivate;
class MainMenu : public AbstractMenu {
        Q_OBJECT
public:
    explicit MainMenu(QObject* parent = nullptr);
    ~MainMenu();

    void drawMenu(QPainter *painter, QSize size) override;

    void moveToNext() override;
    void moveToPrev() override;
    void activate() override;

    void showAgain();

    signals:
        void startGame();

private:
    MainMenuPrivate* d;
};


#endif//SQUIDAIRLINESTHEGAME_MAINMENU_H
