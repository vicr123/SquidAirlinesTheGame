#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "AbstractMenu.h"

struct PauseMenuPrivate;
class PauseMenu : public AbstractMenu {
        Q_OBJECT
    public:
        explicit PauseMenu(QObject* parent = nullptr);
        ~PauseMenu();

    signals:
        void resume();
        void quit();

    private:
        PauseMenuPrivate* d;

        // AbstractMenu interface
    public:
        void drawMenu(QPainter* painter, QSize size);
        void moveToNext();
        void moveToPrev();
        void activate();
};

#endif // PAUSEMENU_H
