#include "pausemenu.h"
#include "nav/NavigationButton.h"
#include "nav/NavigationManager.h"
#include <QPainter>

struct PauseMenuPrivate {
    NavigationManager* nav;
};

PauseMenu::PauseMenu(QObject* parent)
    : AbstractMenu{parent} {
    d = new PauseMenuPrivate();
    d->nav = new NavigationManager();

    auto resumeButton = new NavigationButton(this);
    resumeButton->setText("Resume");
    resumeButton->setCenterPoint(QPoint(400, 450));
    connect(resumeButton, &NavigationButton::activated, this, [this, parent] {
        emit resume();
    });
    d->nav->addNavigationElement(resumeButton);

    auto quitButton = new NavigationButton(this);
    quitButton->setText("Main Menu");
    quitButton->setCenterPoint(QPoint(400, 500));
    connect(quitButton, &NavigationButton::activated, this, [this] {
        emit quit();
    });
    d->nav->addNavigationElement(quitButton);

    connect(d->nav, &NavigationManager::requestPaint, this, &PauseMenu::requestPaint);
}

PauseMenu::~PauseMenu() {
    delete d;
}


void PauseMenu::drawMenu(QPainter* painter, QSize size) {
    //Block the game from being visible
    QLinearGradient sky(QPoint(0, size.height()), QPoint(0, 0));
    sky.setColorAt(0, QColor(0, 200, 255));
    sky.setColorAt(1, QColor(0, 100, 255));

    painter->fillRect(QRect(QPoint(0, 0), size), sky);

    auto rect = this->middleRect(size);

    painter->save();

    //OpenGL shenanigans???
    painter->fillRect(rect, Qt::transparent);

    painter->save();
    auto font = painter->font();
    font.setPointSizeF(30);
    painter->setFont(font);

    QRect gameOverRect;
    gameOverRect.setWidth(painter->fontMetrics().horizontalAdvance("Pause"));
    gameOverRect.setHeight(painter->fontMetrics().height());
    gameOverRect.moveCenter(rect.center());
    gameOverRect.moveTop(rect.top() + 20);

    painter->drawText(gameOverRect, Qt::AlignCenter, "Pause");
    painter->restore();

    d->nav->paint(painter, rect);

    painter->restore();
}

void PauseMenu::moveToNext() {
    d->nav->moveToNext();
}

void PauseMenu::moveToPrev() {
    d->nav->moveToPrev();
}

void PauseMenu::activate() {
    d->nav->activate();
}
