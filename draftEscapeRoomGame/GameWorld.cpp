#include "GameWorld.h"
#include <QPixmap>
#include <QResizeEvent>
#include <QPushButton>
#include "LaptopPuzzle.h"
#include <QTimer>
#include <QMouseEvent>

// constructor
GameWorld::GameWorld(QWidget *parent)
    : QWidget(parent)
{
    // Background (room)
    roomLabel = new QLabel(this);
    roomLabel->setGeometry(0, 0, 800, 600);
    roomLabel->setScaledContents(true);

    // Light overlay
    overlay = new QLabel(this);
    overlay->setGeometry(0, 0, 800, 600);
    overlay->setStyleSheet("background-color: rgba(0,0,0,200);");
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Buttons
    leftButton = new QPushButton("<", this);
    rightButton = new QPushButton(">", this);

    // Position buttons
    leftButton->move(50, 500);
    rightButton->move(700, 500);

    // Connections
    connect(leftButton, &QPushButton::clicked, this, &GameWorld::onLeft);
    connect(rightButton, &QPushButton::clicked, this, &GameWorld::onRight);

    //laptop clue unlocked
    laptopMarker = new QLabel(this);
    laptopMarker->setGeometry(350, 300, 40, 40);
    laptopMarker->setStyleSheet("background-color: black;");
    laptopMarker->show();


    //laptop puzzle
    puzzle = new LaptopPuzzle(this);
    puzzle->hide();

    //go back from puzzle
    connect(puzzle, &LaptopPuzzle::backToRoom, this, [=]() {
        puzzle->hide();
        state = GameState::ROOM;
        updateView();
    });

    //puzzle solved
    connect(puzzle, &LaptopPuzzle::puzzleSolved, this, [=]() {

        puzzleStage = 1;

        messageLabel->setText("Puzzle 1 Completed! Lights on");
        messageLabel->show();

        //emit puzzleCompleted(1);


        QTimer::singleShot(2000, this, [=]() {
            messageLabel->hide();
        });

        QTimer *flicker = new QTimer(this);
        int *count = new int(0);

        connect(flicker, &QTimer::timeout, this, [=]() mutable {

            controller.toggleLight();
            updateView();

            (*count)++;

            if (*count > 5) {
                flicker->stop();
                controller.roomState = RoomState::LIGHT;
                updateView();
            }
        });

        flicker->start(200);
    });

    //message when puzzle solved
    messageLabel = new QLabel(this);
    messageLabel->setGeometry(20, 100, 300, 50);
    messageLabel->setStyleSheet("color: white; font-size: 18px; background: rgba(0,0,0,120);");
    messageLabel->hide();

    updateView();
}

//movement
void GameWorld::onLeft()
{
    controller.goLeft();
    updateView();
}

void GameWorld::onRight()
{
    controller.goRight();
    updateView();
}

//light


//update view
void GameWorld::updateView()
{
    QString image;

    switch (controller.currentWall) {
    case Wall::FRONT: image = ":/images/images/front_lp.png"; break;
    case Wall::LEFT:  image = ":/images/left_.png"; break;
    case Wall::RIGHT: image = ":/images/images/right.png"; break;
    case Wall::BACK:  image = ":/images/images/back.png"; break;
    }

    roomLabel->setPixmap(QPixmap(image));

    //light
    if (controller.roomState == RoomState::DARK)
        overlay->show();
    else
        overlay->hide();

    //shows when puzzle unlocked
    if (controller.currentWall == Wall::FRONT && puzzleStage == 0)
        laptopMarker->show();
    else
        laptopMarker->hide();

    //if (puzzleStage == 1)
        //doorMarker->show();

}

//game mode
void GameWorld::setGameMode(GameMode m)
{
    mode = m;
}


//resize
void GameWorld::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    roomLabel->setGeometry(0, 0, width(), height());
    overlay->setGeometry(0, 0, width(), height());

    leftButton->move(50, height() - 80);
    rightButton->move(width() - 100, height() - 80);

    laptopMarker->move(width()/2 - 20, height()/2 -20);
}

//laptop zoom in
void GameWorld::mousePressEvent(QMouseEvent *event)
{

    QWidget::mousePressEvent(event);


    if (state == GameState::ROOM)
    {
        if (controller.currentWall == Wall::FRONT &&
            laptopMarker->isVisible() &&
            laptopMarker->geometry().contains(event->pos()))
        {
            state = GameState::LAPTOP_ZOOM;

            roomLabel->setPixmap(QPixmap(":/images/images/laptop_closeup.png"));
            return;
        }
    }


    if (state == GameState::LAPTOP_ZOOM)
    {
        state = GameState::LAPTOP_TERMINAL;

        puzzle->setGeometry(
            (width() - 400) / 2,
            (height() - 300) / 2,
            400,
            300
            );

        puzzle->raise();
        puzzle->show();
        puzzle->setFocus();

        return;
    }

}

//message when puzzle solved
void GameWorld::showMessage(const QString &msg)
{
    messageLabel->setText(msg);
    messageLabel->show();

    QTimer::singleShot(2000, this, [=]() {
        messageLabel->hide();
    });
}
