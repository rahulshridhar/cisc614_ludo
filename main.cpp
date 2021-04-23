#include <QApplication>
#include "Board.h"
#include <iostream>
#include <QObject>
#include <unistd.h>
#include <Common.h>

bool display_gui = true;
bool debug = false;
int total_games = 1;
int sim_time_in_ms = 10;
bool play_teams = true;

int main(int argc, char *argv[])
{
    int blue_wins = 0, blue_moves = 0;
    int red_wins = 0, red_moves = 0;
    int yellow_wins = 0, yellow_moves = 0;
    int green_wins = 0, green_moves = 0;

    QApplication a(argc, argv);
    for (int i = 0; i < total_games; i++) {
        Board *board = new Board(i+1, play_teams);

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

    if (play_teams) {
        int team1_wins = blue_wins + yellow_wins;
        int team2_wins = red_wins + green_wins;
        int team1_moves = blue_moves + yellow_moves;
        int team2_moves = red_moves + green_moves;

        double team1_win_pc = (double)team1_wins*100/total_games;
        double team2_win_pc = (double)team2_wins*100/total_games;
        int team1_avg_moves = team1_moves ? team1_moves/team1_wins : 0;
        int team2_avg_moves = team2_moves ? team2_moves/team2_wins : 0;

        std::cout << "Total games = " << total_games << std::endl;
        std::cout << "Team1 %-win of blue and yellow player = " << team1_win_pc << " Average moves = " << team1_avg_moves << std::endl;
        std::cout << "Team2 %-win of red and green player = " << team2_win_pc << " Average moves = " << team2_avg_moves << std::endl;
    } else {
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
    }
    return a.exec();
}
