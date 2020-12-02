#ifndef PAWN_H
#define PAWN_H

#include <Pixel.h>

class Pawn {
    int start;
    Pixel* current_pos;
    std::string color;
    int base_location;
    int moves = 0;

public:
    Pawn(Pixel* start_point, int start, std::string color, int location);
    std::string get_color() { return color; }
    Pixel*& get_current_pos() { return current_pos; }
    void set_current_pos(Pixel* p) { current_pos = p; }
    bool move(std::vector<Pixel*>& play_fields, std::vector<Pixel*>& player_fields, int& finish);
    int get_base_location() { return base_location; }
    void reset_moves() { moves = 0; }
    void increment_move() { moves += 1; }
    int get_moves() { return moves; }
    int get_start() { return start; }
    ~Pawn();

};

#endif // PAWN_H
