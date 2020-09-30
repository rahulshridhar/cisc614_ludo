#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <Dice.h>

class Board : public QGraphicsScene {
private:
    QGraphicsScene* scene;
    Dice* dice;
public:
    Board();
    ~Board();
    void draw();
};

#endif // BOARD_H
