#include "Board.h"
#include "Player.h"
#include <QGraphicsPixmapItem>

Board::Board()
{
    scene = new QGraphicsScene();
}

Board::~Board()
{
    for (auto& i : play_fields) delete i;

    delete scene;
}

// draw the ludo board
void Board::draw() {
    Node current_pos(350, 770);
    int i,j;

    // 10 fields per color
    for(i=0; i<4; ++i) { //drawing fields to play

        for(j=0; j<4; ++j)
            draw_pixel(play_fields, current_pos, directions[i], ":/images/border.png");

        //draw_pixel(play_fields, current_pos, directions[(i+3)%4], ":/images/border.png");
        //draw_pixel(play_fields, current_pos, directions[i], ":/images/border.png");

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

    Player *playerRed = new Player(play_fields.at(10), red_fields, 'r', 10);
    //red was drawn

    current_pos.x = 700;
    current_pos.y = 70;

    for(j=0; j<4; ++j)
        draw_pixel(yellow_fields, current_pos, directions[(j+1)%4], ":/images/yellowpawn.png");

    current_pos.x = 420;
    current_pos.y = 140;
    for(j=0; j<4; ++j)
        draw_pixel(yellow_fields, current_pos, directions[2], ":/images/yellowfinish.png");

    Player *playerYellow = new Player(play_fields.at(20), yellow_fields, 'y', 20);
    //yellow was drawn

    current_pos.x = 700;
    current_pos.y = 700;
    for(j=0; j<4; ++j)
        draw_pixel(green_fields, current_pos, directions[(j+1)%4], ":/images/greenpawn.png");

    current_pos.x = 700;
    current_pos.y = 420;
    for(j=0; j<4; ++j)
        draw_pixel(green_fields, current_pos, directions[3], ":/images/greenfinish.png");

    Player *playerGreen = new Player(play_fields.at(30), green_fields, 'g', 30);
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
    Player *playerBlue = new Player(play_fields.at(0), blue_fields, 'b', 0);

    players.push_back(playerBlue);
    players.push_back(playerRed);
    players.push_back(playerYellow);
    players.push_back(playerGreen);

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

