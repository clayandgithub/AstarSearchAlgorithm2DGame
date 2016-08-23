#include "sprite.h"
#include "Mainwindow.h"

Sprite::Sprite(QPixmap *bmp, int fWidth, int fHeight, int layer_level,
               int direction,int action,int ID, MainWindow *gameView):Layer(bmp,fWidth,fHeight,layer_level)
{
    mWaitFrameCount=0;//停顿帧数的计数变量
    mLoopCount=0;//动作循环的计数变量

    mMoveKind=RANDOM;//运动方式（随机，接近或者静止）
    mActingSwitch=true;//帧动画开关
    mChangeActionAble=true;//当前能否更改正在做的动作（注：主要是为了sprite在移动完一格(或完成一个完整的动作之前)之前不能更改动作，否则会导致sprite的位置不正确）
    //由这两个变量确定绘制的图块
    mCurNo=0;//当前图块在mBmp中的编号所属动作的第一个frame的编号
    mCurFrame=0;//当前帧序列的编号(0,1,2,3)

    //speed control
    mMoveTimes=3;//此次行走已移动的步数(执行了几次move)
    mSpeed=10;//移动的速度(单位:像素/帧)

    mVisible = true;//是否可见
    mPassable = false;//是否可穿透
    mHeartOffesetX=fWidth/2;//重心相对mX的偏移x坐标
    mHeartOffesetY=fHeight/2;//重心相对mY的偏移y坐标
    mOccupiedWidth=fWidth;//以重心为中心,所占的宽度,用于sprite的碰撞检测
    mOccupiedHeight=fHeight;//以重心为中心,所占的高度,用于sprite的碰撞检测

    mID = ID;
    mAssignedRoutes.clear();
    setDirection_Action(direction,action);
    mGameView = gameView;
    mIsLookingBlock = false;//是否正在寻路
    mLookingBlock = -1;
}
Sprite::~Sprite()
{}
void Sprite::tick()
{
    if(mID!=0)//若不是主角,则自己决定方向和动作
    {
        //决定下一帧以及转到下一帧
        /*决定Sprite的动作和方向*************************************/
        switch(mMoveKind)
        {
            case STILL:
                break;
            case RANDOM:
                if(rand()%10 < 1)//行走
                    setDirection_Action(mDirection,WALKING);
                else if(rand()%10 < 2)//改变方向站着
                {
                    setDirection_Action(rand()%4,STANDING);
                }
                else//静止站着
                {
                    setDirection_Action(mDirection,STANDING);
                }
                break;
        }
    }

    /* *************************************/
    switch(mAction)
    {
    case STANDING:
        if(++mWaitFrameCount == FRAMES_PER_STANDING)
        {//等待帧数到,播放下一帧
            mWaitFrameCount = 0;
            if(mActingSwitch)
            {
                nextFrame();
            }
        }
        break;
    case WALKING:
        if(mMoveTimes == 0)
        {
            //在走第一步之前判断前方是否可以通行,
            //如果不可以通行,则转换方向
            if(!isMoveAble(mDirection))
            {
                if(++mWaitFrameCount == FRAMES_PER_WALKING)
                {
                    mWaitFrameCount = 0;
                    //等待帧数到,播放下一帧
                    int temp_direction = find_direction();
                    if(temp_direction == -1)
                    {//都不可通行
                        mChangeActionAble = true;
                        setDirection_Action(DOWN,STANDING);
                    }
                    else
                    {
                        mChangeActionAble = true;
                        setDirection_Action(temp_direction,WALKING);
                        mChangeActionAble = false;
                    }
                }
                break;//跳出switch(即结束此次tick)
            }
            else
            {//若前方可以通行,则真正开始移动
                mChangeActionAble = false;
            }
        }
        //因为走路以movetimes为结束标志,因此不需要判断动画是否到达最后一帧
        if(++mWaitFrameCount == FRAMES_PER_WALKING)
        {//等待帧数到,播放下一帧
            mWaitFrameCount = 0;
            if(mActingSwitch)
            {//播放下一帧动画
                nextFrame();
            }
        }
        //如果已经到达目标格,则结束此次行走
        if(mMoveTimes == STEPS_PER_BLOCK)
        {
            if(mAssignedRoutes.size()>0)
            {
                mAssignedRoutes.erase(mAssignedRoutes.begin());
                if(mAssignedRoutes.size() > 0)
                {
                    mChangeActionAble = true;
                    setDirection_Action(mAssignedRoutes[0],WALKING);
                    move();
                }
            }
            else
            {
                mChangeActionAble = true;
                setDirection_Action(mDirection,STANDING);
            }
        }
        else
        {//如果还没有到达目标格,则走一步
            move();
        }
        break;
    }
}
void Sprite::paint(QPixmap *canvas)
{
    if(!mVisible || mBitmap == NULL)
    {
        return;
    }
    QPainter painter(canvas);
    QRectF src = QRectF();//源头
    QRectF dst = QRectF();//目的地

    src.setLeft(mFrameXCor[mCurNo + mCurFrame]);
    src.setRight(src.left() + mFWidth);
    src.setTop(mFrameYCor[mCurNo + mCurFrame]);
    src.setBottom(src.top() + mFHeight);

    dst.setLeft(mX);
    dst.setTop(mY);
    dst.setRight(dst.left()+mFWidth);
    dst.setBottom(dst.top() + mFHeight);
    painter.drawPixmap(dst,*mBitmap,src);
}
void Sprite::addRoute(int direction)
{
    mAssignedRoutes.push_back(direction);
}

