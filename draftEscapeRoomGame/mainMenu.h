#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class mainMenu : public QWidget {
    Q_OBJECT

public:
    mainMenu();

signals:
    void startGame(); // signal to launch game

private:
    QLabel *titleLabel;
    QPushButton *soloButton;
    QPushButton *raceButton;
    QPushButton *startButton;

    bool modeSelected = false;
};
