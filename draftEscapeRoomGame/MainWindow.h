#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "gameController.h"
#include <QTimer>
#include <QElapsedTimer>
#include "gameMode.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void startGameTimer();
    void updateTimerUI();

    void setGameMode(gameMode m);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    GameController controller;

    //game mode
    gameMode mode;
    bool raceStarted = false;
    QElapsedTimer raceStartTime;

    //fake second player
    QElapsedTimer fakeOpponentTimer;
    QLabel *opponentLabel;


    QLabel *roomLabel;
    QLabel *overlay;

    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *lightButton;
    QPushButton *laptopButton;

    //timer
    QTimer *uiTimer;
    QElapsedTimer gameTimer;
    QLabel *timerLabel;


    void updateView();

    void onLeft();
    void onRight();
    void onLight();
    void onLaptop();
};

