#include <Player.h>
#include <QDebug>

Player::Player(Pixel* start_point, std::vector<Pixel*> player_fields, std::string color, int start_pos, strategy st):
    start(start_point), player_fields(player_fields), color(color), st(st)
{
    for (int i = 0; i < 4; i++) {
        pawns.push_back({ new Pawn(start_point, start_pos, color, i), state::BASE } );
    }
}

Player::~Player() {
    for (int i = 0; i < 4; i++) delete pawns[i].p;
}

int Player::move_board_pawn(Pawn* pawn, int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    int base_loc = pawn->get_base_location();
    pawn->get_current_pos()->set_Pixmap(QPixmap(":/images/border.png"));   //move current_pos to a play field
    pawn->get_current_pos()->remove_pawn();

    moves++;

    while (die_roll) {
        //pawn->get_current_pos()->set_pawn(nullptr);
        int finish = pawn->move(play_fields, player_fields, finish_point);

        if (die_roll == 1) {
            reset_opponent_piece(pawn, players, play_fields);
            set_pawn_on_field(pawn);
            pawn->get_current_pos()->set_pawn(pawn);
        }

        if (finish) {
            finished_pawns++;
            set_pawn_on_field(pawn);
            pawns.at(base_loc).s = state::FINISHED;
            if (die_roll > 1) pawn->get_current_pos()->set_pawn(pawn);
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

//int Player::move(/*Dice* dice*/ int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
int Player::move(Dice* dice, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    int die_roll = dice->roll();
    //qInfo() << "Player" << color.c_str() << "rolled" << die_roll;

    auto player_strategy = get_strategy();

    switch (player_strategy) {
        case strategy::FAST:
            return play_fast_move(die_roll, players, play_fields);
        case strategy::RANDOM:
            return play_random_move(die_roll, players, play_fields);
        case strategy::ATTACKING:
            return play_attacking_move(die_roll, players, play_fields);
        case strategy::DEFENSIVE:
            return play_defensive_move(die_roll, players, play_fields);
    }
}

int Player::play_random_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    if (has_pawn_on_board() && die_roll != 6) {
        // Move random pawn on board
        // find all pawns on board, pick a random index, move that pawn forward
        std::vector<int> active_pawns = get_active_pawns();
        int num_active_pawns = active_pawns.size();
        int random_index = pick_random_index(0, num_active_pawns-1);
        auto& pawn = pawns.at(random_index).p;
        return move_board_pawn(pawn, die_roll, players, play_fields);
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
                // pick a random pawn on board to move;
                std::vector<int> active_pawns = get_active_pawns();
                int random_index = pick_random_index(0, active_pawns.size()-1);
                auto& pawn = pawns.at(random_index).p;
                return move_board_pawn(pawn, die_roll, players, play_fields);
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

int Player::play_fast_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    if (has_pawn_on_board()) {
        Pawn* p = get_pawn_on_board();
        assert(p);
        return move_board_pawn(p, die_roll, players, play_fields);
    }
    else {
        if(die_roll == 6) {
            if (is_pawn_at_base()) {
                auto& pawn = pawns[get_pawn_index_at_base()];  //No need to have a random pick from base
                auto& p = pawn.p;
                //player_fields.at(get_pawn_index_at_base())->set_Pixmap(QPixmap(":/images/border.png"));
                pawn.s = state::INFLIGHT;

                //reset_opponent_piece(p, players, play_fields);
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
                moves++;
            }
        }
    }
    return 1;
}

int Player::play_attacking_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    bool can_attack = false;

    if (!has_pawn_on_board()) return play_random_move(die_roll, players, play_fields);

    std::vector<int> active_pawns = get_active_pawns();
    for (unsigned int i = 0; i < active_pawns.size(); i++) {
        int base_loc = active_pawns[i];
        Pawn* p = pawns.at(base_loc).p;
        //can_attack = is_attack_possible(p, players, play_fields);
        if (can_attack) {
            move_board_pawn(p, die_roll, players, play_fields);
            reset_opponent_piece(p, players, play_fields);  //need to change pawn position before this attack
            return 1;    //can never cause the game to end
        }
    }

    // if no attack possible, play a random piece
    if (!can_attack) return play_random_move(die_roll, players, play_fields);
}

int Player::play_defensive_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    bool can_defend = false;

    if (!has_pawn_on_board()) return play_random_move(die_roll, players, play_fields);

    std::vector<int> active_pawns = get_active_pawns();
    for (unsigned int i = 0; i < active_pawns.size(); i++) {
        int base_loc = active_pawns[i];
        Pawn* p = pawns.at(base_loc).p;
        //can_defend = is_defending_possible(p, players, play_fields);
        if (can_defend) {
            move_board_pawn(p, die_roll, players, play_fields);
            reset_opponent_piece(p, players, play_fields);  //need to change pawn position before this attack
            return 1;    //can never cause the game to end
        }
    }

    // if no defensive move possible, play a random piece
    if (!can_defend) return play_random_move(die_roll, players, play_fields);
}

bool Player::has_pawn_on_board() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::INFLIGHT) return true;
    }
    return false;
}

