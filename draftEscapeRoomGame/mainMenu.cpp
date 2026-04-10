#include "mainMenu.h"

mainMenu::mainMenu()
{
    setFixedSize(800, 600);
    setStyleSheet("background-color: grey;");

    titleLabel = new QLabel("ESCAPE ROOM", this);
    titleLabel->setStyleSheet("color: white; font-size: 40px;");
    titleLabel->adjustSize();
    titleLabel->move(250, 100);

    soloButton = new QPushButton("Play Solo", this);
    raceButton = new QPushButton("Race Mode", this);
    startButton = new QPushButton("Start", this);

    soloButton->move(300, 250);
    raceButton->move(300, 300);
    startButton->move(300, 400);

    startButton->hide();

    connect(soloButton, &QPushButton::clicked, this, [=]() {
        modeSelected = true;
        startButton->show();
    });

    connect(raceButton, &QPushButton::clicked, this, [=]() {
        modeSelected = true;
        startButton->show();
    });

    connect(startButton, &QPushButton::clicked, this, [=]() {
        emit startGame(); // tell main.cpp to start game
    });
}
