#include <QApplication>
#include "Board.h"
#include <iostream>

int total_games = 1;
int blue_wins = 0;
int red_wins = 0;
int yellow_wins = 0;
int green_wins = 0;

int main(int argc, char *argv[])
{
    for (int i = 0; i < total_games; i++) {
        QApplication a(argc, argv);

        Board *board = new Board();

        board->setSceneRect(0, 0, 1000, 900);
        board->draw();
        board->play();

        //QObject::connect(scene, &Board::exit, &a, &QApplication::quit, Qt::QueuedConnection);
        //if (scene->play()) emit scene->exit();

        a.exec();
        //blue = 0, red = 1, yellow = 2, green = 3
        if (board->get_turn() == 0) blue_wins++;
        else if (board->get_turn() == 1) red_wins++;
        else if (board->get_turn() == 2) yellow_wins++;
        else if (board->get_turn() == 3) green_wins++;

        delete board;
    }

    std::cout << "Total games = " << total_games << std::endl;
    std::cout << "Blue wins = " << blue_wins << std::endl;
    std::cout << "Red wins = " << red_wins << std::endl;
    std::cout << "Yellow wins = " << yellow_wins << std::endl;
    std::cout << "Green wins = " << green_wins << std::endl;
    return 0;
}
