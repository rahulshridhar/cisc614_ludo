#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <Dice.h>
#include <Pixel.h>
#include <Node.h>
#include <Player.h>
#include <QTimer>

class Board : public QGraphicsScene {
private:
    QGraphicsScene* scene;
    Dice* dice;
    QTimer* timer;
    std::vector<Pixel*> play_fields;
    std::vector<Pixel*> red_fields;
    std::vector<Pixel*> blue_fields;
    std::vector<Pixel*> green_fields;
    std::vector<Pixel*> yellow_fields;
    std::vector<Pixel*> center_field;
    std::vector<Player*> players;
    int turn = 0;   //blue = 0, red = 1, yellow = 2, green = 3

public:
    Board();
    ~Board();
    void draw();
    void play();
    void draw_pixel(std::vector<Pixel*>& vec, Node& curr_pos, Node next_move, QString img_path);

    const int pixel_size = 70;

    std::vector<Node> directions = {
        {0, -pixel_size}, //up
        {pixel_size, 0}, //right
        {0, pixel_size}, //down
        {-pixel_size, 0}  //left
    };
};

#endif // BOARD_H
