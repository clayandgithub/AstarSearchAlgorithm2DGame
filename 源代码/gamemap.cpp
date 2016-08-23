#include "gamemap.h"

GameMap::GameMap(QPixmap *bmp, int fWidth, int fHeight, int layer_level):Layer(bmp,fWidth,fHeight,layer_level)
{
    for(int i = 0;i < MAP_ROW; ++i)
    {
        for(int j = 0;j < MAP_COLUMN; ++j)
        {
            mMap[i][j] = -1;
        }
    }
    genMapData();
}

GameMap::~GameMap()
{}

void GameMap::genMapData()
{
    srand(time(NULL));
    if(mLayerLevel == 0)
    {
        int rand_0_99 = 0;//100以下的随机数
        //固定的周围
        //two sides
        for(int i = 0;i < MAP_ROW-1;++i)
        {
            mMap[i][0] = 2;
            mMap[i][MAP_COLUMN-1] = 0;
        }
        //top
        for(int i = 0;i < 2;++i)
        {
            for(int j = 1;j < MAP_COLUMN-1;++j)
            {
                mMap[i][j] = 6;
            }
        }
        //top end
        for(int j = 1;j < MAP_COLUMN-1;++j)
        {
            mMap[2][j] = 26;
        }
        //corners
        mMap[MAP_ROW-1][0] = 4;
        mMap[MAP_ROW-1][MAP_COLUMN-1] = 5;

        for(int i = 3;i < MAP_ROW-1;++i)
        {
            for(int j = 1;j < MAP_COLUMN-1;++j)
            {
                rand_0_99 = rand()%100;
                if(rand_0_99 < 80)//80%
                {
                    mMap[i][j] = 420;//地面1
                }
                else if(rand_0_99 < 85)
                    mMap[i][j] = 320;//矿石1
                else if(rand_0_99 < 90)
                    mMap[i][j] = 321;//矿石2
                else if(rand_0_99 < 95)
                    mMap[i][j] = 322;//矿石3
                else if(rand_0_99 < 98)
                    mMap[i][j] = 280;//石头4
                else
                    mMap[i][j] = 281;//植物1
            }
        }
        for(int j = 1;j < MAP_COLUMN-1;++j)
        {
            mMap[MAP_ROW-1][j] = 420;
        }

        {
            //半随机入口
            int rand_1_13 = rand()%13+1;
            mMap[1][rand_1_13] = 300;
            mMap[2][rand_1_13] = 431;
            mMap[3][rand_1_13] = 420;
            //随机放在这里
        }
    }
    else
    {//(第二层图)
        for(int j = 1;j < MAP_COLUMN-1;++j)
        {
            mMap[MAP_ROW-1][j] = 1;
        }
        {
            //半随机出口
            int rand_2_13 = rand()%12+2;
            mMap[MAP_ROW-1][rand_2_13-1] = 2;
            mMap[MAP_ROW-1][rand_2_13] = -1;
            mMap[MAP_ROW-1][rand_2_13+1] = 0;
        }
    }
    //readFromFile();
}

void GameMap::setCell(int row,int col,int tileIndex)
{
    if(row >= 0 && col >= 0)
    {
        mMap[row][col] = tileIndex;
    }
}

int GameMap::getCell(int row,int col)
{
    if(row >= 0 && col >= 0)
    {
        return mMap[row][col];
    }
    return -1;
}
void GameMap::tick()
{
}
void GameMap::paint(QPixmap *canvas)
{
//	!!!!!这里面可以考虑添加一张背景bmp元素.直接先画这个bmp,再画图块,这里用黑色代替
//	在绘制第一层地图时用黑色清除之前的画布
//	canvas->fill(Qt::black);
    if(mBitmap == NULL)
    {
        return;
    }
    QPainter painter(canvas);
    for(int i = 0;i < MAP_ROW; ++i)
    {
        for(int j = 0;j < MAP_COLUMN; ++j)
        {
            if(mMap[i][j] >= 0)
            {
                painter.drawPixmap(mX+j*mFWidth,mY+i*mFHeight,mDstWidth,mDstHeight,
                                   *mBitmap,mFrameXCor[mMap[i][j]],mFrameYCor[mMap[i][j]],mFWidth,mFHeight);
            }
        }
    }
}