//寻找一个此sprite可以通行的方向,若没有则返回-1
int Sprite::find_direction()
{
    int ret_direction = -1;
    switch(mDirection)
    {
    case UP:
        if(isMoveAble(LEFT))
        {
            ret_direction = LEFT;
        }
        else if(isMoveAble(RIGHT))
        {
            ret_direction = RIGHT;
        }
        else if(isMoveAble(DOWN))
        {
            ret_direction = DOWN;
        }
        break;
    case DOWN:
        if(isMoveAble(RIGHT))
        {
            ret_direction = RIGHT;
        }
        else if(isMoveAble(LEFT))
        {
            ret_direction = LEFT;
        }
        else if(isMoveAble(UP))
        {
            ret_direction = UP;
        }
        break;
    case LEFT:
        if(isMoveAble(UP))
        {
            ret_direction = UP;
        }
        else if(isMoveAble(DOWN))
        {
            ret_direction = DOWN;
        }
        else if(isMoveAble(RIGHT))
        {
            ret_direction = RIGHT;
        }
        break;
    case RIGHT:
        //上
        if(isMoveAble(DOWN))
        {
            ret_direction = DOWN;
        }
        else if(isMoveAble(UP))
        {
            ret_direction = UP;
        }
        else if(isMoveAble(LEFT))
        {
            ret_direction = LEFT;
        }
        break;
    }
    return ret_direction;
}

bool Sprite::isRouteOver()
{
    return mAssignedRoutes.empty() && mAction == STANDING;
}

