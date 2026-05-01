#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QMovie>
#include <QGraphicsOpacityEffect>

#include "gameController.h"
#include "gameTypes.h"
#include "LaptopPuzzle.h"
#include "PrisonPuzzle.h"
#include "GirlPuzzle.h"
#include "DeskPuzzle.h"
#include "DoorPuzzle.h"

enum class GameState {
    ROOM,
    LAPTOP_ZOOM,
    LAPTOP_TERMINAL,
    PRISON_ZOOM,
    PRISON_TERMINAL,
    GIRL_ZOOM,
    GIRL_ZOOM2,
    GIRL_ZOOM3,
    DESK_ZOOM
};

class LaptopPuzzle;
class PrisonPuzzle;
class GirlPuzzle;
class DeskPuzzle;
class DoorPuzzle;

class GameWorld : public QWidget {
    Q_OBJECT

public:
    explicit GameWorld(QWidget *parent = nullptr);

    void setGameMode(GameMode m);
    void triggerGlitchShake(int durationMs, int intensity);
    void showEndScreen();
    void resetGame();
    void setFinalTimeDisplay(const QString &time);



signals:
    void backToMainMenu();
    void gameFinished();
    void puzzleCompleted(int puzzleIndex);

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    //GAME MODE
    GameMode mode;

    //GAME CONTROLLER
    GameController controller;


    //GAME STATE
    GameState state = GameState::ROOM;

    //ROOM UI
    QLabel *roomLabel;
    QLabel *overlay;
    QLabel *messageLabel;
    QLabel *effectOverlay;
    QLabel *glitchOverlay;
    QLabel *glitchLabel;
    QMovie *glitchMovie;
    QMovie *glitchError;
    QLabel *girl;
    QMovie *girll;
    QLabel *hologramDoor;
    QMovie *hologramMovie;
    QGraphicsOpacityEffect *hologramOpacity;

    QLabel *endMsg = nullptr;
    QPushButton *menuBtn = nullptr;


    //ROOM BUTTONS
    QPushButton *leftButton;
    QPushButton *rightButton;

    //marker for puzzles
    QLabel *laptopMarker;//for laptop puzzle
    QLabel *prisonMarker;//for prison puzzle
    QLabel *girlMarker;//for girl puzzle
    QLabel *deskMarker;//for desk puzzle
    QLabel *taskMarker;//for task file
    QLabel *cmakeMarker;//for CMakeLists.txt file
    QLabel *doorMarker;//for door puzzle

    //bool laptopUnlocked = true;
    //bool laptopSolved = false;

    LaptopPuzzle *puzzle;
    PrisonPuzzle *puzzle2;
    GirlPuzzle *puzzle3;
    DeskPuzzle *puzzle4;
    DoorPuzzle *puzzle5;

    int puzzleStage = 0;

    void updateView();

    void showMessage(const QString &msg);

    void typeMessage(const QString &fullText, int speedMs);

    //INPUT HANDLERS
    void onLeft();
    void onRight();
};
