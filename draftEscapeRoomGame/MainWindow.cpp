#include "MainWindow.h"
#include <QPixmap>
#include <QResizeEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    // Background (room)
    roomLabel = new QLabel(central);
    roomLabel->setGeometry(0, 0, 800, 600);
    roomLabel->setScaledContents(true);

    // Light overlay
    overlay = new QLabel(central);
    overlay->setGeometry(0, 0, 800, 600);
    overlay->setStyleSheet("background-color: rgba(0,0,0,180);");
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Buttons
    leftButton = new QPushButton("<", central);
    rightButton = new QPushButton(">", central);
    lightButton = new QPushButton("Light", central);
    laptopButton = new QPushButton("Laptop", central);

    // Position buttons
    leftButton->move(50, 500);
    rightButton->move(700, 500);
    lightButton->move(350, 20);
    laptopButton->move(350, 300);

    // Connections
    connect(leftButton, &QPushButton::clicked, this, &MainWindow::onLeft);
    connect(rightButton, &QPushButton::clicked, this, &MainWindow::onRight);
    connect(lightButton, &QPushButton::clicked, this, &MainWindow::onLight);
    connect(laptopButton, &QPushButton::clicked, this, &MainWindow::onLaptop);

    // Window setup
    this->resize(800, 600);
    this->setMinimumSize(800, 600);

    // Timer
    timerLabel = new QLabel("00:00", this);
    timerLabel->setStyleSheet("color: white; font-size: 20px;");
    timerLabel->move(20, 20);
    opponentLabel->hide();

    // Set the mode explicitly for testing
    mode = GameMode::RACE;

    // Setup the UI update timer
    uiTimer = new QTimer(this);
    connect(uiTimer, &QTimer::timeout, this, &MainWindow::updateTimerUI);

    // Trigger the start sequence
    startGameTimer();

    updateView();


    // fake opponent
    opponentLabel = new QLabel("Opponent: 00:00", this);
    opponentLabel->move(20, 50);
    opponentLabel->setStyleSheet("color: red;");

    updateView();
}

// Controls
void MainWindow::onLeft() {
    controller.goLeft();
    updateView();
}

void MainWindow::onRight() {
    controller.goRight();
    updateView();
}

void MainWindow::onLight() {
    controller.toggleLight();
    updateView();
}

void MainWindow::onLaptop() {
    roomLabel->clear();
    roomLabel->setText("ZOOMED INTO LAPTOP\n(Puzzle coming soon)");
    roomLabel->setStyleSheet("color: white; font-size: 24px;");
}

// Update UI
void MainWindow::updateView() {

    QString image;

    switch (controller.currentWall) {
    case Wall::FRONT: image = ":/images/front.png"; break;
    case Wall::LEFT:  image = ":/images/left.png"; break;
    case Wall::RIGHT: image = ":/images/right.png"; break;
    case Wall::BACK:  image = ":/images/back.png"; break;
    }

    roomLabel->setStyleSheet(""); // reset style (important after laptop text)
    roomLabel->setPixmap(QPixmap(image));

    //  Light system
    if (controller.roomState == RoomState::DARK)
        overlay->show();
    else
        overlay->hide();

    //  Laptop only on FRONT wall
    if (controller.currentWall == Wall::FRONT)
        laptopButton->show();
    else
        laptopButton->hide();
}

// Handle resizing
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    roomLabel->setGeometry(0, 0, width(), height());
    overlay->setGeometry(0, 0, width(), height());

    // keep buttons in place relative to screen
    leftButton->move(50, height() - 80);
    rightButton->move(width() - 100, height() - 80);
    lightButton->move(width()/2 - 40, 20);
    laptopButton->move(width()/2 - 50, height()/2);
}

//Timer Starter

void MainWindow::startGameTimer()
{
    // Start the reference clocks
    gameTimer.start();

    if (mode == GameMode::RACE && !raceStarted) {
        raceStartTime.start();
        fakeOpponentTimer.start(); // Only start it here for sync
        raceStarted = true;
    }

    uiTimer->start(1000);
    updateTimerUI(); //manual call
}

void MainWindow::updateTimerUI()
{
    // solo or race mode main timer
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

    // if solo mode, hide opponent
    if (mode != GameMode::RACE) {
        opponentLabel->hide();
        return;
    }

    // if race mode, show opponent
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
