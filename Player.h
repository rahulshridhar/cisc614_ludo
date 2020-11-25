#ifndef PLAYER_H
#define PLAYER_H

#include <Pawn.h>
#include <Dice.h>
#include <Pixel.h>
#include <random>

class Player {
    enum state { BASE, INFLIGHT, FINISHED };
    struct pawns_state {
        Pawn* p;
        state s;
    };

    std::vector<pawns_state> pawns;
    Pixel* start;
    std::vector<Pixel*> player_fields;
    std::string color;
    int finish = 43;  //39 + 4 locations
    int finished_pawns = 0;
public:
    std::string get_color() { return color; }
    Player(Pixel* start_point, std::vector<Pixel*> player_fields, std::string color, int start_pos);
    ~Player();
    int move(Dice* dice, std::vector<Player*> players, std::vector<Pixel*> play_fields);
    bool has_pawn_on_board();
    bool is_pawn_at_base();
    int get_pawn_index_at_base();
    std::vector<int> get_active_pawns();
    int pick_random_index(int length);
    void set_pawn_on_field(int index);
    void reset_opponent_piece(Pawn* attacking_pawn, std::vector<Player*> players, std::vector<Pixel*> play_fields);
    int move_board_pawn(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields);
};

#endif // PLAYER_H
