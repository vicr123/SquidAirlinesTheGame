//
// Created by victo on 6/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_PLAYER_H
#define SQUIDAIRLINESTHEGAME_PLAYER_H

#include <QObject>

class QPainter;
struct PlayerPrivate;
class Player : public QObject {
        Q_OBJECT
    public:
        explicit Player(QObject* parent = nullptr);
        ~Player() override;

        void draw(QPainter* painter);

        double angle();

        void setTarget(int y);
        void moveTarget(int y);

    private:
        PlayerPrivate* d;
};

#endif // SQUIDAIRLINESTHEGAME_PLAYER_H
