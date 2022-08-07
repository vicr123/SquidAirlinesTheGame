//
// Created by victo on 5/08/2022.
//

#include "MainMenu.h"
#include "AbstractMenu.h"
#include "nav/NavigationButton.h"
#include "nav/NavigationManager.h"

#include <QApplication>
#include <QPainter>
#include <QSize>
#include <QVariantAnimation>
#include <QMessageBox>

struct MainMenuPrivate {
        QVariantAnimation opacityAnim;
    NavigationManager* nav;
};

MainMenu::MainMenu(QObject *parent) : AbstractMenu(parent) {
    d = new MainMenuPrivate();
    d->nav = new NavigationManager(this);

    d->opacityAnim.setStartValue(1.0);
    d->opacityAnim.setEndValue(1.0);
    d->opacityAnim.setDuration(250);
    d->opacityAnim.setEasingCurve(QEasingCurve::OutCubic);
    connect(&d->opacityAnim, &QVariantAnimation::valueChanged, this, &MainMenu::requestPaint);

    auto playButton = new NavigationButton(this);
    playButton->setText("Play");
    playButton->setCenterPoint(QPoint(400, 350));
    connect(playButton, &NavigationButton::activated, this, [this] {
        d->opacityAnim.setStartValue(1.0);
        d->opacityAnim.setEndValue(0.0);
        d->opacityAnim.start();

        emit startGame();
    });
    d->nav->addNavigationElement(playButton);

    auto optionsButton = new NavigationButton(this);
    optionsButton->setText("Options");
    optionsButton->setCenterPoint(QPoint(400, 400));
    d->nav->addNavigationElement(optionsButton);

    auto creditsButton = new NavigationButton(this);
    creditsButton->setText("Credits");
    creditsButton->setCenterPoint(QPoint(400, 450));
    connect(creditsButton, &NavigationButton::activated, this, [this, parent] {
        QMessageBox::critical(qobject_cast<QWidget*>(parent), "Credits", "Thanks to Royce551 for operating Squid Airlines\n\nMUSIC:\n\"Overworld\"\nKevin MacLeod (incompetech.com)\nLicensed under Creative Commons: By Attribution 3.0\nhttp://creativecommons.org/licenses/by/3.0/");
    });
    d->nav->addNavigationElement(creditsButton);

    auto quitButton = new NavigationButton(this);
    quitButton->setText("Quit");
    quitButton->setCenterPoint(QPoint(400, 500));
    connect(quitButton, &NavigationButton::activated, this, [] {
        QApplication::exit();
    });
    d->nav->addNavigationElement(quitButton);

    connect(d->nav, &NavigationManager::requestPaint, this, &MainMenu::requestPaint);
}

MainMenu::~MainMenu() {
    delete d;
}

void MainMenu::drawMenu(QPainter *painter, QSize size) {
    auto rect = this->middleRect(size);

    painter->save();
    painter->setOpacity(d->opacityAnim.currentValue().toDouble());

    painter->drawText(rect.left(), rect.top(), "Menu goes here");

    d->nav->paint(painter, rect);

    painter->restore();
}

void MainMenu::moveToNext() {
    d->nav->moveToNext();
}

void MainMenu::moveToPrev() {
    d->nav->moveToPrev();
}
void MainMenu::activate() {
    d->nav->activate();
}
void MainMenu::showAgain() {
    d->opacityAnim.setStartValue(0.0);
    d->opacityAnim.setEndValue(1.0);
    d->opacityAnim.start();
}
