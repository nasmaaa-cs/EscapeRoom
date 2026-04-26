#include "DeskPuzzle.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>


DeskPuzzle::DeskPuzzle(QWidget *parent) : QWidget(parent)
{
    this->setGeometry(0, 0, 800, 600);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    //task file
    QLabel *taskLabel = new QLabel("Task: Translate Makefile to CMakeLists.txt\n\nNote: files are movable");
    taskLabel->setWordWrap(true);
    taskLabel->setStyleSheet("color: #999; font-size: 14px; border: none;");
    taskWindow = createPopup("task.txt", taskLabel, 5, 5, 220, 200);

    //Makfile file
    QLabel *makeLabel = new QLabel(
        "FLAGS = -Wall -std=c++17\n"
        "TARGET = escape\n\n"
        "$(TARGET) : main.o door.png\n"
        "\tg++ $(FLAGS) -o $(TARGET) main.o\n\n"
        "main.o : main.cpp\n"
        "\tg++ $(FLAGS) -c main.cpp\n\n"
        "# ASSET SYNC\n"
        "door.png :\n"
        "\t@echo 'Syncing asset: door.png...'"
        );
    makeLabel->setWordWrap(true);
    makeLabel->setStyleSheet("color: #33ff33; font-family: monospace; font-size: 15px; padding: 10px; border: none;");
    makefileWindow = createPopup("Makefile", makeLabel, 20, 180, 340, 400);

    //CMakeLists.txt window
    cmakeInput = new QTextEdit();
    cmakeInput->setStyleSheet("background: black; color: white; font-family: monospace; font-size: 16px; border: 1px solid #333;");


    QPushButton *btn = new QPushButton("COMPILE AND RUN");
    btn->setFixedHeight(50);
    btn->setStyleSheet("background: #004400; color: white; font-weight: bold; font-size: 15px; border: 1px solid #0f0;");
    connect(btn, &QPushButton::clicked, this, &DeskPuzzle::checkAnswer);

    QWidget *container = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(container);
    lay->setContentsMargins(5, 5, 5, 5);
    lay->addWidget(cmakeInput);
    lay->addWidget(btn);
    cmakeWindow = createPopup("CMakeLists.txt", container, 230, 20, 550, 550);



}

QWidget* DeskPuzzle::createPopup(const QString &title, QWidget *contentWidget, int x, int y, int w, int h)
{

    DraggableWidget *popup = new DraggableWidget(this->parentWidget());
    popup->setGeometry(x, y, w, h);
    popup->setStyleSheet("background: #1a1a1a; border: 3px solid #444; border-radius: 8px;");

    QVBoxLayout *layout = new QVBoxLayout(popup);
    layout->setContentsMargins(8, 4, 8, 8);


    QHBoxLayout *header = new QHBoxLayout();
    QLabel *t = new QLabel(title.toUpper());
    t->setStyleSheet("color: #0f0; font-family: monospace; font-weight: bold; border: none;");

    QPushButton *xBtn = new QPushButton("X");
    xBtn->setFixedSize(26, 26);
    xBtn->setStyleSheet("background: #aa0000; color: white; border-radius: 13px; font-weight: bold;");
    connect(xBtn, &QPushButton::clicked, popup, &QWidget::hide);

    header->addWidget(t);
    header->addStretch();
    header->addWidget(xBtn);

    layout->addLayout(header);
    layout->addWidget(contentWidget);

    popup->hide();
    return popup;
}

void DeskPuzzle::showTaskWindow() { taskWindow->show(); taskWindow->raise(); }
void DeskPuzzle::showMakefileWindow() { makefileWindow->show(); makefileWindow->raise(); }
void DeskPuzzle::showCMakeWindow() { cmakeWindow->show(); cmakeWindow->raise(); }

void DeskPuzzle::checkAnswer()
{
    QString answer = cmakeInput->toPlainText().toLower();
    QString errorMsg = "";

    //1: Minimum Version
    if (!answer.contains("cmake_minimum_required")) {
        errorMsg = "FATAL ERROR: SYSCALL FAILURE - 'cmake_minimum_required' NOT DEFINED.";
    }
    //2a: Project Name
    else if (!answer.contains("project(")) {
        errorMsg = "SYSTEM FAILURE: PROJECT IDENTIFIER NULL. 'project()' EXPECTED.";
    }

    //2b: Specific Project Name Check
    else if (!answer.contains("project(escape")) {
        errorMsg = "IDENTITY MISMATCH: PROJECT NAME MUST BE 'escape'.";
    }

    //3: C++ Standard (The 'set' command)
    else if (!answer.contains("set(") || (!answer.contains("17") && !answer.contains("c++17"))) {
        errorMsg = "KERNEL MISMATCH: C++ STANDARD NOT SPECIFIED OR INCORRECT. REQUIRE: 17.";
    }
    //4: add_executable (The Command itself)
    else if (!answer.contains("add_executable(")) {
        errorMsg = "LINKER ERROR: NO EXECUTABLE DEFINITION FOUND.";
    }
    //5: add_executable (The Target Name 'escape')
    else if (!answer.contains("add_executable(escape")) {
        errorMsg = "LINKER ERROR: TARGET NAME MISMATCH. 'escape' TARGET NOT FOUND.";
    }
    //6: Source Files (main.cpp and door.png)
    else if (!answer.contains("main.cpp") || !answer.contains("door.png")) {
        errorMsg = "COMPILATION ERROR: UNDEFINED REFERENCE - SOURCE FILES MISSING OR INVALID.";
    }

    if (errorMsg.isEmpty()) {
        // if all correct
        taskWindow->hide();
        makefileWindow->hide();
        cmakeWindow->hide();
        emit puzzleSolved();
    } else {
        // if incorrect
        emit wrongAnswer();
        showTerminalError(errorMsg);
    }
    }

void DeskPuzzle::showTerminalError(const QString &msg) {

    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setModal(false);

    msgBox->setText(msg);
    msgBox->setWindowTitle("SYSTEM ERROR");
    msgBox->setStyleSheet("background-color: white; color: red; font-family: monospace; border: 2px solid white;");

    msgBox->show();
}

