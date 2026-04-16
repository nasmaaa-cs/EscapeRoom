#include "PrisonPuzzle.h"
#include <QVBoxLayout>
#include <QKeyEvent>

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

    terminal->append("PRISON SYSTEM v1.04");
    terminal->append("Type command after the prompt.");
    terminal->append("\n> "); // This is the initial prompt

    terminal->installEventFilter(this);



    backButton = new QPushButton("Back", this);

    layout->addWidget(terminal);
    layout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backToRoom();
        this->hide();
    });
}

bool PrisonPuzzle::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == terminal && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {

            QString allText = terminal->toPlainText();

            QStringList lines = allText.split('\n', Qt::SkipEmptyParts);

            if (!lines.isEmpty()) {
                QString lastLine = lines.last();

                if (lastLine.startsWith("> ")) {
                    lastLine = lastLine.mid(2);
                }

                handleCommand(lastLine.trimmed());
            }


            terminal->append("> ");

            return true;

        }
    }
    return QWidget::eventFilter(obj, event);
}

void PrisonPuzzle::handleCommand(QString input)
{

    QString cleanInput = input.trimmed().toLower();


    if (cleanInput.isEmpty()) return;

    if (step == 0)
    {
        if (cleanInput == "git init")
        {
            //terminal->append("\n> " + input);
            terminal->append("<font color='#00ff00'>✔ Repository initialized. System Unlocked.</font>\n"); //change when adding the other steps

            // Temporary
            emit puzzleSolved();
        }
        else
        {

            emit wrongAnswer();


            //terminal->append("\n> " + input);
            terminal->append("<font color='#ff0000'>[ERROR]: Unknown command.</font>");
            terminal->append("<font color='#ffff00'>HINT: You must initialize the prison terminal using 'git init'</font>");


            terminal->ensureCursorVisible();
        }
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
        }*/


