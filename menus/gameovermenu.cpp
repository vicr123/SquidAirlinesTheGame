#include "gameovermenu.h"

#include "nav/NavigationButton.h"
#include "nav/NavigationManager.h"
#include <QPainter>

struct GameOverMenuPrivate {
    NavigationManager* nav;
};

GameOverMenu::GameOverMenu(QObject* parent)
    : AbstractMenu{parent} {
    d = new GameOverMenuPrivate();

    d->nav = new NavigationManager();

    auto mainMenuButton = new NavigationButton(this);
    mainMenuButton->setText("Main Menu");
    mainMenuButton->setCenterPoint(QPoint(400, 400));
    connect(mainMenuButton, &NavigationButton::activated, this, [this] {
        emit mainMenu();
    });
    d->nav->addNavigationElement(mainMenuButton);
}

GameOverMenu::~GameOverMenu() {
    delete d;
}


void GameOverMenu::drawMenu(QPainter* painter, QSize size) {
    auto rect = this->middleRect(size);

    painter->save();

    //OpenGL shenanigans???
    painter->fillRect(rect, Qt::transparent);

    painter->save();
    auto font = painter->font();
    font.setPointSizeF(30);
    painter->setFont(font);

    QRect gameOverRect;
    gameOverRect.setWidth(painter->fontMetrics().horizontalAdvance("Game Over"));
    gameOverRect.setHeight(painter->fontMetrics().height());
    gameOverRect.moveCenter(rect.center());
    gameOverRect.moveTop(rect.top() + 20);

    painter->drawText(gameOverRect, Qt::AlignCenter, "Game Over");
    painter->restore();

    d->nav->paint(painter, rect);

    painter->restore();
}

void GameOverMenu::moveToNext() {
}

void GameOverMenu::moveToPrev() {
}

void GameOverMenu::activate() {
    d->nav->activate();
}
