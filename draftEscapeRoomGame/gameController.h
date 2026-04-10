#pragma once

enum class GameMode {
    SOLO,
    RACE
};

enum class Wall {
    FRONT,
    RIGHT,
    BACK,
    LEFT
};

enum class RoomState {
    DARK,
    LIGHT
};

class GameController {
public:
    Wall currentWall = Wall::FRONT;
    RoomState roomState = RoomState::DARK;

    void goLeft();
    void goRight();
    void toggleLight();
};