//判断此Sprite某一方向的block(s)是否可以通行
bool Sprite::isMoveAble(int direction)
{
    //!!!!这里的x和y与i,j的对应关系是相反的!!!!!!!切记切记!!!!
    int temp_x = mX + mHeartOffesetX - mOccupiedWidth / 2;//包围盒左上角的x
    int temp_y = mY + mHeartOffesetY - mOccupiedHeight / 2;//包围盒左上角的y
    int temp_block;//包围盒面对的第一个block
    int temp_num;//所面对的block的数量

    bool ret_moveAble = true;
    switch(direction)
    {
        case UP:
            temp_block = (temp_y / Layer::BLOCK_SIZE - 1) * 100 + temp_x / Layer::BLOCK_SIZE;
            temp_num = mOccupiedWidth / Layer::BLOCK_SIZE;
            for(int i = 0;i < temp_num;++i)
            {
                temp_block += i;
                if(!isBlockPassAble(temp_block))
                {
                    ret_moveAble = false;
                    break;
                }
            }
            break;
        case DOWN:
            temp_block = ((temp_y + mOccupiedHeight) / Layer::BLOCK_SIZE) * 100 + temp_x / Layer::BLOCK_SIZE;
            temp_num = mOccupiedWidth / Layer::BLOCK_SIZE;
            for(int i = 0;i < temp_num;++i)
            {
                temp_block += i;
                if(!isBlockPassAble(temp_block))
                {
                    ret_moveAble = false;
                    break;
                }
            }
            break;
        case LEFT:
            temp_block = temp_y / Layer::BLOCK_SIZE * 100 + temp_x / Layer::BLOCK_SIZE - 1;
            temp_num = mOccupiedHeight / Layer::BLOCK_SIZE;
            for(int i = 0;i < temp_num;++i)
            {
                temp_block += i * 100;
                if(!isBlockPassAble(temp_block))
                {
                    ret_moveAble = false;
                    break;
                }
            }
            break;
        case RIGHT:
            temp_block = temp_y / Layer::BLOCK_SIZE * 100 + (temp_x + mOccupiedWidth) / Layer::BLOCK_SIZE;
            temp_num = mOccupiedHeight / Layer::BLOCK_SIZE;
            for(int i = 0;i < temp_num;++i)
            {
                temp_block += i * 100;
                if(!isBlockPassAble(temp_block))
                {
                    ret_moveAble = false;
                    break;
                }
            }
            break;
    }
    return ret_moveAble;
}
//移动一步的函数
void Sprite::move()
{
    switch(mDirection)
    {
        case UP:
            mMoveTimes += 1;
            mY -= mSpeed;
            break;
        case DOWN:
            mMoveTimes += 1;
            mY += mSpeed;
            break;
        case LEFT:
            mMoveTimes += 1;
            mX -= mSpeed;
            break;
        case RIGHT:
            mMoveTimes += 1;
            mX += mSpeed;
            break;
    }
}

//此函数改变sprite的动作和方向
void Sprite::setDirection_Action(int direction, int action)
{
    if(!mChangeActionAble)
        return;
    mWaitFrameCount = 0;
    mLoopCount = 0;
    mCurFrame = 0;
    mMoveTimes = 0;
    if(direction == UP || direction == DOWN || direction == RIGHT || direction == LEFT )
    {
        switch(action)
        {
        case STANDING:
            mDirection = direction;
            mAction = action;
            setCurNo(STANDING * DIRECTIONS_NUM * FRAMES_PER_DIRECTION + FRAMES_PER_DIRECTION * direction);
            break;
        case WALKING:
            mDirection = direction;
            mAction = action;
            setCurNo(WALKING * DIRECTIONS_NUM * FRAMES_PER_DIRECTION + FRAMES_PER_DIRECTION * direction);
            break;
        default:
            break;
        }
    }
}

void Sprite::setCurFrame(int frame)
{
    if(frame < 0 || frame > FRAMES_PER_DIRECTION)
        return;
    else
        mCurFrame = frame;
}
int Sprite::getCurFrame()
{
    return mCurFrame;
}

void Sprite::setCurNo(int no)
{
    mCurNo = no;
}

bool Sprite::getVisible()
{
    return mVisible;
}

void Sprite::setVisible(bool b)
{
    mVisible = b;
}

void Sprite::setOccupiedArea(int heart_offset_x, int heart_offset_y, int oc_width, int oc_height)
{
    mHeartOffesetX = heart_offset_x;
    mHeartOffesetY = heart_offset_y;
    mOccupiedWidth = oc_width;
    mOccupiedHeight = oc_height;
}

