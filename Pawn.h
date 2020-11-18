#ifndef PAWN_H
#define PAWN_H

#include <Pixel.h>

class Pawn {
    int start;
    Pixel* current_pos;
    char color;

public:
    Pixel* curr_location;
    Pawn(Pixel* start_point, int start, char color);
    ~Pawn();
};

#endif // PAWN_H
