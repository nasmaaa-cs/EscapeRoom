#include "MainWindow.h"
#include <QPixmap>
#include <QResizeEvent>
#include <QTimer>
#include "GameWorld.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    world = new GameWorld(this);
    setCentralWidget(world);

    // Timer
    timerLabel = new QLabel("00:00", this);
    timerLabel->setStyleSheet("color: white; font-size: 20px;");
    timerLabel->move(20, 20);

    // fake opponent
    opponentLabel = new QLabel("Opponent: 00:00", this);
    opponentLabel->move(20, 50);
    opponentLabel->setStyleSheet("color: red;");

    // Window setup
    this->resize(800, 600);
    this->setMinimumSize(800, 600);

    // Setup the UI update timer
    uiTimer = new QTimer(this);
    connect(uiTimer, &QTimer::timeout, this, &MainWindow::updateTimerUI);
}

//game mode
void MainWindow::startGame(GameMode m)
{
    mode = m;
    raceStarted = false;

    if (mode == GameMode::RACE) {
        opponentLabel->show();
    } else {
        opponentLabel->hide();
    }

    startGameTimer();
}

//timer starter
void MainWindow::startGameTimer()
{
    gameTimer.start();

    if (mode == GameMode::RACE && !raceStarted) {
        raceStartTime.start();
        fakeOpponentTimer.start();
        raceStarted = true;
    }

    uiTimer->start(1000);
    updateTimerUI();
}

//timer update
void MainWindow::updateTimerUI()
{
    qint64 ms = (mode == GameMode::RACE)
                    ? raceStartTime.elapsed()
                    : gameTimer.elapsed();

    int seconds = (ms / 1000) % 60;
    int minutes = (ms / 60000);

    timerLabel->setText(
        QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
        );

    if (mode != GameMode::RACE) {
        opponentLabel->hide();
        return;
    }

    opponentLabel->show();

    qint64 op = fakeOpponentTimer.elapsed();

    int os = (op / 1000) % 60;
    int om = (op / 60000);

    opponentLabel->setText(
        QString("Opponent: %1:%2")
            .arg(om, 2, 10, QChar('0'))
            .arg(os, 2, 10, QChar('0'))
        );
}

void MainWindow::setPlayerName(QString name)
{
    playerName = name;
}
