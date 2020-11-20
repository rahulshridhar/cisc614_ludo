#include <Pawn.h>

Pawn::Pawn(Pixel* start_point, int start, std::string color) : start(start), current_pos(start_point), color(color) {}

Pawn::~Pawn() {}

bool Pawn::move(std::vector<Pixel*>& play_fields, std::vector<Pixel*>& player_fields, int& finish) {
    moves++;

    if (moves >= 40) {
        current_pos = player_fields[4+(moves-40)];
        if (moves == finish) {
            finish--;
            return true;
        }
    }
    else {
        current_pos = play_fields[(start+moves)%40];
    }
    return false;
}
