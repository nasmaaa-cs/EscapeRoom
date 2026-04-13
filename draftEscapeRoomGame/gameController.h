#pragma once
#include "gameTypes.h"

class GameController {
public:
    Wall currentWall = Wall::FRONT;
    RoomState roomState = RoomState::DARK;

    void goLeft();
    void goRight();
    void toggleLight();
};