//传入坐标,判断此sprite是否占有此坐标(不包含边界),用于碰撞检测
bool Sprite::isCorOccupied(int x, int y)
{
    bool ret = false;
    //当此sprite不可见或者可穿透时返回false
    if(mPassable || !mVisible)
    {
        ret = false;
    }
    else
    {
        if(x > mX + mHeartOffesetX - mOccupiedWidth / 2 && x < mX + mHeartOffesetX + mOccupiedWidth / 2
                && y > mY + mHeartOffesetY - mOccupiedHeight / 2 && y < mY + mHeartOffesetY + mOccupiedHeight / 2)
        {
            ret = true;
        }
    }
    return ret;
}

//传入block,判断此sprite是否占有此block
bool Sprite::isBlockOccupied(int in_block)
{
    bool ret = false;
    //当此sprite不可见或者可穿透时返回false
    if(mPassable || !mVisible)
    {
        ret = false;
    }
    else
    {
        int block_leftX = (in_block % 100) * Layer::BLOCK_SIZE;//block的中间点坐标
        int block_leftY = (in_block / 100) * Layer::BLOCK_SIZE;
        QRect rect = getOccupiedRect();
        if(rect.intersects(QRect(block_leftX,block_leftY,Layer::BLOCK_SIZE,Layer::BLOCK_SIZE)))
        {
            ret = true;
        }
    }
    return ret;
}

bool Sprite::getPassable()
{
    return mPassable;
}

void Sprite::setPassable(bool b)
{
    mPassable = b;
}

//(block(格子)用一个整数来表示,其值为 :  行数i*100 + 列数j  例如第10行第8列就是1008,第11行第12列就是1112)

void Sprite::setBlock(int block)
{
    int block_i = block / 100;
    int block_j = block % 100;
    mX = block_j * Layer::BLOCK_SIZE;
    mY = block_i * Layer::BLOCK_SIZE;
}

int Sprite::getID()
{
    return mID;
}

void Sprite::setmPosition(int x, int y)
{
    mX = x;
    mY = y;
}

//返回的是重心的坐标
int Sprite::getX()
{
    return mX + mHeartOffesetX;
}

int Sprite::getY()
{
    return mY + mHeartOffesetY;
}

//返回图片左上角的x坐标
int Sprite::getRealX()
{
    return mX;
}
//返回图片左上角的y坐标
int Sprite::getRealY()
{
    return mY;
}

int Sprite::getDirection()
{
    return mDirection;
}

int Sprite::getAction()
{
    return mAction;
}

bool Sprite::getActingSwitch()
{
    return mActingSwitch;
}

void Sprite::setActingSwitch(bool b)
{
    mActingSwitch = b;

}

//播放下一帧(到末尾自动转到第一帧)
bool Sprite::nextFrame()
{
    if(mCurFrame + 1 >= FRAMES_PER_DIRECTION)
    {
        setCurFrame(0);
        return false;
    }
    else
    {
        setCurFrame(mCurFrame + 1);
        return true;
    }
}


//播放前一帧(当播放到开头自动转到结尾帧)
bool Sprite::prevFrame()
{
    if(mCurFrame - 1 < 0)
    {
        setCurFrame(FRAMES_PER_DIRECTION-1);
        return false;
    }
    else
    {
        setCurFrame(mCurFrame - 1);
        return true;
    }
}

QRect Sprite::getOccupiedRect()
{
    if(mPassable || !mVisible)
    {
        return QRect(0,0,0,0);
    }
    return QRect(mX + mHeartOffesetX - mOccupiedWidth / 2,mY + mHeartOffesetY - mOccupiedHeight / 2,
            mOccupiedWidth, mOccupiedHeight);
}

