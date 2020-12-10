#include <Player.h>
#include <QDebug>
#include <iostream>
#include <Common.h>

Player::Player(Pixel* start_point, std::vector<Pixel*> player_fields, std::string color, int start_pos, strategy st, int game_num):
    start(start_point), player_fields(player_fields), color(color), st(st), game_number(game_num)
{
    for (int i = 0; i < 4; i++) {
        pawns.push_back({ new Pawn(start_point, start_pos, color, i), state::BASE } );
    }
}

Player::~Player() {
    for (int i = 0; i < 4; i++) delete pawns[i].p;
}

//! Given a pawn, this moves the pawn by die_roll number of squares on the board
//! Also checks if game is finished
int Player::move_board_pawn(Pawn* pawn, int die_roll, std::vector<Player*>& players, const std::vector<Pixel*>& play_fields) {
    int base_loc = pawn->get_base_location();
    pawn->get_current_pos()->set_Pixmap(QPixmap(":/images/border.png"));   //move current_pos to a play field
    pawn->get_current_pos()->remove_pawn(pawn);

    while (die_roll) {
        //pawn->get_current_pos()->set_pawn(nullptr);
        int finish = pawn->move(play_fields, player_fields, finish_point);

        if (die_roll == 1) {
            reset_opponent_piece(pawn, players);
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
        qInfo() << "Game No." << game_number << " -> " << color.c_str() << "player wins!";
        return 0;
    }
    return 1;
}

//! Moves the pawn from base when a 6 is rolled
void Player::move_base_pawn(Pawn* p, std::vector<Player*>& players) {
    int base_loc = p->get_base_location();
    pawns.at(base_loc).s = state::INFLIGHT;

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

    reset_opponent_piece(p, players);
}

//! Starting point for a player move -> Player move depends on the strategy of the player
int Player::move(Dice* dice, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    int die_roll = dice->roll();
    if (debug) qInfo() << "Player" << color.c_str() << "rolled" << die_roll;

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

//! Function for a random move by a player
int Player::play_random_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    std::vector<int> available_pawns;
    // find all pawns on board, pick a random index, move that pawn forward

    //if die roll is 6, pick between base and board pawns
    if (die_roll == 6) {
        available_pawns = get_base_and_board_pawns();
    }
    //if die roll is not 6, pick random pawn between board pawns
    else {
        available_pawns = get_active_pawns();
    }

    int num_pawns = available_pawns.size();
    if (!num_pawns) return 1;

    //pick a random index between the available pawns
    int random_index = pick_random_index(0, num_pawns-1);
    auto& pawn = pawns.at(available_pawns[random_index]).p;
    auto pawn_state = pawns.at(available_pawns[random_index]).s;

    moves++;

    //check if this random pawn is on board or on base
    if (pawn_state == state::INFLIGHT) return move_board_pawn(pawn, die_roll, players, play_fields);
    else if (pawn_state == state::BASE) {
        move_base_pawn(pawn, players);
        return 1;
    }
    else {
        //Can only be finished at this point
        assert(false);
    }
    return 1;
}

//! Function to play a fast move by a player
int Player::play_fast_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    //Checks if pawn is present on board
    if (has_pawn_on_board()) {
        Pawn* p = get_pawn_on_board();
        assert(p);
        moves++;
        return move_board_pawn(p, die_roll, players, play_fields);
    }
    //If no pawn on board, has the player rolled a 6 -> Only this can move a pawn from base to the start
    else {
        if(die_roll == 6) {
            if (is_pawn_at_base()) {
                move_base_pawn(get_pawn_at_base(), players);
                moves++;
            }
        }
    }
    return 1;
}

//! Checks if the given pawn can launch an attack on an opponent -> Returns a boolean
bool Player::is_attack_possible(Pawn *p, state s, int die_roll, std::vector<Pixel*> play_fields) {
    if (!p || s == state::FINISHED) return false;

    int attacking_pos;

    if (s == state::INFLIGHT) {
        attacking_pos = (p->get_passed_fields() + p->get_start() + die_roll) % 40;
    }
    else {
        attacking_pos = p->get_start() % 40;
    }

    Pixel* pixel_attacking_pos = play_fields.at(attacking_pos);

    if (pixel_attacking_pos->is_empty()) return false;

    const std::vector<Pawn*>& all_pawns = pixel_attacking_pos->get_curr_pawns();
    //has_more_than_one pawn has a possibility of killing a single opponent pawn even if it is colocated with friendly pawn
    if (all_pawns.size() > 1) {
        int enemy_pawn = 0;
        for (const auto& pawn : all_pawns) {
            if (pawn->get_color() != p->get_color()) enemy_pawn++;
        }
        if (enemy_pawn > 1) return false;
    }
    else {      //if only one pawn
        Pawn* pawn_attacking_pos = pixel_attacking_pos->get_pawn();

        //return false if it is a friendly pawn
        if (pawn_attacking_pos->get_color() == p->get_color()) return false;
    }

    return true;
}

