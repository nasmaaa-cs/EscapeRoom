#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QStringList>

class GirlPuzzle : public QWidget
{
    Q_OBJECT

public:
    explicit GirlPuzzle(QWidget *parent = nullptr);

signals:
    void backToRoom();
    void puzzleSolved();
    void wrongAnswer();

private slots:
    void onCommandEntered();

private:
    void handleCommand(QString input);

    // UI Elements
    QTreeWidget *fileBrowser;
    QTextEdit *terminalOutput;
    QLineEdit *terminalInput;
    QPushButton *backButton;
    QTreeWidgetItem *playerDir;

    // Puzzle State Variables
    int step = 0;
    bool dirCreated = false;
    int filesCopiedCount = 0;

    // Updated file list
    QStringList requiredFiles = {"CMakeLists.txt", "g++", "Makefile"};
};
