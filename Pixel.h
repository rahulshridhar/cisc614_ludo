#ifndef PIXEL_H
#define PIXEL_H

#include <QGraphicsPixmapItem>

class Pixel : public QGraphicsPixmapItem {
public:
    Pixel(QGraphicsPixmapItem *parent) : pixmap(parent) {}
    void set_Pixmap(QPixmap image) { pixmap->setPixmap(image); }
    void set_Pixmap(QGraphicsPixmapItem *image) {pixmap->setPixmap(image->pixmap());}
private:
    QGraphicsPixmapItem *pixmap;
};

#endif // PIXEL_H
