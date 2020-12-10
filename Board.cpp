#include "Board.h"
#include "Player.h"
#include <QGraphicsPixmapItem>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <Common.h>

Board::Board(int game_num) : game_number(game_num)
{
    scene = new QGraphicsScene();

    view = new QGraphicsView(scene);
    view->show();
    view->setFixedSize(1000, 800);

    dice = new Dice();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

Board::~Board()
{
    for (auto& i : play_fields) delete i;
    for (unsigned int i = 0; i < red_fields.size(); i++) {
        delete red_fields[i];
        delete blue_fields[i];
        delete green_fields[i];
        delete yellow_fields[i];
    }
    for (auto& p : players) delete p;

    delete dice;
    delete timer;
    delete view;
    delete scene;
}

// draw the ludo board
void Board::draw() {
    Node current_pos(350, 770);
    int i,j;

    // 10 fields from one player start to next player start (40 play fields)
    for(i=0; i<4; ++i) { //drawing fields to play

        for(j=0; j<4; ++j)
            draw_pixel(play_fields, current_pos, directions[i], ":/images/border.png");

        for(j=0; j<4; ++j)
            draw_pixel(play_fields, current_pos, directions[(i+3)%4], ":/images/border.png");

        for(j=0; j<2; ++j)
            draw_pixel(play_fields, current_pos, directions[i], ":/images/border.png");

    }

    //setting backgrounds of start-fields
    play_fields.at(0)->set_Pixmap(QPixmap(":/images/bluestart.png"));
    play_fields.at(10)->set_Pixmap(QPixmap(":/images/redstart.png"));
    play_fields.at(20)->set_Pixmap(QPixmap(":/images/yellowstart.png"));
    play_fields.at(30)->set_Pixmap(QPixmap(":/images/greenstart.png"));
    /////////////////////////////////////

    //drawing basement and finish of each player
    current_pos.x = 70;
    current_pos.y = 70;
    for(j=0; j<4; ++j)
        draw_pixel(red_fields, current_pos, directions[(j+1)%4], ":/images/redpawn.png");

    current_pos.x = 140;
    current_pos.y = 420;
    for(j=0; j<4; ++j)
        draw_pixel(red_fields, current_pos, directions[1], ":/images/redfinish.png");

    Player *player_red = new Player(play_fields.at(10), red_fields, "red", 10, strategy::DEFENSIVE, game_number);
    //red was drawn

    current_pos.x = 700;
    current_pos.y = 70;

    for(j=0; j<4; ++j)
        draw_pixel(yellow_fields, current_pos, directions[(j+1)%4], ":/images/yellowpawn.png");

    current_pos.x = 420;
    current_pos.y = 140;
    for(j=0; j<4; ++j)
        draw_pixel(yellow_fields, current_pos, directions[2], ":/images/yellowfinish.png");

    Player *player_yellow = new Player(play_fields.at(20), yellow_fields, "yellow", 20, strategy::RANDOM, game_number);
    //yellow was drawn

    current_pos.x = 700;
    current_pos.y = 700;
    for(j=0; j<4; ++j)
        draw_pixel(green_fields, current_pos, directions[(j+1)%4], ":/images/greenpawn.png");

    current_pos.x = 700;
    current_pos.y = 420;
    for(j=0; j<4; ++j)
        draw_pixel(green_fields, current_pos, directions[3], ":/images/greenfinish.png");

    Player *player_green = new Player(play_fields.at(30), green_fields, "green", 30, strategy::FAST, game_number);
    //green was drawn

    current_pos.x = 70;
    current_pos.y = 700;

    for(j=0; j<4; ++j)
        draw_pixel(blue_fields, current_pos, directions[(j+1)%4], ":/images/bluepawn.png");

    current_pos.x = 420;
    current_pos.y = 700;
    for(j=0; j<4; ++j)
        draw_pixel(blue_fields, current_pos, directions[0], ":/images/bluefinish.png");

    current_pos.x = 420;
    current_pos.y = 420;
    draw_pixel(center_field, current_pos, Node(0, 0), ":/images/all.png");
    Player *player_blue = new Player(play_fields.at(0), blue_fields, "blue", 0, strategy::ATTACKING, game_number);

    players.push_back(player_blue);
    players.push_back(player_red);
    players.push_back(player_yellow);
    players.push_back(player_green);

     //Set background to a dark color
     this->get_scene()->setBackgroundBrush(Qt::darkCyan);
}

void Board::draw_pixel(std::vector<Pixel *> &vec, Node &curr_pos, Node next_move, QString images_path)
{
     QPixmap images(images_path);
     QGraphicsPixmapItem *pix = this->get_scene()->addPixmap(images);
     pix->setPos(curr_pos.x, curr_pos.y);
     vec.push_back(new Pixel(pix));

     curr_pos.x += next_move.x;
     curr_pos.y += next_move.y;
}

void Board::play(bool display_gui) {
    //int curr_player = turn;
    //int play_next = players[turn]->move(die_rolls[die_index++], players, play_fields);
    bool play_next = players[turn]->move(dice, players, play_fields);

    //if (turn == 0) std::cout<<"Blue moved " << players[turn]->get_moves() << std::endl;
    //std::this_thread::sleep_for(std::chrono::microseconds(10));

    if(play_next) {

        turn++;
        if(turn == 4) turn = 0;

        for (unsigned int i = 0; i < play_fields.size(); i++) {
            Pawn* p = play_fields.at(i)->get_pawn();
            if (!p) {
                if (i == 0) play_fields.at(i)->set_Pixmap(QPixmap(":/images/bluestart.png"));
                else if (i == 10) play_fields.at(i)->set_Pixmap(QPixmap(":/images/redstart.png"));
                else if (i == 20) play_fields.at(i)->set_Pixmap(QPixmap(":/images/yellowstart.png"));
                else if (i == 30) play_fields.at(i)->set_Pixmap(QPixmap(":/images/greenstart.png"));
            }
            else {
                std::string color = p->get_color();
                switch (color[0]) {
                    case 'b':
                        play_fields.at(i)->set_Pixmap(QPixmap(":/images/bluepawn.png"));
                        break;
                    case 'r':
                        play_fields.at(i)->set_Pixmap(QPixmap(":/images/redpawn.png"));
                        break;
                    case 'y':
                        play_fields.at(i)->set_Pixmap(QPixmap(":/images/yellowpawn.png"));
                        break;
                    case 'g':
                        play_fields.at(i)->set_Pixmap(QPixmap(":/images/greenpawn.png"));
                        break;
                 }
            }
        }

        for (unsigned int i=4; i<blue_fields.size(); ++i) {

            if(!(blue_fields.at(i)->get_pawn()))
                blue_fields.at(i)->set_Pixmap(QPixmap(":/images/bluefinish.png"));
        }
        for (unsigned int i=4; i<red_fields.size(); ++i) {

            if(!(red_fields.at(i)->get_pawn()))
                red_fields.at(i)->set_Pixmap(QPixmap(":/images/redfinish.png"));
        }
        for (unsigned int i=4; i<yellow_fields.size(); ++i) {

            if(!(yellow_fields.at(i)->get_pawn()))
                yellow_fields.at(i)->set_Pixmap(QPixmap(":/images/yellowfinish.png"));
        }
        for (unsigned int i=4; i<green_fields.size(); ++i) {

            if(!(green_fields.at(i)->get_pawn()))
                green_fields.at(i)->set_Pixmap(QPixmap(":/images/greenfinish.png"));
        }

        if (display_gui) QTimer::singleShot(sim_time_in_ms, [this, display_gui] { play(display_gui); } );
        else play(display_gui);
    }
}
