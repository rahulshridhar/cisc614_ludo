#ifndef PIXEL_H
#define PIXEL_H

#include <QGraphicsPixmapItem>

class Pawn;

class Pixel : public QGraphicsPixmapItem {
public:
    Pixel(QGraphicsPixmapItem *parent) : pixmap(parent) {}
    void set_Pixmap(QPixmap image) { pixmap->setPixmap(image); }
    void set_Pixmap(QGraphicsPixmapItem *image) {pixmap->setPixmap(image->pixmap());}
    void set_pawn(Pawn *new_pawn) { curr_pawn = new_pawn; }
    Pawn* get_pawn() { return curr_pawn; }
private:
    Pawn* curr_pawn = nullptr;
    QGraphicsPixmapItem *pixmap;
};

#endif // PIXEL_H
