#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QLineEdit>

#include "gameTypes.h"

class mainMenu : public QWidget {
    Q_OBJECT

public:
    mainMenu();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:

    void StartGame(GameMode mode, QString playerName);

private:
    QLabel *titleLabel;
    QPushButton *soloButton;
    QPushButton *raceButton;
    QPushButton *startButton;
    QLabel *background;

    QLineEdit *nameInput;

    bool modeSelected = false;

    GameMode selectedMode;
};
