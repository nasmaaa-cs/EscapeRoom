#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QVector>

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
    //QTextEdit *terminal;

    QLabel *questionLabel;
    QVector<QPushButton*> choices;

    int currentQuestion = 0;

    void setupUI();

    void loadQuestion();
    void checkAnswer(int index);
};
