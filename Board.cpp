#include "Board.h"
#include <QGraphicsPixmapItem>

Board::Board()
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 800, 600);
}

Board::~Board()
{
    delete scene;
}

// draw the ludo board
void Board::draw() {
    QPixmap img(":/images/border.png");
    QGraphicsPixmapItem *rect = this->addPixmap(img);
    rect->setPos(10, 20);
    QPixmap img1(":/images/blue.png");
    rect = this->addPixmap(img1);
    rect->setPos(100, 200);

    this->setBackgroundBrush(Qt::darkCyan);
}
