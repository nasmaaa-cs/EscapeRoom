#include "PrisonPuzzle.h"
#include <QVBoxLayout>

PrisonPuzzle::PrisonPuzzle(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(R"(
        background-color: black;
        color: #00ff00;
        font-family: monospace;
        border: 2px solid #00ff00;
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);

    terminal = new QTextEdit(this);
    terminal->setStyleSheet("background-color: black; color: #00ff00;");
    terminal->setText("Task 1: Initialize a Git repository\n");

    backButton = new QPushButton("Back", this);

    layout->addWidget(terminal);
    layout->addWidget(backButton);

    connect(terminal, &QTextEdit::textChanged, this, [=]() {
        QString text = terminal->toPlainText();

        if (text.endsWith("\n")) {
            QStringList lines = text.split("\n");
            QString lastCommand = lines[lines.size() - 2];

            handleCommand(lastCommand);
        }
    });

    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backToRoom();
        this->hide();
    });
}

void PrisonPuzzle::handleCommand(QString input)
{
    input = input.trimmed();

    if (step == 0)
    {
        if (input == "git init") {
            terminal->append("✔ Repository initialized.");
            //terminal->append("\nTask 2: Check repository status");
            //step++;
            emit puzzleSolved();
        } else {
            terminal->append(" Hint: git init");
        }
    }

    /*else if (step == 1)
    {
        if (input == "git status") {
            terminal->append("✔ No commits yet.");
            terminal->append("\nTask 3: Stage files");
            step++;
        } else {
            terminal->append(" Hint: git status");
        }
    }

    else if (step == 2)
    {
        if (input == "git add .") {
            terminal->append("✔ Files staged.");
            terminal->append("\nTask 4: Commit changes");
            step++;
        } else {
            terminal->append(" Hint: git add .");
        }
    }

    else if (step == 3)
    {
        if (input.startsWith("git commit")) {
            terminal->append("✔ Changes committed.");
            terminal->append("\nTask 5: Push");
            step++;
        } else {
            terminal->append("Hint: git commit -m \"msg\"");
        }
    }

    else if (step == 4)
    {
        if (input == "git push") {
            terminal->append(" Push failed: No SSH key.");
            terminal->append("\nTask 6: Generate SSH key");
            step++;
        } else {
            terminal->append("Hint: git push");
        }
    }

    else if (step == 5)
    {
        if (input == "ssh-keygen") {
            terminal->append("✔ SSH key generated!");
            terminal->append("\n Prison unlocked!");

            emit puzzleSolved();
        } else {
            terminal->append(" Hint: ssh-keygen");
        }
    }*/
}
