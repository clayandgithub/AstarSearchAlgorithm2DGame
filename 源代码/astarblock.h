#ifndef ASTARBLOCK_H
#define ASTARBLOCK_H
#include "gamemap.h"
#include <vector>
class MainWindow;

using namespace std;
class AStarBlock
{
public:
    int mG[20][20];
    int mH[20][20];
    int mF[20][20];
    int mParent[20][20];
//    int mParent[GameMap::MAP_ROW][GameMap::MAP_COLUMN];
    vector<int> mOpenList;
    vector<int> mCloseList;
    vector<int> mPath;

    MainWindow *mGameView;
public:
    AStarBlock(MainWindow *gameView);
    ~AStarBlock();
    bool FindPath(int startBlock,int endBlock);
    bool isInOpenList(int block);
    bool isInCloseList(int block);
};
#endif // ASTARBLOCK_H
