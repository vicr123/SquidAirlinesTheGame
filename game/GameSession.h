//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_GAMESESSION_H
#define SQUIDAIRLINESTHEGAME_GAMESESSION_H

#include <QObject>

class QKeyEvent;
class QMouseEvent;
class QPainter;
struct GameSessionPrivate;
class GameSession : public QObject {
        Q_OBJECT
    public:
        explicit GameSession(QObject* parent = nullptr);
        ~GameSession();

        void begin();
        void drawScreen(QPainter* painter, QSize size);

        QPointF toGameCoordinates(QPointF windowCoordinates);

        void keyPressEvent(QKeyEvent* event);
        void mouseMoveEvent(QMouseEvent* event);

        void genObjects();

    signals:
        void gameSessionEnded();
        void requestPaint();

    private:
        GameSessionPrivate* d;

        void tick();
};

#endif // SQUIDAIRLINESTHEGAME_GAMESESSION_H
