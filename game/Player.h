//
// Created by victo on 6/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_PLAYER_H
#define SQUIDAIRLINESTHEGAME_PLAYER_H

#include <QObject>

class QPainterPath;
class QPainter;
struct PlayerPrivate;
class Player : public QObject {
        Q_OBJECT
    public:
        explicit Player(QObject* parent = nullptr);
        ~Player() override;

        void draw(QPainter* painter);
        QPolygonF poly();

        double angle();

        void tick(double xDistance);

        void setTarget(int y);
        void moveTarget(int y);

        void setDrawDead(bool drawDead);

    private:
        PlayerPrivate* d;

        QPainterPath planePath();
};

#endif // SQUIDAIRLINESTHEGAME_PLAYER_H
