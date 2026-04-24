#include "GameWorld.h"
#include <QPixmap>
#include <QResizeEvent>
#include <QPushButton>
#include <QTimer>
#include <QMouseEvent>

#include "LaptopPuzzle.h"
#include "PrisonPuzzle.h"
#include "GirlPuzzle.h"


// constructor
GameWorld::GameWorld(QWidget *parent)
    : QWidget(parent)
{
    // Background (room)
    roomLabel = new QLabel(this);
    roomLabel->setGeometry(0, 0, 800, 600);
    roomLabel->setScaledContents(true);
    roomLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Light overlay
    overlay = new QLabel(this);
    overlay->setGeometry(0, 0, 800, 600);
    overlay->setStyleSheet("background-color: rgba(0,0,0,200);");
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents);

    //glitch effect
    glitchOverlay = new QLabel(this);
    glitchOverlay->setGeometry(0, 0, 800, 600);
    glitchOverlay->setScaledContents(true);
    glitchOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.4);
    glitchOverlay->setGraphicsEffect(opacityEffect);
    //glitchMovie = new QMovie(":/images/images/tv.gif");
    //glitchOverlay->setMovie(glitchMovie);
    //glitchMovie->start();
    //glitchOverlay->raise();


    //Cam layer
    effectOverlay = new QLabel(this);
    effectOverlay->setGeometry(0, 0, 800, 600);
    //effectOverlay->setPixmap(QPixmap(":/images/images/cam.png"));
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
    opacity->setOpacity(0.8);
    effectOverlay->setGraphicsEffect(opacity);
    effectOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    //effectOverlay->setScaledContents(true);
    //effectOverlay->raise();

    //error glitch
    glitchLabel = new QLabel(this);
    glitchLabel->setGeometry(0, 0, 800, 600);
    glitchLabel->setScaledContents(true);
    glitchLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    glitchError = new QMovie(":/images/images/errorrr.gif");
    QGraphicsOpacityEffect *opacity2 = new QGraphicsOpacityEffect(this);
    opacity2->setOpacity(0.2);
    glitchLabel->setGraphicsEffect(opacity2);
    glitchLabel->setMovie(glitchError);
    glitchLabel->hide();

    //girl vid
    girl = new QLabel(this);
    girl->setGeometry(0, 0, 800, 600);
    girl->setScaledContents(true);
    girl->setAttribute(Qt::WA_TransparentForMouseEvents);
    girll = new QMovie(":/images/images/girl_waving_left.gif");
    girl->setMovie(girll);
    girll->start();
    girl->hide();

    // Buttons
    leftButton = new QPushButton("<", this);
    leftButton->hide();
    rightButton = new QPushButton(">", this);
    rightButton->hide();

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

    //prison clue unlocked
    prisonMarker = new QLabel(this);
    prisonMarker->setGeometry(350, 300, 40, 40);
    prisonMarker->setStyleSheet("background-color: black;");
    prisonMarker->hide();

    prisonMarker->raise();
    prisonMarker->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    //girl clue unlocked
    girlMarker = new QLabel(this);
    girlMarker->setGeometry(350, 300, 40, 40);
    girlMarker->setStyleSheet("background-color: black;");
    girlMarker->hide();

    girlMarker->raise();
    girlMarker->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    //desk clue unlocked
    deskMarker = new QLabel(this);
    deskMarker->setGeometry(350, 300, 40, 40);
    deskMarker->setStyleSheet("background-color: black;");
    deskMarker->hide();

    deskMarker->raise();
    deskMarker->setAttribute(Qt::WA_TransparentForMouseEvents, true);


    //laptop puzzle
    puzzle = new LaptopPuzzle(this);
    puzzle->hide();

    //prison puzzle
    puzzle2 = new PrisonPuzzle(this);
    puzzle2->hide();

    //girl puzzle
    puzzle3 = new GirlPuzzle(this);
    puzzle3->hide();


    //go back from laptop puzzle
    connect(puzzle, &LaptopPuzzle::backToRoom, this, [=]() {
        puzzle->hide();
        state = GameState::ROOM;
        updateView();
    });

    //go back from prison puzzle
    connect(puzzle2, &PrisonPuzzle::backToRoom, this, [=]() {
        puzzle2->hide();
        state = GameState::ROOM;
        updateView();
    });

    //go back from girl puzzle
    connect(puzzle3, &GirlPuzzle::backToRoom, this, [=]() {
        puzzle3->hide();
        state = GameState::ROOM;
        updateView();
    });

    //laptop puzzle solved
    connect(puzzle, &LaptopPuzzle::puzzleSolved, this, [=]() {

        puzzleStage++;
        state = GameState::ROOM;
        puzzle->hide();

        messageLabel->setText("Puzzle 1 Completed! Lights on");
        messageLabel->setWordWrap(true);
        messageLabel->show();

        QTimer::singleShot(2000, this, [=]() {
            messageLabel->hide();
        });

        QTimer *flicker = new QTimer(this);
        int count = 0;

        connect(flicker, &QTimer::timeout, this, [=]() mutable {

            controller.toggleLight();
            updateView();

            count++;

            if (count > 5) {
                flicker->stop();
                controller.roomState = RoomState::LIGHT;
                updateView();
            }
        });

        flicker->start(200);
    });

    //prison puzzle solved
    connect(puzzle2, &PrisonPuzzle::puzzleSolved, this, [=]() {

        state = GameState::ROOM;
        puzzleStage++;

        puzzle2->hide();

        messageLabel->setText("Puzzle 2 Completed! Prison Unlocked");
        messageLabel->setWordWrap(true);
        messageLabel->show();

        QTimer::singleShot(2000, this, [=]() {
            messageLabel->hide();
        });


        prisonMarker->hide();

        updateView();
    });

    //girl puzzle solved
    connect(puzzle3, &GirlPuzzle::puzzleSolved, this, [=]() {

        state = GameState::ROOM;
        puzzleStage++;

        puzzle3->hide();

        messageLabel->setText("Puzzle 3 Completed!");
        messageLabel->setWordWrap(true);
        messageLabel->show();

        QTimer::singleShot(2000, this, [=]() {
            messageLabel->hide();
        });


        girlMarker->hide();

        updateView();
    });

    //wrong answer
