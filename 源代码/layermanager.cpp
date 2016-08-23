#include "layermanager.h"

LayerManager::LayerManager()
{
    mLayers.clear();
    setViewWindow(0, 0, 600, 600);
}

LayerManager::~LayerManager()
{
}

//设置窗口的大小和显示位置,用来实现滚屏
void LayerManager::setViewWindow(int x, int y, int width, int height)
{
    mViewX = x;
    mViewY = y;
    mViewWidth = width;
    mViewHeight = height;
}

//将图层添加到图层数组中的最后一个元素之后
void LayerManager::append(Layer *lr)
{
    mLayers.push_back(lr);
}

//移除已经存在的layer
void LayerManager::remove(Layer *lr)
{
    if(lr == NULL)
    {
        return;
    }
    vector <Layer*>::iterator iter;
    for(iter = mLayers.begin();iter!=mLayers.end();++iter)
    {
        if(*iter == lr)
        {
            mLayers.erase(iter);
        }
    }
}

//删除全部Layer
void LayerManager::removeAllLayers()
{
    mLayers.clear();
}

void LayerManager::bubbleSortByY()
{
    Layer *temp = NULL;
    for(int i = mLayers.size() - 1;i > 0;--i)
    {
        for(int j = 0;j < i;++j)
        {
            if(mLayers[j]->getY() > mLayers[j+1]->getY())
            {
                temp = mLayers[j];
                mLayers[j] = mLayers[j+1];
                mLayers[j+1] = temp;
            }
        }
    }
}
void LayerManager::tick()
{
    for(unsigned int i = 0;i < mLayers.size();++i)
    {
        if(mLayers[i] != NULL)
        {
            mLayers[i]->tick();
        }
    }

    //用冒泡排序法将layers按照y值从小到大进行排序  ( 这样就可以保证同一图层优先级的layers可以按照Y值来决定绘制的先后顺序---y小的先绘制 )
    bubbleSortByY();

//    //接下来设置观察窗口
//    if(MainActivity.mGameView.mMe != null)
//    {
//        if(MainActivity.mGameView.mMe.getY() - 5*MainActivity.BLOCK_SIZE < 0)
//            setViewWindow(0, 0, MainActivity.CANVAS_WIDTH, MainActivity.CANVAS_HEIGHT);
//        else if(MainActivity.mGameView.mMe.getY() - 5*MainActivity.BLOCK_SIZE >
//                    (GameMap.MAP_ROW * MainActivity.BLOCK_SIZE - MainActivity.CANVAS_HEIGHT))
//            setViewWindow(0, GameMap.MAP_ROW * MainActivity.BLOCK_SIZE - MainActivity.CANVAS_HEIGHT,
//                    MainActivity.CANVAS_WIDTH, MainActivity.CANVAS_HEIGHT);
//        else
//            setViewWindow(0, MainActivity.mGameView.mMe.getY() - 5 * MainActivity.BLOCK_SIZE,
//                    MainActivity.CANVAS_WIDTH, MainActivity.CANVAS_HEIGHT);
//    }
//    else
//    {
//        setViewWindow(0, 0, MainActivity.CANVAS_WIDTH, MainActivity.CANVAS_HEIGHT);
//    }
}

void LayerManager::paint(QPixmap *canvas,int x, int y)
{
//    canvas.translate(x - mViewX ,y - mViewY);
//    canvas.clipRect(mViewX, mViewY, mViewX+mViewWidth, mViewY+mViewHeight);
    //按顺序绘制每一层
    //绘制第0,1,2,3,4层
    for(int level = 0;level < Layer::TOP_LAYER_LEVEL; ++level)
    {
        for(unsigned int i = 0;i < mLayers.size(); ++i)
        {
            if(mLayers[i]!=NULL && mLayers[i]->getLayerLevel() == level)
            {
                mLayers[i]->paint(canvas);
            }
        }
    }
//    canvas.translate(mViewX - x,mViewY - y);
}