Pawn* Player::get_pawn_on_board() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::INFLIGHT) return pawns[i].p;
    }
    return nullptr;
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

void Player::set_pawn_on_field(Pawn* p) {
    std::string color = p->get_color();
    switch (color[0]) {
        case 'b':
            p->get_current_pos()->set_Pixmap(QPixmap(":/images/bluepawn.png"));
            break;
        case 'r':
            p->get_current_pos()->set_Pixmap(QPixmap(":/images/redpawn.png"));
            break;
        case 'y':
            p->get_current_pos()->set_Pixmap(QPixmap(":/images/yellowpawn.png"));
            break;
        case 'g':
            p->get_current_pos()->set_Pixmap(QPixmap(":/images/greenpawn.png"));
            break;
    }
}

// Need attacking pawn to get the field's current pawn
void Player::reset_opponent_piece(Pawn* attacking_pawn, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    auto curr_pixel = attacking_pawn->get_current_pos();
    Pawn* curr_pawn = curr_pixel->get_pawn();
    if (!curr_pawn || curr_pixel->has_more_than_one_pawn()) return;

    if (curr_pawn && attacking_pawn->get_color() != curr_pawn->get_color()) {
        curr_pixel->remove_pawn();
        std::string color = curr_pawn->get_color();
        int base_location = curr_pawn->get_base_location();
        switch (color[0]) {
            case 'b':
                players.at(0)->pawns.at(base_location).p->set_current_pos(players.at(0)->player_fields.at(base_location));
                players.at(0)->pawns.at(base_location).s = state::BASE;
                players.at(0)->pawns.at(base_location).p->reset_passed_fields();
                players.at(0)->player_fields.at(base_location)->set_pawn(curr_pawn);
                players.at(0)->player_fields.at(base_location)->set_Pixmap(QPixmap(":/images/bluepawn.png"));
                break;
            case 'r':
                players.at(1)->pawns.at(base_location).p->set_current_pos(players.at(1)->player_fields.at(base_location));
                players.at(1)->pawns.at(base_location).s = state::BASE;
                players.at(1)->pawns.at(base_location).p->reset_passed_fields();
                players.at(1)->player_fields.at(base_location)->set_pawn(curr_pawn);
                players.at(1)->player_fields.at(base_location)->set_Pixmap(QPixmap(":/images/redpawn.png"));
                break;
            case 'y':
                players.at(2)->pawns.at(base_location).p->set_current_pos(players.at(2)->player_fields.at(base_location));
                players.at(2)->pawns.at(base_location).s = state::BASE;
                players.at(2)->pawns.at(base_location).p->reset_passed_fields();
                players.at(2)->player_fields.at(base_location)->set_pawn(curr_pawn);
                players.at(2)->player_fields.at(base_location)->set_Pixmap(QPixmap(":/images/yellowpawn.png"));
                break;
            case 'g':
                players.at(3)->pawns.at(base_location).p->set_current_pos(players.at(3)->player_fields.at(base_location));
                players.at(3)->pawns.at(base_location).s = state::BASE;
                players.at(3)->pawns.at(base_location).p->reset_passed_fields();
                players.at(3)->player_fields.at(base_location)->set_pawn(curr_pawn);
                players.at(3)->player_fields.at(base_location)->set_Pixmap(QPixmap(":/images/greenpawn.png"));
                break;
        }
    }
}
