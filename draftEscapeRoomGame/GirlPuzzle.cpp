#include "GirlPuzzle.h"
#include <QVBoxLayout>
#include <QKeyEvent>

GirlPuzzle::GirlPuzzle(QWidget *parent)
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
        terminal->setText("Enter the command to view permissions\n");

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

bool GirlPuzzle::eventFilter(QObject *obj, QEvent *event)
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

void GirlPuzzle::handleCommand(QString input)
    {

        QString cleanInput = input.trimmed().toLower();


        if (cleanInput.isEmpty()) return;

        if (step == 0)
        {
            if (cleanInput == "ls")
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
