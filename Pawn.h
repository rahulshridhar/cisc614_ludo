#ifndef PAWN_H
#define PAWN_H

#include <Pixel.h>

class Pawn {
    int start;
    Pixel* current_pos;
    std::string color;
    int moves = 0;

public:
    //Pixel* curr_location;
    Pawn(Pixel* start_point, int start, std::string color);
    std::string get_color() { return color; }
    Pixel* get_current_pos() { return current_pos; }
    void set_current_pos(Pixel* p) { current_pos = p; }
    bool move(std::vector<Pixel*>& play_fields, std::vector<Pixel*>& player_fields, int& finish);
    ~Pawn();

};

#endif // PAWN_H
