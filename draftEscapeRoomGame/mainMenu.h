#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "gameMode.h"
#include <QResizeEvent>

class mainMenu : public QWidget {
    Q_OBJECT

public:
    mainMenu();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:

    void StartGame(gameMode mode);

private:
    QLabel *titleLabel;
    QPushButton *soloButton;
    QPushButton *raceButton;
    QPushButton *startButton;
    QLabel *background;

    bool modeSelected = false;

    gameMode selectedMode;
};
