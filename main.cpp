#include <QApplication>
#include <QGraphicsView>
#include "Board.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Board *scene = new Board();

    QGraphicsView *view = new QGraphicsView(scene);
    view->show();
    view->setFixedSize(1000, 800);

    scene->setSceneRect(0, 0, 1000, 900);
    scene->draw();

    return a.exec();
}
