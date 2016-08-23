#ifndef GAMEMAP_H
#define GAMEMAP_H
#include "layer.h"
#include <QtGui>
#include <time.h>

class GameMap : public Layer
{
public:
    static const int MAP_ROW = 20;
    static const int MAP_COLUMN = 20;
    static const int PASSABLE_BLOCK_START = 19*20;//可以通过的地图块的起始标号
    int mMap[MAP_ROW][MAP_COLUMN];
public:
    GameMap(QPixmap *bmp, int fWidth, int fHeight, int layer_level);
    ~GameMap();
    void genMapData();
    void setCell(int row,int col,int tileIndex);
    int getCell(int row,int col);
    void tick();
    void paint(QPixmap *canvas);
};

#endif // GAMEMAP_H
