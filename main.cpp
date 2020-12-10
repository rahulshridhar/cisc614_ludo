#include <QApplication>
#include "Board.h"
#include <iostream>
#include <QObject>
#include <unistd.h>
#include <Common.h>

bool display_gui = true;
bool debug = true;
int total_games = 1;
int sim_time_in_ms = 10;

int main(int argc, char *argv[])
{
    int blue_wins = 0, blue_moves = 0;
    int red_wins = 0, red_moves = 0;
    int yellow_wins = 0, yellow_moves = 0;
    int green_wins = 0, green_moves = 0;

    QApplication a(argc, argv);
    for (int i = 0; i < total_games; i++) {
        Board *board = new Board(i+1);

        board->get_scene()->setSceneRect(0, 0, 1000, 900);
        board->draw();
        board->play(display_gui);

        if (display_gui) a.exec();

        //blue = 0, red = 1, yellow = 2, green = 3
        if (board->get_turn() == 0) {
            blue_wins++;
            blue_moves += board->get_players().at(0)->get_moves();
        }
        else if (board->get_turn() == 1) {
            red_wins++;
            red_moves += board->get_players().at(1)->get_moves();
        }
        else if (board->get_turn() == 2) {
            yellow_wins++;
            yellow_moves += board->get_players().at(2)->get_moves();
        }
        else if (board->get_turn() == 3) {
            green_wins++;
            green_moves += board->get_players().at(3)->get_moves();
        }

        delete board;
    }

    //assert(blue_wins + red_wins + yellow_wins + green_moves != total_games);

    // Compute win percentages and average moves taken during a player's win
    double blue_win_pc = (double)blue_wins*100/total_games;
    double red_win_pc = (double)red_wins*100/total_games;
    double yellow_win_pc = (double)yellow_wins*100/total_games;
    double green_win_pc = (double)green_wins*100/total_games;
    int avg_blue_moves = blue_moves ? blue_moves/blue_wins : 0;
    int avg_red_moves = red_moves ? red_moves/red_wins : 0;
    int avg_yellow_moves = yellow_moves ? yellow_moves/yellow_wins : 0;
    int avg_green_moves = green_moves ? green_moves/green_wins : 0;

    std::cout << "Total games = " << total_games << std::endl;
    std::cout << "Blue %-win = " << blue_win_pc << " Average moves = " << avg_blue_moves << std::endl;
    std::cout << "Red %-win = " << red_win_pc << " Average moves = " << avg_red_moves << std::endl;
    std::cout << "Yellow %-win = " << yellow_win_pc << " Average moves = " << avg_yellow_moves << std::endl;
    std::cout << "Green %-win = " << green_win_pc << " Average moves = " << avg_green_moves << std::endl;

    return a.exec();
}
