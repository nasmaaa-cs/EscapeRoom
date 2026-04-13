#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QResizeEvent>
#include "gameTypes.h"

class mainMenu : public QWidget {
    Q_OBJECT

public:
    mainMenu();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:

    void StartGame(GameMode mode);

private:
    QLabel *titleLabel;
    QPushButton *soloButton;
    QPushButton *raceButton;
    QPushButton *startButton;
    QLabel *background;

    bool modeSelected = false;

    GameMode selectedMode;
};
