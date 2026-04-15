#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include "gameTypes.h"

class GameWorld;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void startGameTimer();
    void updateTimerUI();

    void startGame(GameMode m);
    void setPlayerName(QString name);


private:

    //GAME MODE
    GameMode mode;

    //PlAYER NAME
    QString playerName;


    //GAME WORLD
    GameWorld *world;

    bool raceStarted = false;

    //TIMERS
    QTimer *uiTimer;
    QElapsedTimer gameTimer;
    QElapsedTimer raceStartTime;

    //FAKE OPPONENT (RACE MODE ONLY)
    QElapsedTimer fakeOpponentTimer;
    QLabel *opponentLabel;

    QLabel *timerLabel;
};
