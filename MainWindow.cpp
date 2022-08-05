//
// Created by victo on 5/08/2022.
//

#include "MainWindow.h"

struct MainWindowPrivate {

};

MainWindow::MainWindow(QWidget *parent) {
    d = new MainWindowPrivate();
    this->resize(800, 600);
}

MainWindow::~MainWindow() {
    delete d;
}
