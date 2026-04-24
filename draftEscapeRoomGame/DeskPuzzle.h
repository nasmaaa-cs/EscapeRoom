#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QVector>


class DeskPuzzle : public QWidget
{
    Q_OBJECT

public:

    explicit DeskPuzzle(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void puzzleSolved();
    void backToRoom();
    void wrongAnswer();

private:

    int step = 0;

    QPushButton *backButton;

    //void handleCommand(QString input);


};
