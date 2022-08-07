#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

#include "AbstractMenu.h"

struct GameOverMenuPrivate;
class GameOverMenu : public AbstractMenu {
        Q_OBJECT
    public:
        explicit GameOverMenu(QObject* parent = nullptr);
        ~GameOverMenu();

    signals:
        void mainMenu();

    private:
        GameOverMenuPrivate* d;

        // AbstractMenu interface
    public:
        void drawMenu(QPainter* painter, QSize size);
        void moveToNext();
        void moveToPrev();
        void activate();
};

#endif // GAMEOVERMENU_H
