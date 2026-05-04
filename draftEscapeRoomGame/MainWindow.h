#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include <QStackedWidget>
#include "gameTypes.h"
#include "NetworkManager.h"

class GameWorld;
class mainMenu;
class NetworkManager;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void startGameTimer();
    void updateTimerUI();

    void startGame(GameMode m, NetworkRole role, QString ip);
    void setPlayerName(QString name);

public slots:
    void handleNetworkError(const QString& message);

private:
    NetworkManager *netManager;

    QStackedWidget *stackedWidget;
    mainMenu *main;

    //GAME MODE
    GameMode mode;

    NetworkRole currentRole;
    QString targetIp;

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