//判断block是否为此sprite的周围的格子//若是则返回此格在此sprite包围盒的什么方向(UP  0,DOWN  1,LEFT  2,RIGHT  3)
int Sprite::isBlockAround(int in_block)
{
    //怪物必须处在非移动状态中才进行判断
    if(mX % Layer::BLOCK_SIZE != 0 || mY % Layer::BLOCK_SIZE != 0 )
    {
        return -1;//-1表示in_block不在周围
    }
    int temp_x = mX + mHeartOffesetX - mOccupiedWidth / 2;//包围盒左上角的x
    int temp_y = mY + mHeartOffesetY - mOccupiedHeight / 2;//包围盒左上角的y
    int temp_block;//包围盒面对的第一个block
    int temp_num;//所面对的block的数量
    //UP
    temp_block = (temp_y / Layer::BLOCK_SIZE - 1) * 100 + temp_x / Layer::BLOCK_SIZE;
    temp_num = mOccupiedWidth / Layer::BLOCK_SIZE;
    for(int i = 0;i < temp_num;++i)
    {
        temp_block += i;
        if(in_block == temp_block)
        {
            return UP;
        }
    }
    //DOWN
    temp_block = ((temp_y + mOccupiedHeight) / Layer::BLOCK_SIZE) * 100 + temp_x / Layer::BLOCK_SIZE;
    temp_num = mOccupiedWidth / Layer::BLOCK_SIZE;
    for(int i = 0;i < temp_num;++i)
    {
        temp_block += i;
        if(in_block == temp_block)
        {
            return DOWN;
        }
    }
    // LEFT:
    temp_block = temp_y / Layer::BLOCK_SIZE * 100 + temp_x / Layer::BLOCK_SIZE - 1;
    temp_num = mOccupiedHeight / Layer::BLOCK_SIZE;
    for(int i = 0;i < temp_num;++i)
    {
        temp_block += i * 100;
        if(in_block == temp_block)
        {
            return LEFT;
        }
    }
    // RIGHT:
    temp_block = temp_y / Layer::BLOCK_SIZE * 100 + (temp_x + mOccupiedWidth) / Layer::BLOCK_SIZE;
    temp_num = mOccupiedHeight / Layer::BLOCK_SIZE;
    for(int i = 0;i < temp_num;++i)
    {
        temp_block += i * 100;
        if(in_block == temp_block)
        {
            return RIGHT;
        }
    }
    return -1;
}
bool Sprite::isFacingMe()
{
    return false;
//    return isBlockAround(mGameView->mMe.getFootBlock()) == mDirection;
}


//判断一个block是否可以通行
bool Sprite::isBlockPassAble(int block)
{
    //若超过map显示范围,则返回false
    if(block/100 < 0 || block/100 >= GameMap::MAP_ROW
            || block%100 < 0 || block%100 >= GameMap::MAP_COLUMN)
    {
        return false;
    }

    //先检测地图的碰撞
    if(mGameView->mMap_1->getCell(block/100,block%100) < GameMap::PASSABLE_BLOCK_START)
        return false;
    else
    {
        //检测sprites间的碰撞 (包括monsters和主角)
        //只在第一步时检测
        for(int spriteNo = 0;spriteNo < MainWindow::MAX_SPRITES_PER_MAP; ++spriteNo)
        {
            if(mGameView->mSprite[spriteNo] != NULL
                    && this != mGameView->mSprite[spriteNo]
                    && mGameView->mSprite[spriteNo]->isBlockOccupied(block))
            {
                return false;
            }
        }
//        if(this != mGameView->mMe && (mGameView->mMe.isBlockOccupied(block)))
//        {
//            return false;
//        }
    }
    return true;
}
void Sprite::setLookingBlock(int endBlock)
{
    mIsLookingBlock = true;
    mLookingBlock = endBlock;
}

int Sprite::getFootBlock()
{
    int i;
    int j;
    if(mFHeight>Layer::BLOCK_SIZE)
    {
        i = mY/Layer::BLOCK_SIZE + 1;//foot_i
    }
    else
    {
        i = mY/Layer::BLOCK_SIZE;//foot_i
    }
    j = mX/Layer::BLOCK_SIZE;//foot_j
    return i * 100 + j;
}
