#ifndef SPRITE_H
#define SPRITE_H
#include "layer.h"
#include "gamemap.h"
#include <QtGui>
#include<vector>

class MainWindow;//声明外部类
class Sprite : public Layer
{

public:
    //动作每一帧停顿的帧数,至少为1(最后一帧也要等待),可用于速度控制
    static const int FRAMES_PER_STANDING = 4;
    static const int FRAMES_PER_WALKING = 1;
    //动作循环的次数,至少为1(每播放完一次完整的动作(目前为4帧)为1次),也可用于速度控制
    static const int STANDING_LOOP_NUM = 1;
    //	static const int WALKING_LOOP_NUM = 1;//行走
    //每个方向的帧数
    static const int FRAMES_PER_DIRECTION = 4;
    //每个动作方向的数量（如“站立”这个动作有“上、下、左、右”共4个方向）
    static const int DIRECTIONS_NUM = 4;
    //四个方向常量(不可改变数字,会在某些地方用到对应的数字)
    static const int UP = 0;
    static const int DOWN = 1;
    static const int LEFT = 2;
    static const int RIGHT = 3;

    //sprite运动方式
    static const int STILL = 0;
    static const int RANDOM = 1;
    static const int CHASE_ME = 2;

    //动作状态
    static const int STANDING = 0;
    static const int WALKING = 1;

    //每移动一格所需要的步数//speed control
    static const int STEPS_PER_BLOCK = 3;
    //最大的占用block的数量
    static const int MAX_OCCUPIED_BLOCK_NUM = 0;
    //移动路线
    static const int MAX_ROUTES = 20;

    int mWaitFrameCount;//停顿帧数的计数变量
    int mLoopCount;//动作循环的计数变量

    int mDirection;//当前的方向
    int mAction;//当前的动作
    int mMoveKind;//运动方式（随机，接近或者静止）
    bool mActingSwitch;//帧动画开关
    bool mChangeActionAble;//当前能否更改正在做的动作（注：主要是为了sprite在移动完一格(或完成一个完整的动作之前)之前不能更改动作，否则会导致sprite的位置不正确）
    //由这两个变量确定绘制的图块
    int mCurNo;//当前图块在mBmp中的编号所属动作的第一个frame的编号
    int mCurFrame;//当前帧序列的编号(0,1,2,3)

    //speed control
    int mMoveTimes;//此次行走已移动的步数(执行了几次move)
    int mSpeed;//移动的速度(单位:像素/帧)

    int mID;//sprite的ID（保留ID: 0为自己，1为传送门，2为向上移动点  3为向下移动点 4为道具商人5为武器商人  6 为镇长）
    bool mVisible;//是否可见
    bool mPassable;//是否可穿透

    int mHeartOffesetX;//重心相对mX的偏移x坐标
    int mHeartOffesetY;//重心相对mY的偏移y坐标
    int mOccupiedWidth;//以重心为中心,所占的宽度,用于sprite的碰撞检测
    int mOccupiedHeight;//以重心为中心,所占的高度,用于sprite的碰撞检测
    vector<int> mAssignedRoutes;//设置移动路线
    MainWindow *mGameView;
    bool mIsLookingBlock;//是否正在寻路
    int mLookingBlock;//寻路的目的地

public:
    Sprite(QPixmap *bmp, int fWidth, int fHeight, int layer_Level, int direction,int action,int ID,MainWindow *gameView);
    ~Sprite();
    void tick();
    void paint(QPixmap *canvas);
    void addRoute(int direction);
    int find_direction();
    bool isRouteOver();
    bool isMoveAble(int direction);
    void move();
    void setDirection_Action(int direction, int action);
    void setCurFrame(int frame);
    int getCurFrame();
    void setCurNo(int no);
    bool getVisible();
    void setVisible(bool b);
    void setOccupiedArea(int heart_offset_x, int heart_offset_y, int oc_width, int oc_height);
    bool isCorOccupied(int x, int y);
    bool isBlockOccupied(int in_block);
    bool getPassable();
    void setPassable(bool b);
    void setBlock(int block);
    int getID();
    void setmPosition(int x, int y);
    int getX();
    int getY();
    int getRealX();
    int getRealY();
    int getDirection();
    int getAction();
    bool getActingSwitch();
    void setActingSwitch(bool b);
    bool nextFrame();
    bool prevFrame();
    QRect getOccupiedRect();
    int isBlockAround(int in_block);
    bool isFacingMe();
    int getFootBlock();
    bool isBlockPassAble(int block);
    void setLookingBlock(int endBlock);
};

#endif // SPRITE_H
