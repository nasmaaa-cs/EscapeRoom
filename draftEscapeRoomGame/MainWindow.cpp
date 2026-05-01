#include "MainWindow.h"
#include <QPixmap>
#include <QResizeEvent>
#include <QTimer>
#include <QStackedWidget>

#include "GameWorld.h"
#include "mainMenu.h"
#include "NetworkManager.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Setup the UI update timer
    uiTimer = new QTimer(this);
    connect(uiTimer, &QTimer::timeout, this, &MainWindow::updateTimerUI);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    main = new mainMenu(this);
    world = new GameWorld(this);

    stackedWidget->addWidget(main);
    stackedWidget->addWidget(world);

    stackedWidget->setCurrentIndex(0);

    connect(world, &GameWorld::backToMainMenu, this, [=]() {
        uiTimer->stop();
        stackedWidget->setCurrentIndex(0);
        timerLabel->hide();
        opponentLabel->hide();
    });

    connect(main, &mainMenu::StartGame, this, [=](GameMode mode, QString name, NetworkRole role, QString ip) {
        this->setPlayerName(name);
        this->startGame(mode, role, ip);
    });

    // Timer
    timerLabel = new QLabel("00:00", this);
    timerLabel->setStyleSheet("color: white; font-size: 20px;");
    timerLabel->move(20, 20);

    // opponent
    opponentLabel = new QLabel("Opponent: 00:00", this);
    opponentLabel->move(20, 50);
    opponentLabel->setFixedWidth(300);
    opponentLabel->setStyleSheet("color: red;");

    // Window setup
    this->resize(800, 600);
    this->setMinimumSize(800, 600);

    timerLabel->raise();
    opponentLabel->raise();
    timerLabel->hide();
    opponentLabel->hide();

    netManager = new NetworkManager(this);

    connect(netManager, &NetworkManager::opponentPuzzleSolved, this, [=](int puzzleId) {
        opponentLabel->setText(QString("Opponent: Solving Puzzle %1").arg(puzzleId + 1));
        opponentLabel->setStyleSheet("color: orange;");
    });

    connect(netManager, &NetworkManager::opponentFinished, this, [=](QString time) {
        opponentLabel->setText(QString("Opponent Finished: %1").arg(time));
        opponentLabel->setStyleSheet("color: red;");
    });

    connect(world, &GameWorld::puzzleCompleted, this, [=](int puzzleIndex) {
        if (mode == GameMode::RACE) {
            netManager->sendPuzzleSolved(puzzleIndex);
        }
    });

    connect(netManager, &NetworkManager::opponentConnected, this, [=]() {
        opponentLabel->setText("Opponent Joined! Race Started!");
        opponentLabel->setStyleSheet("color: green; font-weight: bold;");
    }, Qt::QueuedConnection);

    connect(world, &GameWorld::gameFinished, this, [=]() {
        uiTimer->stop();

        timerLabel->hide();
        opponentLabel->hide();

        QString finalTime = timerLabel->text();

        world->setFinalTimeDisplay(finalTime);
        if (mode == GameMode::RACE) {
            netManager->sendFinished(finalTime);
        }
    });
}

//game mode
void MainWindow::startGame(GameMode m, NetworkRole role, QString ip)
{
    timerLabel->setText("00:00");
    timerLabel->show();
    world->resetGame();

    mode = m;
    this->currentRole = role;
    this->targetIp = ip;
    raceStarted = false;

    stackedWidget->setCurrentIndex(1);

    if (mode == GameMode::RACE) {
        opponentLabel->show();
        opponentLabel->setText("Waiting for connection...");
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
        raceStarted = true;


        if (currentRole == NetworkRole::HOST) {
            netManager->hostGame(8080);
        } else {
            qDebug() << "Attempting to join IP:" << targetIp;
            QString connectIp = targetIp.isEmpty() ? "127.0.0.1" : targetIp;
            netManager->joinGame(connectIp.toStdString(), 8080);
        }
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

}

void MainWindow::setPlayerName(QString name)
{
    playerName = name;
}
