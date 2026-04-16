#include "LaptopPuzzle.h"
#include <QVBoxLayout>
#include <QTimer>

LaptopPuzzle::LaptopPuzzle(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, 500, 300);

    setStyleSheet(R"(
        background-color: black;
        color: #00ff00;
        font-family: monospace;
    )");

    //QUESTION LABEL
    questionLabel = new QLabel(this);
    questionLabel->setGeometry(20, 20, 460, 60);
    questionLabel->setStyleSheet("color: #00ff00;");

    //BACK BUTTON
    backButton = new QPushButton("Back", this);
    backButton->setGeometry(10, 10, 80, 30);

    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backToRoom();
        hide();
    });

    //MULTIPLE CHOICE BUTTONS
    for (int i = 0; i < 3; i++) {
        QPushButton *btn = new QPushButton(this);
        btn->setGeometry(20, 100 + i * 60, 460, 40);
        choices.push_back(btn);

        connect(btn, &QPushButton::clicked, this, [=]() {
            checkAnswer(i);
        });
    }

    loadQuestion();
}

//draft question for test
void LaptopPuzzle::loadQuestion()
{
    if (currentQuestion == 0) {
        questionLabel->setText("Q1: What is Zero-shot prompting?");

        choices[0]->setText("Prompt with no examples");   // correct
        choices[1]->setText("Prompt with many examples");
        choices[2]->setText("Step by step reasoning");
    }

    else if (currentQuestion == 1) {
        questionLabel->setText("Q2: Few-shot means?");

        choices[0]->setText("No examples");
        choices[1]->setText("Some examples");  // correct
        choices[2]->setText("Only output");
    }

    else if (currentQuestion == 2) {
        questionLabel->setText("Q3: Chain-of-thought means?");

        choices[0]->setText("Random guessing");
        choices[1]->setText("Step-by-step reasoning"); // correct
        choices[2]->setText("No reasoning");
    }

    else if (currentQuestion == 3) {
        questionLabel->setText("Q4: Best prompt type for reasoning?");

        choices[0]->setText("Chain-of-thought"); // correct
        choices[1]->setText("Zero-shot only");
        choices[2]->setText("No prompt");
    }
}

void LaptopPuzzle::checkAnswer(int index)
{
    bool correct = false;

    switch (currentQuestion) {
    case 0: correct = (index == 0); break;
    case 1: correct = (index == 1); break;
    case 2: correct = (index == 1); break;
    case 3: correct = (index == 0); break;
    }

    if (!correct) {
        emit wrongAnswer();

        // Optional: Make the button flash red so they know which one was wrong
        choices[index]->setStyleSheet("background-color: #550000; color: #ff0000;");
        QTimer::singleShot(200, this, [=]() {
            choices[index]->setStyleSheet("background-color: black; color: #00ff00;");
        });

        return;
    }

    currentQuestion++;

    if (currentQuestion == 4) {
        emit puzzleSolved();
        hide();
        return;
    }

    loadQuestion();
}