\
    connect(puzzle, &LaptopPuzzle::wrongAnswer, this, [=]() {
        triggerGlitchShake(300, 10); // Shake for 300ms with 10px intensity
    });

    connect(puzzle2, &PrisonPuzzle::wrongAnswer, this, [=]() {
        triggerGlitchShake(300, 10);
    });

    connect(puzzle3, &GirlPuzzle::wrongAnswer, this, [=]() {
        triggerGlitchShake(300, 10);
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


//update view
void GameWorld::updateView()
{
    QString image;

    switch (controller.currentWall) {
    case Wall::FRONT: image = ":/images/images/front_lp.png"; break;

    case Wall::LEFT:
        if(puzzleStage <2)
            image = ":/images/images/left_.png";
        else
            image = ":/images/images/girl_freed.png";

        break;

    case Wall::RIGHT: image = ":/images/images/right.png"; break;
    case Wall::BACK:
        if(puzzleStage <3)
            image = ":/images/images/back_.png";
        else
            image = ":/images/images/files.png";
        break;
    }

    roomLabel->setPixmap(QPixmap(image));

    //light
    if (controller.roomState == RoomState::DARK)
        overlay->show();
    else
        overlay->hide();

    //shows when laptop puzzle unlocked
    if (controller.currentWall == Wall::FRONT && puzzleStage == 0)
        laptopMarker->show();
    else
        laptopMarker->hide();

    //shows when prison puzzle unlocked
        if (controller.currentWall == Wall::LEFT && puzzleStage == 1)
        prisonMarker->show();
    else
        prisonMarker->hide();

    //shows when girl puzzle unlocked
    if (controller.currentWall == Wall::LEFT && puzzleStage == 2)
        girlMarker->show();
    else
        girlMarker->hide();

    if (state == GameState::ROOM){
        leftButton->show();
        rightButton->show();}

    else {
        leftButton->hide();
        rightButton->hide();}

    if (state != GameState::GIRL_ZOOM) {
        girl->hide();}

    //shows when desk puzzle unlocked
    if (controller.currentWall == Wall::BACK && puzzleStage == 3)
        deskMarker->show();
    else
        deskMarker->hide();

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

    effectOverlay->setGeometry(0, 0, width(), height());
    glitchOverlay->setGeometry(0, 0, width(), height());

    glitchLabel->setGeometry(0, 0, width(), height());

    girl->setGeometry(0, 0, width(), height());

    leftButton->move(50, (height() * 0.6 - 80));
    rightButton->move(width() - 100, (height() * 0.6 - 80));

    laptopMarker->move(width()/2 - 20, height()/2 -20);

    int x = width()/2 + 80;
    int y = height()/2 + 60;

    prisonMarker->setGeometry( x, y, 40, 40);

    girlMarker->move(width()/2 - 20, height()/2 -20);

    deskMarker->move(width()/2 -20, (height() * 0.68 - 80));
}

//zoom in
void GameWorld::mousePressEvent(QMouseEvent *event)
{

    QWidget::mousePressEvent(event);

    // laptop
    if (state == GameState::ROOM &&
        controller.currentWall == Wall::FRONT &&
        laptopMarker->isVisible() &&
        laptopMarker->geometry().contains(event->pos()))
    {
        state = GameState::LAPTOP_ZOOM;
        updateView();
        roomLabel->setPixmap(QPixmap(":/images/images/laptop_closeup.png"));
        return;
    }

    if (state == GameState::LAPTOP_ZOOM)
    {
        state = GameState::LAPTOP_TERMINAL;

        puzzle->setGeometry((width() - 400)/2, (height() - 300)/2, 400, 300);
        puzzle->raise();
        puzzle->show();
        puzzle->setFocus();

        return;
    }

    // prison

    if (state == GameState::ROOM &&
        controller.currentWall == Wall::LEFT &&
        prisonMarker->isVisible() &&
        prisonMarker->geometry().contains(event->pos()))
    {
        state = GameState::PRISON_ZOOM;
        updateView();
        roomLabel->setPixmap(QPixmap(":/images/images/prison_closeup.png"));
        return;
    }

    if (state == GameState::PRISON_ZOOM)
    {
        state = GameState::PRISON_TERMINAL;

        puzzle2->setGeometry((width() - 400)/2, (height() - 300)/2, 400, 300);
        puzzle2->raise();
        puzzle2->show();
        puzzle2->setFocus();

        return;
    }

    // girl

    if (state == GameState::ROOM &&
        controller.currentWall == Wall::LEFT &&
        girlMarker->isVisible() &&
        girlMarker->geometry().contains(event->pos()))
    {
        state = GameState::GIRL_ZOOM;
        roomLabel->setPixmap(QPixmap(":/images/images/girl_side.png"));

        //typeMessage("Thank you for helping me...", 50);

        return;
    }

    if (state == GameState::GIRL_ZOOM)
    {
        state = GameState::GIRL_ZOOM2;


        updateView();
        roomLabel->setPixmap(QPixmap(":/images/images/girl_closeup_.png"));




        //typeMessage("I can help you get out," 50);

        return;
    }

    if (state == GameState::GIRL_ZOOM2)
    {
        state = GameState::GIRL_ZOOM3;
        roomLabel->setPixmap(QPixmap(":/images/images/girl_closeup_.png"));

        //typeMessage("I don't have permission to read, write, or execute... Can you give me access?", 50);

        puzzle3->setGeometry((width() - 400)/2, (height() - 300)/2, 400, 300);
        puzzle3->raise();
        puzzle3->show();
        puzzle3->setFocus();


        return;
    }

    //desk
    if (state == GameState::ROOM &&
        controller.currentWall == Wall::BACK &&
        deskMarker->isVisible() &&
        deskMarker->geometry().contains(event->pos()))
    {
        state = GameState::DESK_ZOOM;
        updateView();
        roomLabel->setPixmap(QPixmap(":/images/images/desk_closeup_.png"));
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

//effect
void GameWorld::triggerGlitchShake(int durationMs, int intensity)
{
    glitchLabel->show();
    glitchLabel->raise();
    glitchError->start();

    QWidget* activePuzzle = nullptr;
    if (state == GameState::LAPTOP_TERMINAL) activePuzzle = puzzle;
    else if (state == GameState::PRISON_TERMINAL) activePuzzle = puzzle2;
    else if (state == GameState::GIRL_ZOOM3) activePuzzle = puzzle3;

    QPoint roomOldPos = roomLabel->pos();
    QPoint puzzleOldPos = activePuzzle ? activePuzzle->pos() : QPoint(0,0);

    QTimer* shakeTimer = new QTimer(this);
    int elapsed = 0;

    connect(shakeTimer, &QTimer::timeout, this, [=]() mutable {
        if (elapsed < durationMs) {
            int dx = (std::rand() % (intensity * 2)) - intensity;
            int dy = (std::rand() % (intensity * 2)) - intensity;

            // Shake the background
            roomLabel->move(roomOldPos.x() + dx, roomOldPos.y() + dy);

            // Shake the puzzle if it's open
            if (activePuzzle) {
                activePuzzle->move(puzzleOldPos.x() + dx, puzzleOldPos.y() + dy);
            }

            elapsed += 20;
        } else {
            // Reset everything
            roomLabel->move(roomOldPos);
            if (activePuzzle) activePuzzle->move(puzzleOldPos);

            glitchError->stop();
            glitchLabel->hide();

            shakeTimer->stop();
            shakeTimer->deleteLater();
        }
    });
    shakeTimer->start(20);
}

//typing effect
void GameWorld::typeMessage(const QString &fullText, int speedMs)
{
    messageLabel->setText("");
    messageLabel->show();


    static int charIndex = 0;
    charIndex = 0;

    QTimer *typeTimer = new QTimer(this);

    connect(typeTimer, &QTimer::timeout, this, [=]() mutable {
        if (charIndex < fullText.length()) {
            messageLabel->setText(fullText.left(charIndex + 1));
            charIndex++;

        } else {
            typeTimer->stop();
            typeTimer->deleteLater();


            QTimer::singleShot(3000, messageLabel, &QLabel::hide);
        }
    });

    typeTimer->start(speedMs); // Lower is faster
}
