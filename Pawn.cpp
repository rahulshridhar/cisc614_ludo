#include <Pawn.h>

Pawn::Pawn(Pixel* start_point, int start, std::string color, int location)
    : start(start), current_pos(start_point), color(color), base_location(location) {}

Pawn::~Pawn() {}

bool Pawn::move(const std::vector<Pixel*>& play_fields, const std::vector<Pixel*>& player_fields, int& finish) {
    passed_fields++;

    if (passed_fields >= 40) {
        current_pos = player_fields[4+(finish-passed_fields)];
        finish--;
        return true;
    }
    else {
        current_pos = play_fields[(start+passed_fields)%40];
    }
    return false;
}
