#include <Player.h>
#include <QDebug>

Player::Player(Pixel* start_point, std::vector<Pixel*> player_fields, std::string color, int start_pos):
    start(start_point), player_fields(player_fields), color(color)
{
    for (int i = 0; i < 4; i++) {
        pawns.push_back({ new Pawn(start_point, start_pos, color, i), state::BASE } );
    }
}

Player::~Player() {
    for (int i = 0; i < 4; i++) delete pawns[i].p;
}

int Player::move_board_pawn(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    // Move random pawn on board
    // find all pawns on board, pick a random index, move that pawn forward
    std::vector<int> active_pawns = get_active_pawns();
    int random_index = pick_random_index(active_pawns.size());
    auto& pawn = pawns.at(random_index).p;
    pawn->get_current_pos()->set_Pixmap(QPixmap(":/images/border.png"));   //move current_pos to a play field

    while (die_roll) {
        //pawn->get_current_pos()->set_pawn(nullptr);
        int finish = pawn->move(play_fields, player_fields, finish);

        if (die_roll == 1) {
            reset_opponent_piece(pawn, players, play_fields);
            set_pawn_on_field(random_index);
        }
        pawn->get_current_pos()->set_pawn(pawn);

        if (finish) {
            finished_pawns++;
            set_pawn_on_field(random_index);
            pawns.at(random_index).s = state::FINISHED;
            die_roll = 1;
        }

        die_roll--;
    }
    if (finished_pawns == 4) {
        qInfo() << color.c_str() << "player wins!";
        return 0;
    }
    return 1;
}

int Player::move(Dice* dice, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    int die_roll = dice->roll();
    qInfo() << "Player" << color.c_str() << "rolled" << die_roll;

    if (has_pawn_on_board() && die_roll != 6) {
        return move_board_pawn(die_roll, players, play_fields);
    }
    else {
        if (die_roll == 6) {
            if (is_pawn_at_base()) {
                auto& pawn = pawns[get_pawn_index_at_base()];  //No need to have a random pick from base
                auto& p = pawn.p;
                //player_fields.at(get_pawn_index_at_base())->set_Pixmap(QPixmap(":/images/border.png"));
                pawn.s = state::INFLIGHT;

                reset_opponent_piece(p, players, play_fields);
                std::string col = p->get_color();

                switch(col[0]) {

                case 'b':
                    p->get_current_pos()->set_Pixmap(QPixmap(":/images/blue.png"));
                    player_fields.at(p->get_base_location())->set_Pixmap(QPixmap(":/images/blue.png"));  //May need to fix this?
                    start->set_Pixmap(QPixmap(":/images/bluepawn.png"));
                    break;
                case 'r':
                    p->get_current_pos()->set_Pixmap(QPixmap(":/images/red.png"));
                    player_fields.at(p->get_base_location())->set_Pixmap(QPixmap(":/images/red.png"));
                    start->set_Pixmap(QPixmap(":/images/redpawn.png"));
                    break;
                case 'y':
                    p->get_current_pos()->set_Pixmap(QPixmap(":/images/yellow.png"));
                    player_fields.at(p->get_base_location())->set_Pixmap(QPixmap(":/images/yellow.png"));
                    start->set_Pixmap(QPixmap(":/images/yellowpawn.png"));
                    break;
                case 'g':
                    p->get_current_pos()->set_Pixmap(QPixmap(":/images/green.png"));
                    player_fields.at(p->get_base_location())->set_Pixmap(QPixmap(":/images/green.png"));
                    start->set_Pixmap(QPixmap(":/images/greenpawn.png"));
                    break;
                }
                p->set_current_pos(start);
                start->set_pawn(p);

                reset_opponent_piece(p, players, play_fields);
            }
            else if (has_pawn_on_board()) {
                return move_board_pawn(die_roll, players, play_fields);
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

// Need attacking pawn to get the field's current pawn
void Player::reset_opponent_piece(Pawn* attacking_pawn, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    auto curr_pixel = attacking_pawn->get_current_pos();
    if (curr_pixel->get_pawn() || curr_pixel->has_more_than_one_pawn()) return;
    Pawn* curr_pawn = curr_pixel->get_pawn();

    if (curr_pawn && attacking_pawn->get_color() != curr_pawn->get_color()) {
        std::string color = curr_pawn->get_color();
        int base_location = curr_pawn->get_base_location();
        switch (color[0]) {
            case 'b':
                players.at(0)->pawns.at(base_location).p->set_current_pos(player_fields.at(base_location));
                players.at(0)->pawns.at(base_location).s = state::BASE;
                players.at(0)->pawns.at(base_location).p->reset_moves();
                players.at(0)->player_fields.at(base_location)->set_pawn(curr_pawn);
                players.at(0)->player_fields.at(base_location)->set_Pixmap(QPixmap(":/images/bluepawn.png"));
                break;
            case 'r':
                players.at(1)->pawns.at(base_location).p->set_current_pos(player_fields.at(base_location));
                players.at(1)->pawns.at(base_location).s = state::BASE;
                players.at(1)->pawns.at(base_location).p->reset_moves();
                players.at(1)->player_fields.at(base_location)->set_pawn(curr_pawn);
                players.at(1)->player_fields.at(base_location)->set_Pixmap(QPixmap(":/images/redpawn.png"));
                break;
            case 'y':
                players.at(2)->pawns.at(base_location).p->set_current_pos(player_fields.at(base_location));
                players.at(2)->pawns.at(base_location).s = state::BASE;
                players.at(2)->pawns.at(base_location).p->reset_moves();
                players.at(2)->player_fields.at(base_location)->set_pawn(curr_pawn);
                players.at(2)->player_fields.at(base_location)->set_Pixmap(QPixmap(":/images/yellowpawn.png"));
                break;
            case 'g':
                players.at(3)->pawns.at(base_location).p->set_current_pos(player_fields.at(base_location));
                players.at(3)->pawns.at(base_location).s = state::BASE;
                players.at(3)->pawns.at(base_location).p->reset_moves();
                players.at(3)->player_fields.at(base_location)->set_pawn(curr_pawn);
                players.at(3)->player_fields.at(base_location)->set_Pixmap(QPixmap(":/images/greenpawn.png"));
                break;
        }
    }
}
