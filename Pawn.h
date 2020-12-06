#ifndef PAWN_H
#define PAWN_H

#include <Pixel.h>

class Pawn {
    int start;
    Pixel* current_pos;
    std::string color;
    int base_location;
    int passed_fields = 0;

public:
    Pawn(Pixel* start_point, int start, std::string color, int location);
    std::string get_color() { return color; }
    Pixel*& get_current_pos() { return current_pos; }
    void set_current_pos(Pixel* p) { current_pos = p; }
    bool move(const std::vector<Pixel*>& play_fields, const std::vector<Pixel*>& player_fields, int& finish);
    int get_base_location() { return base_location; }
    void reset_passed_fields() { passed_fields = 0; }
    void increment_passed_fields() { passed_fields += 1; }
    int get_passed_fields() { return passed_fields; }
    int get_start() { return start; }
    ~Pawn();

};

#endif // PAWN_H
