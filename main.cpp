#include <QApplication>
#include <QGraphicsView>
#include "Board.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Board *scene = new Board();

    QGraphicsView *view = new QGraphicsView(scene);
    view->show();
    view->setFixedSize(800, 600);

    scene->draw();

    return a.exec();
}
