#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>

#include "gameController.h"
#include "gameTypes.h"

enum class GameState {
    ROOM,
    LAPTOP_ZOOM,
    LAPTOP_TERMINAL
};

class LaptopPuzzle;

class GameWorld : public QWidget {
    Q_OBJECT

public:
    explicit GameWorld(QWidget *parent = nullptr);

    void setGameMode(GameMode m);


signals:
    //void puzzleCompleted(int puzzleNumber);

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

    //ROOM BUTTONS
    QPushButton *leftButton;
    QPushButton *rightButton;
    QLabel *laptopMarker;

    //bool laptopUnlocked = true;
    //bool laptopSolved = false;

    LaptopPuzzle *puzzle;

    int puzzleStage = 0;

    void updateView();

    void showMessage(const QString &msg);

    //INPUT HANDLERS
    void onLeft();
    void onRight();
};
