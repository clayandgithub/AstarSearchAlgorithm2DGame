#include "astarblock.h"
#include "sprite.h"
#include "Mainwindow.h"
#include <cmath>

AStarBlock::AStarBlock(MainWindow *gameView)
{
    mGameView = gameView;
    for(int i =0;i<20;++i)
    {
        for(int j =0;j<20;++j)
        {
            mG[i][j] = 0;
            mH[i][j] = 0;
            mF[i][j] = 0;
            mParent[i][j] = 0;
        }
    }
    mOpenList.clear();
    mCloseList.clear();
    mPath.clear();
}

AStarBlock::~AStarBlock()
{
}
bool AStarBlock::FindPath(int startBlock, int endBlock)
{
    if(!mGameView->mMe->isBlockPassAble(endBlock) || startBlock/100 < 0 || startBlock/100 >= GameMap::MAP_ROW
            || startBlock%100 < 0 || startBlock%100 >= GameMap::MAP_COLUMN
            || startBlock == endBlock)
    {//目标不可达
        return false;
    }

    //计算mH
    for(int i =0;i < GameMap::MAP_ROW;++i)
    {
        for(int j =0;j < GameMap::MAP_COLUMN;++j)
        {
            mH[i][j] = abs(endBlock/100 - i) + abs(endBlock%100 - j);
        }
    }

    //初始化开始block的数据
    mG[startBlock/100][startBlock%100] = 0;
    mF[startBlock/100][startBlock%100] = mG[startBlock/100][startBlock%100] + mH[startBlock/100][startBlock%100];
    mParent[startBlock/100][startBlock%100] = -1;//parent 为 -1 代表开始节点,其他的节点的parent都大于等于0

    mOpenList.clear();
    mCloseList.clear();
    mPath.clear();
    mOpenList.push_back(startBlock);

    bool bingo = false;
    int minFBlock = 0;
    int aroundMinFBlock = 0;
    vector<int>::iterator min_iter;
    int size = mOpenList.size();
    while(mOpenList.size()>0 && !bingo)
    {
        size = mOpenList.size();
        min_iter = mOpenList.begin();
        for(vector<int>::iterator iter = mOpenList.begin()+1;iter!=mOpenList.end();++iter)
        {
            if(mF[(*iter)/100][(*iter)%100] < mF[(*min_iter)/100][(*min_iter)%100])
            {
                min_iter = iter;
            }
        }
        minFBlock = *min_iter;
        // 把它从 "开启列表" 中删除, 并放到 "关闭列表" 中.
        mOpenList.erase(min_iter);
        mCloseList.push_back(minFBlock);

        //判断上下左右四个block并更新
        for(int i = 0;i < 4;++i)
        {
            switch(i)
            {
            case 0:
                //up
                aroundMinFBlock = minFBlock-100;
                break;
            case 1:
                //down
                aroundMinFBlock = minFBlock+100;
                break;
            case 2:
                //left
                aroundMinFBlock = minFBlock-1;
                break;
            case 3:
                //right
                aroundMinFBlock = minFBlock+1;
                break;
            }//switch end

            if(endBlock == aroundMinFBlock)
            {
                //找到当前目标节点
                bingo = true;
                mParent[(endBlock)/100][(endBlock)%100] = minFBlock;
                mOpenList.push_back(endBlock);
                break;
            }
            else if(isInOpenList(aroundMinFBlock))
            {
                //已经在openList中,则判断是否更新mG,mF和mParent
                if(mG[minFBlock/100][minFBlock%100] + 1 < mG[(aroundMinFBlock)/100][(aroundMinFBlock)%100])
                {
                    mG[(aroundMinFBlock)/100][(aroundMinFBlock)%100] = mG[minFBlock/100][minFBlock%100] + 1;
                    mF[(aroundMinFBlock)/100][(aroundMinFBlock)%100] = mG[(aroundMinFBlock)/100][(aroundMinFBlock)%100]+mH[(aroundMinFBlock)/100][(aroundMinFBlock)%100];
                    mParent[(aroundMinFBlock)/100][(aroundMinFBlock)%100] = minFBlock;
                }
            }
            else if(!isInCloseList(aroundMinFBlock) && mGameView->mMe->isBlockPassAble(aroundMinFBlock))
            {
                //不在openList中,但是不在closeList中且可以通行,则加入openList中去
                mG[(aroundMinFBlock)/100][(aroundMinFBlock)%100] = mG[minFBlock/100][minFBlock%100] + 1;
                mF[(aroundMinFBlock)/100][(aroundMinFBlock)%100] = mG[(aroundMinFBlock)/100][(aroundMinFBlock)%100]+mH[(aroundMinFBlock)/100][(aroundMinFBlock)%100];
                mParent[(aroundMinFBlock)/100][(aroundMinFBlock)%100] = minFBlock;
                mOpenList.push_back(aroundMinFBlock);
            }
        }//for end
    }//while end

    if(bingo)
    {
        int tempBlock = endBlock;
        int parent = mParent[tempBlock/100][tempBlock%100];
        while(parent != -1)
        {
            switch(tempBlock - parent)
            {
                case -100:
                    mPath.push_back(0);//Sprite::UP
                    break;
                case 100:
                    mPath.push_back(1);//Sprite::DOWN
                    break;
                case -1:
                    mPath.push_back(2);//Sprite::LEFT
                    break;
                case 1:
                    mPath.push_back(3);//Sprite::RIGHT
                    break;
                default:
                    break;
            }
            tempBlock = parent;
            parent = mParent[parent/100][parent%100];
        }

        //现在路径是反的,需要颠倒顺序
        int temp;
        int i = 0;
        int j = mPath.size()-1;
        for(i = 0;i < j;++i,--j)
        {
            temp = mPath[i];
            mPath[i] = mPath[j];
            mPath[j] = temp;
        }
    }
    return bingo;
}
bool AStarBlock::isInOpenList(int block)
{
    bool ret = false;
    for(unsigned int i = 0;i < mOpenList.size();++i)
    {
        if(mOpenList[i]==block)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

bool AStarBlock::isInCloseList(int block)
{
    bool ret = false;
    for(unsigned int i = 0;i < mCloseList.size();++i)
    {
        if(mCloseList[i]==block)
        {
            ret = true;
            break;
        }
    }
    return ret;
}
