#include <Player.h>

Player::Player(Pixel* start_point, std::vector<Pixel*> player_fields, char color, int start_pos):
    start(start_point), player_fields(player_fields), color(color)
{
    for (int i = 0; i < 4; i++) {
        pawns.push_back(new Pawn(start_point, start_pos, color));
    }
}

Player::~Player() {
    for (int i = 0; i < 4; i++) delete pawns[i];
}
