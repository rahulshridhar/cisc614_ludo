#include "Board.h"
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
    Node curr_pos(535, 15);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            draw_pixel(play_fields, curr_pos, directions[i], ":/images/border.png");
        }
    }
    for (int j = 0; j < 2; j++) draw_pixel(play_fields, curr_pos, directions[0], ":/images/border.png");
    for (int j = 0; j < 4; j++) draw_pixel(play_fields, curr_pos, directions[3], ":/images/border.png");
    for (int j = 0; j < 4; j++) draw_pixel(play_fields, curr_pos, directions[0], ":/images/border.png");
    for (int j = 0; j < 2; j++) draw_pixel(play_fields, curr_pos, directions[3], ":/images/border.png");
    for (int i = 2; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            draw_pixel(play_fields, curr_pos, directions[i], ":/images/border.png");
        }
    }

    for (int j = 0; j < 2; j++) draw_pixel(play_fields, curr_pos, directions[2], ":/images/border.png");

    for (int i = 1; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            draw_pixel(play_fields, curr_pos, directions[i], ":/images/border.png");
        }
    }

    for (int j = 0; j < 2; j++) draw_pixel(play_fields, curr_pos, directions[1], ":/images/border.png");

    //Setup home bases for all players
    Node yellow_base(700, 70);
    for (int j = 0; j < 4; j++) draw_pixel(yellow_fields, yellow_base, directions[j], ":/images/yellowpawn.png");

    Node green_base(700, 600);
    for (int j = 0; j < 4; j++) draw_pixel(green_fields, green_base, directions[j], ":/images/greenpawn.png");

    Node blue_base(140, 600);
    for (int j = 0; j < 4; j++) draw_pixel(blue_fields, blue_base, directions[j], ":/images/bluepawn.png");

    Node red_base(140, 70);
    for (int j = 0; j < 4; j++) draw_pixel(red_fields, red_base, directions[j], ":/images/redpawn.png");

    //Setup finish locations for all players
    Node yellow_finish(465, 85);
    for (int j = 0; j < 4; j++) draw_pixel(yellow_fields, yellow_finish, directions[0], ":/images/yellowfinish.png");

    Node green_finish(535, 365);
    for (int j = 0; j < 4; j++) draw_pixel(green_fields, green_finish, directions[1], ":/images/greenfinish.png");

    Node blue_finish(465, 435);
    for (int j = 0; j < 4; j++) draw_pixel(blue_fields, blue_finish, directions[0], ":/images/bluefinish.png");

    Node red_finish(395, 365);
    for (int j = 0; j < 4; j++) draw_pixel(red_fields, red_finish, directions[3], ":/images/redfinish.png");

     // draw center pixel
     Node center_pos(465, 365);
     Node center_no_next(0, 0);
     draw_pixel(center_field, center_pos, center_no_next, ":/images/all.png");

     //Set background to a dark color
     this->setBackgroundBrush(Qt::darkCyan);
}

void Board::draw_pixel(std::vector<Pixel *> &vec, Node &curr_pos, Node next_move, QString img_path)
{
     QPixmap img(img_path);
     QGraphicsPixmapItem *pix = this->addPixmap(img);
     pix->setPos(curr_pos.x, curr_pos.y);
     vec.push_back(new Pixel(pix));

     curr_pos.x += next_move.x;
     curr_pos.y += next_move.y;
}

