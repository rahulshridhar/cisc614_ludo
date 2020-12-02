#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <QObject>
#include <Pixel.h>
#include <Node.h>
#include <Player.h>
#include <QTimer>
#include <QGraphicsView>

class Board : public QGraphicsScene {
    //Q_OBJECT
private:
    //QGraphicsScene* scene;
    QGraphicsView* view;
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
    int play();
    void draw_pixel(std::vector<Pixel*>& vec, Node& curr_pos, Node next_move, QString img_path);
    int get_turn() { return turn; }

    const int pixel_size = 70;

    std::vector<Node> directions = {
        {0, -pixel_size}, //up
        {pixel_size, 0}, //right
        {0, pixel_size}, //down
        {-pixel_size, 0}  //left
    };

    std::vector<int> die_rolls = { 6, 6, 6, 6, 3, 6, 5, 3, 1, 3, 1, 4, 6, 1, 2, 5, 1, 1, 3, 5, 2, 6, 5, 5, 5, 5, 6,
                                   2, 5, 2, 3, 2, 5, 2, 1, 3, 3, 3, 3, 4, 6, 3, 6, 5, 3, 1, 3, 1, 4, 6, 1, 2, 5, 1,
                                   1, 3, 5, 2, 6, 5, 5, 5, 5, 6, 2, 5, 2, 3, 2, 5, 2, 1, 3, 3, 3, 3, 4, 6, 3, 6, 5,
                                   3, 1, 3, 1, 4, 6, 1, 2, 5, 1, 1, 3, 5, 2, 6, 5, 5, 5, 5, 6, 2, 5, 2, 3, 2, 5, 2,
                                   1, 3, 3, 3, 3, 4, 6, 6, 6, 6, 3, 6, 5, 3, 1, 3, 1, 4, 6, 1, 2, 5, 1, 1, 3, 5, 2,
                                   6, 5, 5, 5, 5, 6, 2, 5, 2, 3, 2, 5, 2, 1, 3, 3, 3, 3, 4, 6, 3, 6, 5, 3, 1, 3, 1,
                                   4, 6, 1, 2, 5, 1, 1, 3, 5, 2, 6, 5, 5, 5, 5, 6, 2, 5, 2, 3, 2, 5, 2, 1, 3, 3, 3,
                                   3, 4, 6, 3, 6, 5, 3, 1, 3, 1, 4, 6, 1, 2, 5, 1, 1, 3, 5, 2, 6, 5, 5, 5, 5, 6, 2,
                                   6, 6, 6, 6, 3, 6, 5, 3, 1, 3, 1, 4, 6, 1, 2, 5, 1, 1, 3, 5, 2, 6, 5, 5, 5, 5, 6,
                                   2, 5, 2, 3, 2, 5, 2, 1, 3, 3, 3, 3, 4, 6, 3, 6, 5, 3, 1, 3, 1, 4, 6, 1, 2, 5, 1,
                                   1, 3, 5, 2, 6, 5, 5, 5, 5, 6, 2, 5, 2, 3, 2, 5, 2, 1, 3, 3, 3, 3, 4, 6, 3, 6, 5,
                                   3, 1, 3, 1, 4, 6, 1, 2, 5, 1, 1, 3, 5, 2, 6, 5, 5, 5, 5, 6, 2, 5, 2, 3, 2, 5, 2,
                                   1, 3, 3, 3, 3, 4, 6, 6, 6, 6, 3, 6, 5, 3, 1, 3, 1, 4, 6, 1, 2, 5, 1, 1, 3, 5, 2,
                                   6, 5, 5, 5, 5, 6, 2, 5, 2, 3, 2, 5, 2, 1, 3, 3, 3, 3, 4, 6, 3, 6, 5, 3, 1, 3, 1,
                                   4, 6, 1, 2, 5, 1, 1, 3, 5, 2, 6, 5, 5, 5, 5, 6, 2, 5, 2, 3, 2, 5, 2, 1, 3, 3, 3,
                                   3, 4, 6, 3, 6, 5, 3, 1, 3, 1, 4, 6, 1, 2, 5, 1, 1, 3, 5, 2, 6, 5, 5, 5, 5, 6, 2,
                                   5, 2, 3, 2, 5, 2};
    int die_index = 0;
signals:
    void exit();
};

#endif // BOARD_H
