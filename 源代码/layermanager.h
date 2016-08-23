#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "layer.h"

class LayerManager
{
public:
    vector<Layer*> mLayers;
    int mViewX;
    int mViewY;
    int mViewWidth;
    int mViewHeight; // the view window co-ordinates.


public:
    LayerManager();
    ~LayerManager();
    void setViewWindow(int x, int y, int width, int height);
    void append(Layer *lr);
    void remove(Layer *lr);
    void removeAllLayers();
    void bubbleSortByY();
    void tick();
    void paint(QPixmap *canvas,int x, int y);
};

#endif // LAYERMANAGER_H
