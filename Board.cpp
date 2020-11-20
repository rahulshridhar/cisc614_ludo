#include "Board.h"
#include "Player.h"
#include <QGraphicsPixmapItem>

Board::Board()
{
    scene = new QGraphicsScene();
    dice = new Dice();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

Board::~Board()
{
    for (auto& i : play_fields) delete i;
    for (int i = 0; i < red_fields.size(); i++) {
        delete red_fields[i];
        delete blue_fields[i];
        delete green_fields[i];
        delete yellow_fields[i];
    }
    for (auto& p : players) delete p;

    delete dice;
    delete timer;
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

    Player *player_red = new Player(play_fields.at(10), red_fields, "red", 10);
    //red was drawn

    current_pos.x = 700;
    current_pos.y = 70;

    for(j=0; j<4; ++j)
        draw_pixel(yellow_fields, current_pos, directions[(j+1)%4], ":/images/yellowpawn.png");

    current_pos.x = 420;
    current_pos.y = 140;
    for(j=0; j<4; ++j)
        draw_pixel(yellow_fields, current_pos, directions[2], ":/images/yellowfinish.png");

    Player *player_yellow = new Player(play_fields.at(20), yellow_fields, "yellow", 20);
    //yellow was drawn

    current_pos.x = 700;
    current_pos.y = 700;
    for(j=0; j<4; ++j)
        draw_pixel(green_fields, current_pos, directions[(j+1)%4], ":/images/greenpawn.png");

    current_pos.x = 700;
    current_pos.y = 420;
    for(j=0; j<4; ++j)
        draw_pixel(green_fields, current_pos, directions[3], ":/images/greenfinish.png");

    Player *player_green = new Player(play_fields.at(30), green_fields, "green", 30);
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
    Player *player_blue = new Player(play_fields.at(0), blue_fields, "blue", 0);

    players.push_back(player_blue);
    players.push_back(player_red);
    players.push_back(player_yellow);
    players.push_back(player_green);

     //Set background to a dark color
     this->setBackgroundBrush(Qt::darkCyan);
}

void Board::draw_pixel(std::vector<Pixel *> &vec, Node &curr_pos, Node next_move, QString images_path)
{
     QPixmap images(images_path);
     QGraphicsPixmapItem *pix = this->addPixmap(images);
     pix->setPos(curr_pos.x, curr_pos.y);
     vec.push_back(new Pixel(pix));

     curr_pos.x += next_move.x;
     curr_pos.y += next_move.y;
}

void Board::play() {
    int play_next = players[turn]->move(dice, players, play_fields);

    if(play_next) {

        turn++;
        if(turn == 4) turn = 0;

        if(!(play_fields.at(0)->get_pawn()))
            play_fields.at(0)->set_Pixmap(QPixmap(":/images/bluestart.png"));
        if(!(play_fields.at(10)->get_pawn()))
            play_fields.at(10)->set_Pixmap(QPixmap(":/images/redstart.png"));
        if(!(play_fields.at(20)->get_pawn()))
            play_fields.at(20)->set_Pixmap(QPixmap(":/images/yellowstart.png"));
        if(!(play_fields.at(30)->get_pawn()))
            play_fields.at(30)->set_Pixmap(QPixmap(":/images/greenstart.png"));

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

        QTimer::singleShot(200, [this] { play(); } );
    }
}
