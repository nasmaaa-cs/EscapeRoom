#include <QApplication>
#include <QWidget>
#include "mainMenu.h"
#include "MainWindow.h"
#include "gameMode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainMenu menu;

    menu.show();

    QObject::connect(&menu, &mainMenu::StartGame, [&](GameMode mode, QString name) {
        MainWindow *game = new MainWindow();
        game->setPlayerName(name);
        game->startGame(mode);
        game->showMaximized();
        menu.close();
    });

    return a.exec();
}
