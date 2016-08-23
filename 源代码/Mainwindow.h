#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "layer.h"
#include "layermanager.h"
#include "gamemap.h"
#include "sprite.h"
#include "astarblock.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
private slots:
    void myUpdate();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *event);

    void tick();
    void paint();
    void initLayers();

//Layers
public:
    static const int FPS = 20;
    //GameState Constantant
    static const int NO_EVENT = 0;
    static const int GAMING = 1;
    static const int MAX_SPRITES_PER_MAP = 10;

    LayerManager *mLM;
    GameMap *mMap_1;
    GameMap *mMap_2;
    Sprite *mMe;
    Sprite* mSprite[MAX_SPRITES_PER_MAP];
    
//Pixmaps
public:
    QPixmap *mMapBMP1;
    QPixmap *mMapBMP2;
    QPixmap *mMeBMP;
public:
    AStarBlock* mAStar;

private:
    Ui::MainWindow *ui;
    QTimer *mUpdateTimer;
    QPixmap *mBoard;

};

#endif // MAINWINDOW_H
