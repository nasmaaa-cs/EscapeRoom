#include "mainMenu.h"

mainMenu::mainMenu()
{
    resize(800, 600);
    setMinimumSize(800, 600);

    //background
    background = new QLabel(this);
    background->setPixmap(QPixmap(":/images/images/mainMenuBG.png"));
    background->setScaledContents(true);
    background->setGeometry(0, 0, width(), height());
    background->lower();

    // Create widgets
    titleLabel = new QLabel("ESCAPE ROOM");
    titleLabel->setStyleSheet("color: white; font-size: 40px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "background-color: rgba(0,0,0,150); color: white; padding: 10px;"
        );

    soloButton = new QPushButton("Play Solo");
    raceButton = new QPushButton("Race Mode");
    startButton = new QPushButton("Start");
    soloButton->setStyleSheet(
        "background-color: rgba(0,0,0,150); color: white; padding: 10px;"
        );
    raceButton->setStyleSheet(
        "background-color: rgba(0,0,0,150); color: white; padding: 10px;"
        );

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
        selectedMode = GameMode::SOLO;
        startButton->show();
    });

    connect(raceButton, &QPushButton::clicked, this, [=]() {
        modeSelected = true;
        selectedMode = GameMode::RACE;
        startButton->show();
    });

    connect(startButton, &QPushButton::clicked, this, [=]() {
        emit StartGame(selectedMode);
    });
}
void mainMenu::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    background->setGeometry(0, 0, width(), height());
}
