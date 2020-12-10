#ifndef PLAYER_H
#define PLAYER_H

#include <Pawn.h>
#include <Dice.h>
#include <Pixel.h>
#include <random>

//! Holds the states of the pawns
enum state { BASE, INFLIGHT, FINISHED };

//! Enum storing the different player strategies
enum strategy {
    FAST,
    RANDOM,
    ATTACKING,
    DEFENSIVE
};

class Player {
    struct pawns_state {
        Pawn* p;
        state s;
    };

    std::vector<pawns_state> pawns;
    Pixel* start;
    std::vector<Pixel*> player_fields;
    std::string color;
    strategy st;
    int finish_point = 43;  //39 + 4 locations
    int finished_pawns = 0;
    int moves = 0;
public:
    std::string get_color() { return color; }
    Player(Pixel* start_point, std::vector<Pixel*> player_fields, std::string color, int start_pos, strategy st, int game_num);
    ~Player();
    //int move(/*Dice* dice*/ int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields);
    int move(Dice* dice, std::vector<Player*> players, std::vector<Pixel*> play_fields);
    bool has_pawn_on_board();
    Pawn* get_pawn_on_board();
    Pawn* get_pawn_at_base();
    bool is_pawn_at_base();
    int get_pawn_index_at_base();
    strategy get_strategy() { return st; }
    int play_random_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields);
    int play_fast_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields);
    int play_attacking_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields);
    int play_defensive_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields);
    std::vector<int> get_active_pawns();
    std::vector<int> get_base_and_board_pawns();
    void set_pawn_on_field(Pawn* p);
    void reset_opponent_piece(Pawn* attacking_pawn, std::vector<Player*>& players);
    int move_board_pawn(Pawn* pawn, int die_roll, std::vector<Player*>& players, const std::vector<Pixel*>& play_fields);
    void move_base_pawn(Pawn *p, std::vector<Player*>& players);
    int get_moves() { return moves; }
    bool is_attack_possible(Pawn *p, state s, int die_roll, std::vector<Pixel*> play_fields);
    bool is_defense_possible(Pawn *p, state s, int die_roll, const std::vector<Pixel*>& play_fields);
    int game_number = -1;
};

#endif // PLAYER_H
