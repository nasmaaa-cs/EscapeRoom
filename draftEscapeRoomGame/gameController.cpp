#include "gameController.h"

void GameController::goRight() {
    currentWall = static_cast<Wall>((static_cast<int>(currentWall) + 1) % 4);
}

void GameController::goLeft() {
    currentWall = static_cast<Wall>((static_cast<int>(currentWall) + 3) % 4);
}

void GameController::toggleLight() {
    if (roomState == RoomState::DARK)
        roomState = RoomState::LIGHT;
    else
        roomState = RoomState::DARK;
}
