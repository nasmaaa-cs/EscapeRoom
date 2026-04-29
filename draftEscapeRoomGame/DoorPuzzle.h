#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>

class DoorPuzzle : public QWidget
{
    Q_OBJECT

public:
    explicit DoorPuzzle(QWidget *parent = nullptr);
    void resetPuzzle();

signals:
    void puzzleSolved();
    void wrongAnswer();
    void backToRoom();

private slots:
    void handleExecute();
    void switchToCMake();
    void switchToCpp();
    void validateBoilerplate();

private:
    void setupUI();
    void updatePanelStyle(QString colorHex);
    bool eventFilter(QObject *obj, QEvent *event) override;

    QTextEdit *codeEditor;
    QLabel *statusDisplay;
    QLabel *instructionLabel;
    QLabel *cmakeNode;
    QLabel *cppNode;
    QPushButton *execButton;
    QPushButton *closeButton;

    QString currentFile;
    QString cmakeUserAdded;
    QString cppCode;

    const QString cmakeBoilerplate =
        "cmake_minimum_required(VERSION 3.21)\n"
        "project(escape)\n"
        "set(CMAKE_CXX_STANDARD 17)\n"
        "add_executable(escape main.cpp door.png)\n\n"
        "# --- ADD Qt6 COMPONENTS BELOW ---\n";
};
