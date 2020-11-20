#include <Player.h>
#include <QDebug>

Player::Player(Pixel* start_point, std::vector<Pixel*> player_fields, std::string color, int start_pos):
    start(start_point), player_fields(player_fields), color(color)
{
    for (int i = 0; i < 4; i++) {
        pawns.push_back({ new Pawn(start_point, start_pos, color), state::BASE } );
    }
}

Player::~Player() {
    for (int i = 0; i < 4; i++) delete pawns[i].p;
}

int Player::move(Dice* dice, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    int die_roll = dice->roll();
    qInfo() << "Player" << get_color().c_str() << "rolled" << die_roll;

    bool pawn_on_board = has_pawn_on_board();
    if (pawn_on_board) {
        // Move random pawn on board
        // find all pawns on board, pick a random index, move that pawn forward
        std::vector<int> active_pawns = get_active_pawns();
        int random_index = pick_random_index(sizeof(active_pawns));
        pawns.at(random_index).p->get_current_pos()->set_Pixmap(QPixmap(":/images/border.png"));   //move current_pos to a play field

        while (die_roll) {
            pawns.at(random_index).p->get_current_pos()->set_pawn(nullptr);
            int finish = pawns.at(random_index).p->move(play_fields, player_fields, finish);

            die_roll--;
        }
    }
    else {
        if (die_roll == 6) {
            if (is_pawn_at_base()) {
                auto& pawn = pawns[get_pawn_index_at_base()];  //No need to have a random pick from base
                pawn.p->get_current_pos()->set_Pixmap(QPixmap(":/images/border.png"));
                pawn.s = state::INFLIGHT;
            }
            else {
                //No pawn at base, no pawn on board
                //Can only be finished at this point
                assert(false);
            }
        }
    }

    return 1;
}

bool Player::has_pawn_on_board() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::INFLIGHT) return true;
    }
    return false;
}

bool Player::is_pawn_at_base() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::BASE) return true;
    }
    return false;
}

int Player::get_pawn_index_at_base() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::BASE) return i;
    }
    assert(false);
}

std::vector<int> Player::get_active_pawns() {
    std::vector<int> ret;
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::INFLIGHT) ret.push_back(i);
    }
    return ret;
}

int Player::pick_random_index(int length) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, length-1);
    return distrib(gen);
}


void Player::set_pawn_on_field(int index) {
    auto& pawn = pawns.at(index).p;
    std::string color = pawn->get_color();
    switch (color[0]) {
        case 'b':
            pawn->get_current_pos()->set_Pixmap(QPixmap(":/images/bluepawn.png"));
            break;
        case 'r':
            pawn->get_current_pos()->set_Pixmap(QPixmap(":/images/redpawn.png"));
            break;
        case 'y':
            pawn->get_current_pos()->set_Pixmap(QPixmap(":/images/yellowpawn.png"));
            break;
        case 'g':
            pawn->get_current_pos()->set_Pixmap(QPixmap(":/images/greenpawn.png"));
            break;
    }
}
