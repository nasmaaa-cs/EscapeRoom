#include <QApplication>
#include <QWidget>
#include "mainMenu.h"
#include "MainWindow.h"
#include "gameMode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
