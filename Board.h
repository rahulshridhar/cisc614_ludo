#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <QObject>
#include <Pixel.h>
#include <Node.h>
#include <Player.h>
#include <QTimer>
#include <QGraphicsView>
#include <QApplication>

class Board : public QGraphicsScene {
    Q_OBJECT
private:
    QGraphicsScene* scene;
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
    bool team_play;

public:
    Board(int game_num, bool team_play);
    virtual ~Board();
    void draw();
    void play(bool display_gui);
    void draw_pixel(std::vector<Pixel*>& vec, Node& curr_pos, Node next_move, QString img_path);
    int get_turn() { return turn; }
    std::vector<Player*> get_players() { return players; }
    QGraphicsScene* get_scene() { return scene; }
    int game_number = -1;

    const int pixel_size = 70;

    std::vector<Node> directions = {
        {0, -pixel_size}, //up
        {pixel_size, 0}, //right
        {0, pixel_size}, //down
        {-pixel_size, 0}  //left
    };
};

#endif // BOARD_H
