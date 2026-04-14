#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

class LaptopPuzzle : public QWidget
{
    Q_OBJECT

public:
    explicit LaptopPuzzle(QWidget *parent = nullptr);

signals:
    void puzzleSolved();
    void backToRoom();

private:
    QPushButton *backButton;
    QTextEdit *terminal;

    int currentQuestion = 0;

    void setupUI();
    void loadQuestion(int index);
    void checkAnswer(QString answer);
};