//! Function that launches an attacking move by a player
int Player::play_attacking_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    bool can_attack = false;

    std::vector<int> available_pawns;
    // find all pawns on board, find any one that can attack an opponent pawn

    //if die roll is 6, check between base and board pawns
    if (die_roll == 6) {
        available_pawns = get_base_and_board_pawns();
    }
    //if die roll is not 6, check between board pawns
    else {
        available_pawns = get_active_pawns();
    }

    int num_pawns = available_pawns.size();
    if (!num_pawns) return play_random_move(die_roll, players, play_fields);

    //Check if any available pawn can launch an attack - just picks the first pawn that can perform an attack
    for (unsigned int i = 0; i < available_pawns.size(); i++) {
        int base_loc = available_pawns[i];
        Pawn* p = pawns.at(base_loc).p;
        state s = pawns.at(base_loc).s;
        can_attack = is_attack_possible(p, s, die_roll, play_fields);
        if (can_attack) {
            if (s == state::BASE) {
                moves++;
                move_base_pawn(p, players);
                return 1;
            }
            else if (s == state::INFLIGHT) {
                moves++;
                return move_board_pawn(p, die_roll, players, play_fields);
            }
            else {
                assert(false);
            }
        }
    }

    // if no attack possible, play a random piece
    return play_random_move(die_roll, players, play_fields);
}

//! Checks if the given pawn can defend an attack from an opponent -> Returns a boolean
bool Player::is_defense_possible(Pawn *p, state s, int die_roll, const std::vector<Pixel*>& play_fields) {
    if (!p || s == state::FINISHED || s == state::BASE) return false;

    int defensive_pos = (p->get_passed_fields() + p->get_start() + die_roll) % 40;

    // Check if moving to this defensive position allows this pawn to be defended
    // This translates to checking if (pos-1 to pos-6 have no pawns on it)
    for (int start = 1; start <= 6; start++) {
        int pos = (defensive_pos - start) % 40;
        pos = pos < 0 ? 40 + pos : pos;
        //std::cout << defensive_pos << " " << start << " " << pos << std::endl;
        Pixel* pixel_behind = play_fields.at(pos);
        if (pixel_behind->is_empty()) continue;
        const std::vector<Pawn*> pawns = pixel_behind->get_curr_pawns();
        for (const auto& pawn: pawns) {
            if (pawn->get_color() != p->get_color()) return false;
        }
    }

    return true;
}

//! Function that launches a defensive move by a player
int Player::play_defensive_move(int die_roll, std::vector<Player*> players, std::vector<Pixel*> play_fields) {
    // find all pawns on board, find any one that can be defended
    // only board pawns need to be defended
    std::vector<int> available_pawns = get_active_pawns();
    int num_pawns = available_pawns.size();
    if (!num_pawns) return play_random_move(die_roll, players, play_fields);

    bool can_defend = false;

    // Check first available pawn that can defend itself
    for (unsigned int i = 0; i < available_pawns.size(); i++) {
        int base_loc = available_pawns[i];
        Pawn* p = pawns.at(base_loc).p;
        state s = pawns.at(base_loc).s;
        can_defend = is_defense_possible(p, s, die_roll, play_fields);
        if (can_defend) {
            if (s == state::INFLIGHT) {
                moves++;
                return move_board_pawn(p, die_roll, players, play_fields);
            }
            else {
                assert(false);
            }
        }
    }

    // if no defense possible, play a random piece
    return play_random_move(die_roll, players, play_fields);
}

//! Returns the first available pawn on board
Pawn* Player::get_pawn_on_board() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::INFLIGHT) return pawns[i].p;
    }
    return nullptr;
}

//! Returns true or false based on whether a pawn is available on the board
bool Player::has_pawn_on_board() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::INFLIGHT) return true;
    }
    return false;
}

//! Checks if pawn is available at base
bool Player::is_pawn_at_base() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::BASE) return true;
    }
    return false;
}

//! Returns the first available pawn at base
Pawn* Player::get_pawn_at_base() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::BASE) return pawns[i].p;
    }
    return nullptr;
}

//! Returns the base location of the first available pawn at base
int Player::get_pawn_index_at_base() {
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::BASE) return i;
    }
    assert(false);
}

//! Returns the list of all active pawns on the board
std::vector<int> Player::get_active_pawns() {
    std::vector<int> ret;
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::INFLIGHT) ret.push_back(i);
    }
    return ret;
}

//! Returns a list of base locations of all the base and inflight pawns
std::vector<int> Player::get_base_and_board_pawns() {
    std::vector<int> ret;
    for (int i = 0; i < 4; i++) {
        if (pawns[i].s == state::INFLIGHT || pawns[i].s == state::BASE) ret.push_back(i);
    }
    return ret;
}

//! Sets the pixel on the field based on the color of the pawn
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

//! Function that resets an opponent's piece if it can be attacked by the attacking_pawn
void Player::reset_opponent_piece(Pawn* attacking_pawn, std::vector<Player*>& players) {
    auto curr_pixel = attacking_pawn->get_current_pos();
    if (curr_pixel->is_empty()) return;

    const std::vector<Pawn*>& all_pawns = curr_pixel->get_curr_pawns();
    //has_more_than_one pawn has a possibility of killing a single opponent pawn even if it is colocated with friendly pawn
    Pawn *curr_pawn = nullptr;
    int enemy_pawns = 0;
    for (const auto& pawn : all_pawns) {
        if (pawn->get_color() != attacking_pawn->get_color()) {
            curr_pawn = pawn;
            enemy_pawns++;
        }
    }
    if (enemy_pawns > 1) return;

    if (curr_pawn && attacking_pawn->get_color() != curr_pawn->get_color()) {
        curr_pixel->remove_pawn(curr_pawn);
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
