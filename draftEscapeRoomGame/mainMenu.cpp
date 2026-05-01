#include "mainMenu.h"

mainMenu::mainMenu(QWidget *parent) : QWidget(parent)
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

    //Name Box
    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Enter your name");

    nameInput->setStyleSheet(
        "background-color: rgba(0,0,0,150); color: white; padding: 10px;"
        );

    //ip input
    ipInput = new QLineEdit();
    ipInput->setPlaceholderText("Opponent IP (e.g. 127.0.0.1)");
    ipInput->setFixedSize(250, 40);
    ipInput->setStyleSheet("background-color: rgba(0,0,0,150); color: white; padding: 10px;");
    ipInput->hide();

    soloButton = new QPushButton("Play Solo");
    raceButton = new QPushButton("Race Mode");
    startButton = new QPushButton("Start");
    hostButton = new QPushButton("Host Game", this);
    joinButton = new QPushButton("Join Game", this);
    soloButton->setStyleSheet(
        "background-color: rgba(0,0,0,150); color: white; padding: 10px;"
        );
    raceButton->setStyleSheet(
        "background-color: rgba(0,0,0,150); color: white; padding: 10px;"
        );
    hostButton->setFixedSize(200, 50);
    joinButton->setFixedSize(200, 50);
    hostButton->setStyleSheet(raceButton->styleSheet());
    joinButton->setStyleSheet(raceButton->styleSheet());
    hostButton->hide();
    joinButton->hide();
    soloButton->setFixedSize(200, 50);
    raceButton->setFixedSize(200, 50);
    startButton->setFixedSize(200, 50);
    nameInput->setFixedSize(250, 40);


    startButton->hide();

    // Layout
    QVBoxLayout *layout = new QVBoxLayout();

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(40);

    layout->addWidget(nameInput, 0, Qt::AlignCenter);
    layout->addWidget(soloButton, 0, Qt::AlignCenter);
    layout->addWidget(raceButton, 0, Qt::AlignCenter);
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    layout->addWidget(hostButton, 0, Qt::AlignCenter);
    layout->addWidget(joinButton, 0, Qt::AlignCenter);
    layout->addWidget(ipInput, 0, Qt::AlignCenter);

    layout->addSpacing(20);


    layout->addStretch();

    layout->setAlignment(Qt::AlignCenter);

    setLayout(layout);

    // Logic
    connect(soloButton, &QPushButton::clicked, this, [=]() {
        modeSelected = true;
        selectedMode = GameMode::SOLO;
        hostButton->hide();
        joinButton->hide();
        ipInput->hide();
        startButton->show();
    });

    connect(raceButton, &QPushButton::clicked, this, [=]() {
        modeSelected = true;
        selectedMode = GameMode::RACE;
        hostButton->show();
        joinButton->show();
    });

    connect(hostButton, &QPushButton::clicked, this, [=]() {
        selectedRole = NetworkRole::HOST;
        ipInput->hide();
        startButton->show();
    });

    connect(joinButton, &QPushButton::clicked, this, [=]() {
        selectedRole = NetworkRole::JOIN;
        ipInput->show();
        startButton->show();
    });

    connect(startButton, &QPushButton::clicked, this, [=]() {

        if (nameInput->text().isEmpty()) {
            nameInput->setPlaceholderText("Please enter your name!");
            return;
        }

        emit StartGame(selectedMode, nameInput->text(), selectedRole, ipInput->text());
    });
}
void mainMenu::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    background->setGeometry(0, 0, width(), height());
}
