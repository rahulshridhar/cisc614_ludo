#ifndef PLAYER_H
#define PLAYER_H

#include <Pawn.h>

class Player {
    std::vector<Pawn*> pawns;
    Pixel* start;
    std::vector<Pixel*> player_fields;
    char color;
public:
    char get_color() { return color; }
    Player(Pixel* start_point, std::vector<Pixel*> player_fields, char color, int start_pos);
    ~Player();
};

#endif // PLAYER_H
