//
// Created by victo on 5/08/2022.
//

#include <QOpenGLWidget>

#ifndef SQUIDAIRLINESTHEGAME_MAINWINDOW_H
#define SQUIDAIRLINESTHEGAME_MAINWINDOW_H

struct MainWindowPrivate;
class MainWindow : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    MainWindowPrivate* d;
};


#endif//SQUIDAIRLINESTHEGAME_MAINWINDOW_H
