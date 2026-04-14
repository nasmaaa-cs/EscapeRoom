#include "LaptopPuzzle.h"
#include <QVBoxLayout>

LaptopPuzzle::LaptopPuzzle(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, 400, 300);

    setStyleSheet(R"(
        background-color: black;
        color: #00ff00;
        font-family: monospace;
        border: 2px solid #00ff00;
    )");

    setupUI();
    loadQuestion(0);
}

void LaptopPuzzle::setupUI()
{
    terminal = new QTextEdit(this);
    terminal->setGeometry(10, 50, 380, 200);
    terminal->setStyleSheet(
        "background-color:black; color:#00ff00;"
        );

    terminal->setReadOnly(false);

    backButton = new QPushButton("Back", this);
    backButton->setGeometry(10, 10, 80, 30);

    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backToRoom();
    });

    //input detection (temporary)
    connect(terminal, &QTextEdit::textChanged, this, [=]() {
        QString text = terminal->toPlainText();
        checkAnswer(text);
    });
}

void LaptopPuzzle::loadQuestion(int index)
{
    if (index == 0)
        terminal->setText("Create a ZERO-SHOT prompt:\n");
    else if (index == 1)
        terminal->setText("Create a FEW-SHOT prompt:\n");
    else if (index == 2)
        terminal->setText("Create a STEP-BY-STEP prompt:\n");
    else if (index == 3)
        terminal->setText("Combine all prompt types:\n");
}

void LaptopPuzzle::checkAnswer(QString answer)
{
    // placeholder logic
    if (answer.contains("done")) {

        currentQuestion++;

        if (currentQuestion >= 4) {
            emit puzzleSolved();
            return;
        }

        loadQuestion(currentQuestion);
    }
}
