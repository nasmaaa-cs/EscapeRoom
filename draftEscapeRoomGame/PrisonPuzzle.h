#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QVector>

class PrisonPuzzle : public QWidget
{
    Q_OBJECT

public:
    explicit PrisonPuzzle(QWidget *parent = nullptr);

signals:
    void puzzleSolved();
    void backToRoom();

private:
    QPushButton *backButton;
    QTextEdit *terminal;

    int step = 0;

    void handleCommand(QString input);

    //QLabel *questionLabel;
    //QVector<QPushButton*> choices;

    //int currentQuestion = 0;

    //void setupUI();

    //void loadQuestion();
    //void checkAnswer(int index);
};
