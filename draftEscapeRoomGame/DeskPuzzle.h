#pragma once

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QMouseEvent>

    class DraggableWidget : public QWidget {
    Q_OBJECT
public:
    explicit DraggableWidget(QWidget *parent = nullptr) : QWidget(parent) {}
protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            this->raise();
            dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
        }
    }
    void mouseMoveEvent(QMouseEvent *event) override {
        if (event->buttons() & Qt::LeftButton) {
            move(event->globalPosition().toPoint() - dragPosition);
            event->accept();
        }
    }
private:
    QPoint dragPosition;
};

class DeskPuzzle : public QWidget
{
    Q_OBJECT
public:
    explicit DeskPuzzle(QWidget *parent = nullptr);
    void showTaskWindow();
    void showMakefileWindow();
    void showCMakeWindow();

signals:
    void puzzleSolved();
    void wrongAnswer();
    void backToRoom();

private slots:
    void checkAnswer();
    void showTerminalError(const QString &msg);

private:
    QWidget *taskWindow;
    QWidget *makefileWindow;
    QWidget *cmakeWindow;
    QTextEdit *cmakeInput;


    QWidget* createPopup(const QString &title, QWidget *contentWidget, int x, int y, int w, int h);
};
