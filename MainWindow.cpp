//
// Created by victo on 5/08/2022.
//

#include "MainWindow.h"
#include "game/GameSession.h"
#include "game/audioengine.h"
#include "menus/MainMenu.h"
#include "menus/gameovermenu.h"
#include "menus/pausemenu.h"
#include <QIcon>
#include <QKeyEvent>
#include <QPainter>
#include <QSvgRenderer>
#include <QTimer>
#include <QVariantAnimation>

struct MainWindowPrivate {
        QList<AbstractMenu*> menusToDraw;

        AudioEngine* audio;

        QPointer<GameSession> session;

        MainMenu* mainMenu;
        GameOverMenu* gameOverMenu;
        PauseMenu* pauseMenu;

        bool playing = false;

        QSvgRenderer publisherRenderer;
        double publisherOpacity = 1;
};

MainWindow::MainWindow(QWidget* parent) :
    QOpenGLWidget(parent) {
    d = new MainWindowPrivate();
    this->resize(1024, 768);
    this->setMouseTracking(true);
    this->setMinimumSize(1024, 768);

    this->setWindowIcon(QIcon(":/app-icon.svg"));

    d->publisherRenderer.load(QStringLiteral(":/entertaining-logo-wordmark-dark.svg"));

    auto surfaceFormat = this->format();
    surfaceFormat.setSamples(10);
    surfaceFormat.setDepthBufferSize(24);
    surfaceFormat.setStencilBufferSize(8);
    this->setFormat(surfaceFormat);

    this->prepareNewGameSession();

    d->mainMenu = new MainMenu(this);

    connect(d->mainMenu, &MainMenu::requestPaint, this, [this] {
        this->update();
    });
    connect(d->mainMenu, &MainMenu::startGame, this, [this] {
        d->playing = true;
        d->session->begin();
        d->audio->setState(AudioEngine::State::Game5H);
    });

    d->gameOverMenu = new GameOverMenu(this);
    connect(d->gameOverMenu, &GameOverMenu::requestPaint, this, [this] {
        this->update();
    });
    connect(d->gameOverMenu, &GameOverMenu::mainMenu, this, [this] {
        d->menusToDraw.removeAll(d->gameOverMenu);
        d->mainMenu->showAgain();
        d->audio->setState(AudioEngine::State::PreGame);
        this->prepareNewGameSession();
    });

    d->pauseMenu = new PauseMenu(this);
    connect(d->pauseMenu, &PauseMenu::requestPaint, this, [this] {
        this->update();
    });
    connect(d->pauseMenu, &PauseMenu::resume, this, [this] {
        d->playing = true;
        d->session->resumeAfterPause();
        d->menusToDraw.removeAll(d->pauseMenu);
    });
    connect(d->pauseMenu, &PauseMenu::quit, this, [this] {
        d->menusToDraw.removeAll(d->pauseMenu);
        d->mainMenu->showAgain();
        d->audio->setState(AudioEngine::State::PreGame);
        this->prepareNewGameSession();
    });

    QTimer::singleShot(3000, this, [this] {
        QVariantAnimation* anim = new QVariantAnimation(this);
        anim->setStartValue(1.0);
        anim->setEndValue(0.0);
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        connect(anim, &QVariantAnimation::valueChanged, this, [this](QVariant value) {
            d->publisherOpacity = value.toDouble();
            this->update();
        });
        connect(anim, &QVariantAnimation::finished, this, [this, anim] {
            anim->deleteLater();
            d->menusToDraw.append(d->mainMenu);

            d->audio = new AudioEngine(this);
        });
        anim->start();
    });
}

MainWindow::~MainWindow() {
    delete d;
}

void MainWindow::paintEvent(QPaintEvent* e) {
    QOpenGLWidget::paintEvent(e);

    if (d->publisherOpacity > 0) {
        QPainter painter(this);
        painter.fillRect(QRect(0, 0, this->width(), this->height()), Qt::black);

        painter.setOpacity(d->publisherOpacity);

        QRect publisherRect;
        publisherRect.setSize(d->publisherRenderer.defaultSize());
        publisherRect.moveCenter(QRect(0, 0, this->width(), this->height()).center());
        d->publisherRenderer.render(&painter, publisherRect);
    } else {
        QLinearGradient sky(QPoint(0, this->height()), QPoint(0, 0));
        sky.setColorAt(0, QColor(0, 200, 255));
        sky.setColorAt(1, QColor(0, 100, 255));

        QPainter painter(this);
        painter.fillRect(QRect(0, 0, this->width(), this->height()), sky);

        if (d->session) {
            d->session->drawScreen(&painter, QSize(this->width(), this->height()));
        }

        for (auto menu : d->menusToDraw) {
            menu->drawMenu(&painter, QSize(this->width(), this->height()));
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    QWidget::keyPressEvent(event);

    if (d->publisherOpacity > 0) return;

    if (d->playing) {
        d->session->keyPressEvent(event);
    } else {
        switch (event->key()) {
            case Qt::Key_Left:
            case Qt::Key_Up:
                d->menusToDraw.last()->moveToPrev();
                break;
            case Qt::Key_Right:
            case Qt::Key_Down:
                d->menusToDraw.last()->moveToNext();
                break;
            case Qt::Key_Return:
                d->menusToDraw.last()->activate();
        }
        //        d->menusToDraw.last()->
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (d->publisherOpacity > 0) return;

    if (d->playing) {
        d->session->mouseMoveEvent(event);
    }
}

void MainWindow::prepareNewGameSession() {
    if (d->session) {
        d->session->deleteLater();
    }

    d->session = new GameSession();
    connect(d->session, &GameSession::requestPaint, this, [this] {
        this->update();
    });
    connect(d->session, &GameSession::changeAudioState, this, [this](AudioEngine::State state) {
        d->audio->setState(state);
    });
    connect(d->session, &GameSession::paused, this, [this]() {
        d->playing = false;
        d->audio->setState(AudioEngine::State::EndGame);
        d->menusToDraw.append(d->pauseMenu);
    });
    connect(d->session, &GameSession::gameSessionEnded, this, [this] {
        d->playing = false;
        d->gameOverMenu->setDistanceTravelled(d->session->distanceTravelled());
        d->menusToDraw.append(d->gameOverMenu);
        d->audio->setState(AudioEngine::State::EndGame);

        this->update();
    });
}
