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

class GameWorld : public QWidget {
    Q_OBJECT

public:
    explicit GameWorld(QWidget *parent = nullptr);

    void setGameMode(GameMode m);

protected:
    void resizeEvent(QResizeEvent *event) override;

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

    //ROOM BUTTONS
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *lightButton;
    QPushButton *laptopButton;

    void updateView();

    //INPUT HANDLERS
    void onLeft();
    void onRight();
    void onLight();
    void onLaptop();
};
