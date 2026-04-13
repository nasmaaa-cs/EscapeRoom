#include "GameWorld.h"
#include <QPixmap>
#include <QResizeEvent>
#include <QPushButton>

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
    overlay->setStyleSheet("background-color: rgba(0,0,0,180);");
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Buttons
    leftButton = new QPushButton("<", this);
    rightButton = new QPushButton(">", this);
    lightButton = new QPushButton("Light", this);
    laptopButton = new QPushButton("Laptop", this);

    // Position buttons
    leftButton->move(50, 500);
    rightButton->move(700, 500);
    lightButton->move(350, 20);
    laptopButton->move(350, 300);

    // Connections
    connect(leftButton, &QPushButton::clicked, this, &GameWorld::onLeft);
    connect(rightButton, &QPushButton::clicked, this, &GameWorld::onRight);
    connect(lightButton, &QPushButton::clicked, this, &GameWorld::onLight);
    connect(laptopButton, &QPushButton::clicked, this, &GameWorld::onLaptop);

    updateView();
}

//moement
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
void GameWorld::onLight()
{
    controller.toggleLight();
    updateView();
}

//laptop
void GameWorld::onLaptop()
{
    //roomLabel->setStyleSheet("color: white; font-size: 24px;");
}

//update view
void GameWorld::updateView()
{
    QString image;

    switch (controller.currentWall) {
    case Wall::FRONT: image = ":/images/images/front.png"; break;
    case Wall::LEFT:  image = ":/images/images/left.png"; break;
    case Wall::RIGHT: image = ":/images/images/right.png"; break;
    case Wall::BACK:  image = ":/images/images/back.png"; break;
    }

    roomLabel->setPixmap(QPixmap(image));

    if (controller.roomState == RoomState::DARK)
        overlay->show();
    else
        overlay->hide();

    if (controller.currentWall == Wall::FRONT)
        laptopButton->show();
    else
        laptopButton->hide();
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
    lightButton->move(width()/2 - 40, 20);
    laptopButton->move(width()/2 - 50, height()/2);
}
