#include "mainMenu.h"

mainMenu::mainMenu()
{
    resize(800, 600);
    setMinimumSize(800, 600);
    setStyleSheet("background-color: grey;");

    // Create widgets
    titleLabel = new QLabel("ESCAPE ROOM");
    titleLabel->setStyleSheet("color: white; font-size: 40px;");
    titleLabel->setAlignment(Qt::AlignCenter);

    soloButton = new QPushButton("Play Solo");
    raceButton = new QPushButton("Race Mode");
    startButton = new QPushButton("Start");

    startButton->hide();

    // Layout
    QVBoxLayout *layout = new QVBoxLayout();

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(40);

    layout->addWidget(soloButton);
    layout->addWidget(raceButton);
    layout->addSpacing(20);
    layout->addWidget(startButton);

    layout->addStretch();

    layout->setAlignment(Qt::AlignCenter);

    setLayout(layout);

    // Logic
    connect(soloButton, &QPushButton::clicked, this, [=]() {
        modeSelected = true;
        selectedMode = gameMode::SOLO;
        startButton->show();
    });

    connect(raceButton, &QPushButton::clicked, this, [=]() {
        modeSelected = true;
        selectedMode = gameMode::RACE;
        startButton->show();
    });

    connect(startButton, &QPushButton::clicked, this, [=]() {
        emit StartGame(selectedMode);
    });
}
