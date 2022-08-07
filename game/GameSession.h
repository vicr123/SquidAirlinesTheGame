//
// Created by victo on 5/08/2022.
//

#ifndef SQUIDAIRLINESTHEGAME_GAMESESSION_H
#define SQUIDAIRLINESTHEGAME_GAMESESSION_H

#include <QObject>
#include "audioengine.h"

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

        quint64 distanceTravelled();

        void genObjects();

        void triggerGameOver();
        void resumeAfterPause();

    signals:
        void gameSessionEnded();
        void requestPaint();
        void changeAudioState(AudioEngine::State state);
        void paused();

    private:
        GameSessionPrivate* d;

        void tick();
};

#endif // SQUIDAIRLINESTHEGAME_GAMESESSION_H
