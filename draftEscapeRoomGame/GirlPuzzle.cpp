#include "GirlPuzzle.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

GirlPuzzle::GirlPuzzle(QWidget *parent)
    : QWidget(parent), playerDir(nullptr), step(0), dirCreated(false), filesCopiedCount(0)
{
    setStyleSheet(R"(
        background-color: black;
        font-family: monospace;
    )");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);


    fileBrowser = new QTreeWidget(this);
    fileBrowser->setHeaderLabel("Girl's directory");


    fileBrowser->setMinimumWidth(220);

    fileBrowser->setStyleSheet(R"(
        QTreeWidget {
            background-color: #00ff00;
            color: black;
            border: 2px solid black;
            font-size: 14px;
        }
        QHeaderView::section {
            background-color: black;
            color: #00ff00;
            border: 1px solid #00ff00;
            font-weight: bold;
        }
    )");

    for (const QString& fileName : requiredFiles) {
        QTreeWidgetItem *item = new QTreeWidgetItem(fileBrowser);
        item->setText(0, fileName);
    }

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(0);


    terminalOutput = new QTextEdit(this);
    terminalOutput->setReadOnly(true);

    terminalOutput->setStyleSheet(R"(
        QTextEdit {
            background-color: black;
            color: #00ff00;
            border: 2px solid #00ff00;
            border-bottom: none;
            font-size: 14px;
            padding: 5px;
        }
    )");
    terminalOutput->append("GIRL SYSTEM v1.04");
    terminalOutput->append("OBJECTIVE: Create a directory named 'MyDirectory' and copy all files into it.\n");


    QWidget *inputContainer = new QWidget(this);

    inputContainer->setStyleSheet(R"(
        QWidget {
            background-color: black;
            border: 2px solid #00ff00;
            border-top: none;
        }
    )");

    QHBoxLayout *inputLayout = new QHBoxLayout(inputContainer);
    inputLayout->setContentsMargins(5, 5, 5, 5);

    QLabel *promptLabel = new QLabel("> ", inputContainer);
    promptLabel->setStyleSheet("color: #00ff00; font-weight: bold; border: none; font-size: 16px;");

    terminalInput = new QLineEdit(inputContainer);
    terminalInput->setStyleSheet("background-color: black; color: #00ff00; border: none; outline: none; font-size: 16px;");

    inputLayout->addWidget(promptLabel);
    inputLayout->addWidget(terminalInput);


    backButton = new QPushButton("Back", this);
    backButton->setStyleSheet("color: #00ff00; border: 2px solid #00ff00; padding: 5px; margin-top: 15px; background-color: black;");


    rightLayout->addWidget(terminalOutput);
    rightLayout->addWidget(inputContainer);
    rightLayout->addWidget(backButton);

    mainLayout->addWidget(fileBrowser, 2);
    mainLayout->addLayout(rightLayout, 2);

    connect(terminalInput, &QLineEdit::returnPressed, this, &GirlPuzzle::onCommandEntered);

    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backToRoom();
        this->hide();
    });

    terminalInput->setFocus();
}

void GirlPuzzle::onCommandEntered()
{
    QString input = terminalInput->text();
    if (input.trimmed().isEmpty()) return;

    terminalOutput->append("> " + input);
    terminalInput->clear();
    handleCommand(input);
}

void GirlPuzzle::handleCommand(QString input)
{
    QStringList args = input.trimmed().split(" ", Qt::SkipEmptyParts);
    if (args.isEmpty()) return;

    QString command = args[0].toLower();


    if (!dirCreated) {
        if (command == "mkdir" && args.size() == 2) {
            QString newDirName = args[1];

            if (newDirName == "MyDirectory") {
                dirCreated = true;
                terminalOutput->append("<font color='#00ff00'>✔ Directory 'MyDirectory' created successfully.</font>");

                playerDir = new QTreeWidgetItem(fileBrowser);
                playerDir->setText(0, "MyDirectory/");
                playerDir->setForeground(0, QBrush(Qt::darkRed));
                playerDir->setExpanded(true);
            } else {
                terminalOutput->append("<font color='#ffff00'>[HINT]: Try naming the directory 'MyDirectory' exactly.</font>");
            }
        }
        else {
            emit wrongAnswer();
            terminalOutput->append("<font color='#ff0000'>[ERROR]: Unknown or incomplete command.</font>");
            terminalOutput->append("<font color='#ffff00'>HINT: Use 'mkdir &lt;dirname&gt;' to create a folder.</font>");
        }
    }

    else {
        if (command == "cp" && args.size() == 3) {
            QString sourceFile = args[1];
            QString targetDir = args[2];

            if (targetDir.endsWith("/")) targetDir.chop(1);

            if (targetDir != "MyDirectory") {
                terminalOutput->append("<font color='#ff0000'>[ERROR]: Target directory does not exist.</font>");
                return;
            }

            if (requiredFiles.contains(sourceFile)) {
                terminalOutput->append("<font color='#00ff00'>✔ Copied " + sourceFile + " to MyDirectory/.</font>");

                QTreeWidgetItem *copiedFile = new QTreeWidgetItem(playerDir);
                copiedFile->setText(0, sourceFile);
                copiedFile->setForeground(0, QBrush(Qt::red));

                requiredFiles.removeAll(sourceFile);
                filesCopiedCount++;

                if (requiredFiles.isEmpty()) {
                    terminalOutput->append("<br><font color='#00ff00'><b>*** ALL FILES SECURED. SYSTEM UNLOCKED. ***</b></font>");
                    terminalInput->setReadOnly(true);
                    emit puzzleSolved();
                }
            } else {
                terminalOutput->append("<font color='#ff0000'>[ERROR]: File not found or already copied.</font>");
            }
        }
        else {
            emit wrongAnswer();
            terminalOutput->append("<font color='#ff0000'>[ERROR]: Unknown or incomplete command.</font>");
            terminalOutput->append("<font color='#ffff00'>HINT: Use 'cp &lt;filename&gt; MyDirectory' to copy files.</font>");
        }
    }

    terminalOutput->ensureCursorVisible();
}
