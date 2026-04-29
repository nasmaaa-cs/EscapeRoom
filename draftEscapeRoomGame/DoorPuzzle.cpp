#include "DoorPuzzle.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QTextCursor>
#include <QEvent>

DoorPuzzle::DoorPuzzle(QWidget *parent) : QWidget(parent)
{
    setupUI();
    resetPuzzle();
}

void DoorPuzzle::setupUI()
{
    setFixedSize(900, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *editorLayout = new QHBoxLayout();

    //files
    QVBoxLayout *sideNodes = new QVBoxLayout();
    cmakeNode = new QLabel("CMakeLists.txt");
    cppNode = new QLabel("main.cpp");
    cmakeNode->setFixedSize(160, 80);
    cppNode->setFixedSize(160, 80);
    cmakeNode->setAlignment(Qt::AlignCenter);
    cppNode->setAlignment(Qt::AlignCenter);
    cmakeNode->installEventFilter(this);
    cppNode->installEventFilter(this);

    sideNodes->addWidget(new QLabel("PROJECT_HIVE:"));
    sideNodes->addWidget(cmakeNode);
    sideNodes->addWidget(cppNode);
    sideNodes->addStretch();

    //editor window
    QVBoxLayout *centerArea = new QVBoxLayout();
    instructionLabel = new QLabel("INITIALIZING...");
    instructionLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    codeEditor = new QTextEdit();
    codeEditor->setFont(QFont("Consolas", 13));

    statusDisplay = new QLabel("SYSTEM_READY");
    statusDisplay->setAlignment(Qt::AlignCenter);
    statusDisplay->setFixedHeight(80);

    execButton = new QPushButton("BUILD AND EXECUTE");
    closeButton = new QPushButton("X");
    closeButton->setFixedSize(35, 35);

    centerArea->addWidget(closeButton, 0, Qt::AlignRight);
    centerArea->addWidget(instructionLabel);
    centerArea->addWidget(codeEditor);
    centerArea->addWidget(statusDisplay);
    centerArea->addWidget(execButton);

    editorLayout->addLayout(sideNodes);
    editorLayout->addLayout(centerArea);
    mainLayout->addLayout(editorLayout);

    connect(execButton, &QPushButton::clicked, this, &DoorPuzzle::handleExecute);
    connect(closeButton, &QPushButton::clicked, this, &DoorPuzzle::backToRoom);
    connect(codeEditor, &QTextEdit::textChanged, this, &DoorPuzzle::validateBoilerplate);

    updatePanelStyle("#00ffff");
}

void DoorPuzzle::updatePanelStyle(QString colorHex)
{
    this->setStyleSheet(QString(
                            "DoorPuzzle { background-color: rgba(0, 10, 25, 230); border: 4px solid %1; border-radius: 12px; }"
                            "QLabel { color: %1; font-family: 'Consolas'; }"
                            "QTextEdit { background: rgba(0,0,0,200); border: 1px solid %1; color: #ffffff; }"
                            "QPushButton { background: %1; color: #000000; font-weight: bold; border: none; min-height: 40px; }"
                            ).arg(colorHex));

    QString nodeStyle = QString("border: 2px solid %1; background: rgba(0, 255, 255, 15);").arg(colorHex);
    cmakeNode->setStyleSheet(nodeStyle);
    cppNode->setStyleSheet(nodeStyle);
}

void DoorPuzzle::validateBoilerplate()
{
    if (currentFile == "cmake") {
        if (!codeEditor->toPlainText().startsWith(cmakeBoilerplate)) {
            codeEditor->blockSignals(true);
            codeEditor->setPlainText(cmakeBoilerplate + cmakeUserAdded);
            codeEditor->blockSignals(false);

            QTextCursor cursor = codeEditor->textCursor();
            cursor.movePosition(QTextCursor::End);
            codeEditor->setTextCursor(cursor);
        }
    }
}

void DoorPuzzle::switchToCMake()
{
    if (currentFile == "cpp") cppCode = codeEditor->toPlainText();
    currentFile = "cmake";
    instructionLabel->setText("TASK: ADD Qt6 WIDGETS");
    codeEditor->setPlainText(cmakeBoilerplate + cmakeUserAdded);

    QTextCursor cursor = codeEditor->textCursor();
    cursor.movePosition(QTextCursor::End);
    codeEditor->setTextCursor(cursor);
}

void DoorPuzzle::switchToCpp()
{
    if (currentFile == "cmake") {
        cmakeUserAdded = codeEditor->toPlainText().mid(cmakeBoilerplate.length());
    }
    currentFile = "cpp";
    instructionLabel->setText("TASK: write a Qt main using QLabel and Pixmap command to create and display door.png");
    codeEditor->setPlainText(cppCode);
}

void DoorPuzzle::handleExecute()
{
    if (currentFile == "cmake") cmakeUserAdded = codeEditor->toPlainText().mid(cmakeBoilerplate.length());
    else cppCode = codeEditor->toPlainText();

    //validation
    bool cmakeValid = cmakeUserAdded.contains("find_package(Qt6 REQUIRED COMPONENTS Widgets)") &&
                      cmakeUserAdded.contains("target_link_libraries") &&
                      cmakeUserAdded.contains("Qt6::Widgets");

    bool cppValid = cppCode.contains("QLabel") &&
                    cppCode.contains("setPixmap") &&
                    cppCode.contains("door.png") &&
                    cppCode.contains("show()");

    if (cmakeValid && cppValid) {
        updatePanelStyle("#00ff00"); // success
        statusDisplay->setStyleSheet("color: #00ff00; font-size: 24px; font-weight: bold;");
        statusDisplay->setText(">>> SUCCESS: DOOR.EXE COMPILED <<<");
        execButton->setEnabled(false);
        QTimer::singleShot(1500, this, [=]() { emit puzzleSolved(); });
    } else {
        updatePanelStyle("#ff0000"); // fail
        statusDisplay->setStyleSheet("color: #ff0000; font-size: 18px;");
        statusDisplay->setText("BUILD ERROR: MISSING DEPENDENCIES");
        emit wrongAnswer(); // triggers shake

        QTimer::singleShot(1500, this, [=]() { updatePanelStyle("#00ffff"); });
    }
}

void DoorPuzzle::resetPuzzle()
{
    cmakeUserAdded = "";
    cppCode = "";
    switchToCMake();
    updatePanelStyle("#00ffff");
}

bool DoorPuzzle::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == cmakeNode) { switchToCMake(); return true; }
        if (obj == cppNode) { switchToCpp(); return true; }
    }
    return QWidget::eventFilter(obj, event);
}
