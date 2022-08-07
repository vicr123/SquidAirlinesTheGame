//
// Created by victo on 6/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_PLAYER_H
#define SQUIDAIRLINESTHEGAME_PLAYER_H

#include <QObject>

class QRandomGenerator64;
class QPainterPath;
class QPainter;
struct PlayerPrivate;
class Player : public QObject {
        Q_OBJECT
    public:
        explicit Player(QRandomGenerator64* random, QObject* parent = nullptr);
        ~Player() override;

        void begin();

        void draw(QPainter* painter);
        QPolygonF poly();

        double angle();

        double fuel();
        void addFuel(double fuel);

        int health();
        void damage();

        void tick(double xDistance);

        void setTarget(int y);
        void moveTarget(int y);

        void setDrawDead(bool drawDead);

signals:
        void triggerGameOver();

    private:
        PlayerPrivate* d;

        QPainterPath planePath();
};

#endif // SQUIDAIRLINESTHEGAME_PLAYER_H
