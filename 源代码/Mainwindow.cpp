#include "Mainwindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(600,600);
    mBoard = new QPixmap(600,600);
    mUpdateTimer = new QTimer(this);
    connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
//    mState = NO_ANIMATE;
    initLayers();
    mUpdateTimer->start(1000/FPS);//update start
}

void MainWindow::initLayers()
{
    mAStar = new AStarBlock(this);
    mLM = new LayerManager();
    mMapBMP1 = new QPixmap("D:/img/map_tiles_1.png");
    mMapBMP2 = new QPixmap("D:/img/map_tiles_2.png");
    mMeBMP = new QPixmap("D:/img/temp_me.png");
    mMap_1 = new GameMap(mMapBMP1,Layer::BLOCK_SIZE,Layer::BLOCK_SIZE,0);
    mMap_2 = new GameMap(mMapBMP2,Layer::BLOCK_SIZE,Layer::BLOCK_SIZE,2);
    mMe = new Sprite(mMeBMP,Layer::BLOCK_SIZE,2*Layer::BLOCK_SIZE,1,Sprite::UP,Sprite::STANDING,0,this);
    mMe->setBlock(1010);
    mMe->setOccupiedArea(Layer::BLOCK_SIZE / 2, Layer::BLOCK_SIZE * 3 / 2, Layer::BLOCK_SIZE, Layer::BLOCK_SIZE);
    for(int i = 0;i<MAX_SPRITES_PER_MAP;++i)
    {
        mSprite[i] = NULL;
    }
    mLM->append(mMap_1);
    mLM->append(mMap_2);
    mLM->append(mMe);
}

MainWindow::~MainWindow()
{
    if(mUpdateTimer!=NULL)
    {
        delete mUpdateTimer;
        mUpdateTimer = NULL;
    }

    if(ui!=NULL)
    {
        delete ui;
        ui = NULL;
    }

    if(mLM!=NULL)
    {
        delete mLM;
        mLM = NULL;
    }
    if(mMap_1!=NULL)
    {
        delete mMap_1;
        mMap_1 = NULL;
    }
    if(mMap_2!=NULL)
    {
        delete mMap_2;
        mMap_2 = NULL;
    }
    if(mMapBMP1!=NULL)
    {
        delete mMapBMP1;
        mMapBMP1 = NULL;
    }
    if(mMapBMP2!=NULL)
    {
        delete mMapBMP2;
        mMapBMP2 = NULL;
    }
    if(mBoard!=NULL)
    {
        delete mBoard;
        mBoard = NULL;
    }
    if(mMeBMP!=NULL)
    {
        delete mMeBMP;
        mMeBMP = NULL;
    }
    if(mMe!=NULL)
    {
        delete mMe;
        mMe = NULL;
    }
    if(mAStar!=NULL)
    {
        delete mAStar;
        mAStar = NULL;
    }
}
void MainWindow::myUpdate()
{
    //mUpdateTimer->start(int(1000/FPS));//update start
    if(mMe->mIsLookingBlock)
    {
        qDebug("寻路开始...");
        //需要寻路,设置移动路线
        if(mAStar->FindPath(mMe->getFootBlock(),mMe->mLookingBlock))
        {
            for(unsigned int i = 0;i<mAStar->mPath.size();++i)
            {
                mMe->addRoute(mAStar->mPath[i]);
            }
            mMe->setDirection_Action(mMe->mAssignedRoutes[0],Sprite::WALKING);
        }
        else
        {
            qDebug("目的地不可达或已经到达目的地!");
        }
        mMe->mIsLookingBlock = false;
        qDebug("寻路结束!");
    }
    tick();
    paint();
}
void MainWindow::tick()
{
    mLM->tick();
}
void MainWindow::paint()
{
    mLM->paint(mBoard,0,0);
//    qDebug("papint zhi hou");
////test
//    QPainter painter(mBoard);
//    QPen pen(Qt::red);
//    pen.setWidth(16);
//    painter.setPen(pen);
//    mBoard->fill(Qt::black);
//    QFont font = QFont();
//    font.setPixelSize(26);
//    painter.setFont(font);
//    int i = 0;
//    QString temp_str = tr("%1").arg(i);
//    painter.drawText(200,200,temp_str);
////test
    update();
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter window_painter(this);
    window_painter.drawPixmap(0,0,*mBoard);
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(mMe->mChangeActionAble && mMe->mAssignedRoutes.size()==0)
    {
        int block_i = event->y()/Layer::BLOCK_SIZE;
        int block_j = event->x()/Layer::BLOCK_SIZE;
        int block = block_i*100+block_j;
        mMe->setLookingBlock(block);
    }
    else
    {
        qDebug("wait!!");
    }
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        mMe->setDirection_Action(Sprite::UP,Sprite::WALKING);
        break;
    case Qt::Key_Down:
        mMe->setDirection_Action(Sprite::DOWN,Sprite::WALKING);
        break;
    case Qt::Key_Left:
        mMe->setDirection_Action(Sprite::LEFT,Sprite::WALKING);
        break;
    case Qt::Key_Right:
        mMe->setDirection_Action(Sprite::RIGHT,Sprite::WALKING);
        break;
    case Qt::Key_Space:
        break;
    default:
        break;
    }
}
