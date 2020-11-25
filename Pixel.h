#ifndef PIXEL_H
#define PIXEL_H

#include <QGraphicsPixmapItem>

class Pawn;

class Pixel : public QGraphicsPixmapItem {
public:
    Pixel(QGraphicsPixmapItem *parent) : pixmap(parent) {}
    void set_Pixmap(QPixmap image) { pixmap->setPixmap(image); }
    void set_Pixmap(QGraphicsPixmapItem *image) {pixmap->setPixmap(image->pixmap());}
    QGraphicsPixmapItem * getPixmap() { return pixmap; }
    void set_pawn(Pawn *new_pawn) { curr_pawns.push_back(new_pawn); }
    Pawn* get_pawn() { return curr_pawns.size() ? curr_pawns[0] : nullptr; }
    void remove_pawn() { curr_pawns.erase(curr_pawns.begin()); }
    bool is_empty() { return curr_pawns.size() == 0; }
    bool has_more_than_one_pawn() { return curr_pawns.size() > 1; }
private:
    std::vector<Pawn*> curr_pawns = {};
    QGraphicsPixmapItem *pixmap;
};

#endif // PIXEL_H
