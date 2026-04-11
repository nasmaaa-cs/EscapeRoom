#include <QApplication>
#include "mainMenu.h"
#include "MainWindow.h"
#include "gameMode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainMenu menu;

    menu.show();

    //QObject::connect(&menu, &mainMenu::startGame, [&]() {
    //    menu.close();
    //   game.show();
    //   game.startGameTimer();
    //});

    QObject::connect(&menu, &mainMenu::StartGame, [&](gameMode mode) {
        MainWindow *game = new MainWindow();
        game->setGameMode(mode);
        game->showMaximized();
        menu.close();
});

    return a.exec();
}
