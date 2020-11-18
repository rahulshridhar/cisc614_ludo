#include <Pawn.h>

Pawn::Pawn(Pixel* start_point, int start, char color) : start(start), current_pos(start_point), color(color) {}

Pawn::~Pawn() {}
