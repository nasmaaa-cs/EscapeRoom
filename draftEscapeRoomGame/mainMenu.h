#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "gameMode.h"

class mainMenu : public QWidget {
    Q_OBJECT

public:
    mainMenu();

signals:

    void StartGame(gameMode mode);

private:
    QLabel *titleLabel;
    QPushButton *soloButton;
    QPushButton *raceButton;
    QPushButton *startButton;

    bool modeSelected = false;

    gameMode selectedMode;
};
