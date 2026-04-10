#include <QApplication>
#include "mainMenu.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainMenu menu;
    MainWindow game;

    menu.show();

    QObject::connect(&menu, &mainMenu::startGame, [&]() {
        menu.close();
        game.show();
        game.startGameTimer();
    });

    return a.exec();
}
