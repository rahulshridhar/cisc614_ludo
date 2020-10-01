#ifndef PIXEL_H
#define PIXEL_H

#include <QGraphicsPixmapItem>

class Pixel : public QGraphicsPixmapItem {
public:
    Pixel(QGraphicsPixmapItem *parent) : pixmap(parent) {}
private:
    QGraphicsPixmapItem *pixmap;
};

#endif // PIXEL_H
