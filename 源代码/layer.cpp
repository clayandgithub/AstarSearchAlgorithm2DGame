#include "layer.h"

Layer::Layer(QPixmap *bmp, int fWidth, int fHeight,int level)
{
    //判断传入的参数是否合法
    try
    {
        bool temp_flag_illeagl_argument = false;
        if((bmp != NULL && (fWidth<=0||fHeight<=0)) || level < 0 || level >= TOP_LAYER_LEVEL)
        {
            temp_flag_illeagl_argument = true;
        }
        if(temp_flag_illeagl_argument)
            throw 1;
    }
    catch(int& value)
    {
        if(value==1)
        {
            qDebug("illeagl_argument!");
        }
    }
    mBitmap = bmp;
    mFWidth = fWidth;
    mFHeight = fHeight;
    mDstWidth = fWidth;
    mDstHeight = fHeight;
    mX = 0;
    mY = 0;
    mLayerLevel = level;
    mFrameXCor.clear();
    if(mBitmap == NULL)
    {
        //mBitmap为null表示此layer不包含图片,这也是允许的
        mFrameNum = 0;
    }
    else
    {
        initializeFrames(bmp,fWidth,fHeight);
    }
}

Layer::~Layer()
{}

void Layer::initializeFrames(QPixmap *bmp,int fWidth,int fHeight)
{
    //此函数将图像的每一个图块在图片上的x,y坐标分别存储在两个数组中
    int width = bmp->width();
    int height = bmp->height();
    int xnum = width/fWidth;
    int ynum = height/fHeight;
    mFrameNum = xnum*ynum;
    for(int i=0;i<mFrameNum;++i)
    {
        mFrameXCor.push_back((i%xnum)*fWidth);
    }
    for(int i=0;i<mFrameNum;++i)
    {
        mFrameYCor.push_back((i/xnum)*fHeight);
    }
}

//得到此layer一帧画面的高
int Layer::getHeight()
{
    return mFHeight;
}

//得到此layer一帧画面的宽
int Layer::getWidth()
{
    return mFWidth;
}

//设置此layer绘制时的宽
void Layer::setDstSize(int width, int height)
{
    if(width > 0 && height > 0)
    {
        mDstWidth = width;
        mDstHeight = height;
    }
}

int Layer::getDstWidth()
{
    return mDstWidth;
}

int Layer::getDstHeight()
{
    return mDstHeight;
}

int Layer::getDstRightBottomX()
{
    return mX + mDstWidth;
}

int Layer::getDstRightBottomY()
{
    return mY + mDstHeight;
}

int Layer::getX()
{
    return mX;
}

int Layer::getY()
{
    return mY;
}

//设置layer优先级
void Layer::setLayerLevel(int level)
{
    mLayerLevel = level;
}

//得到layer优先级
int Layer::getLayerLevel()
{
    return mLayerLevel;
}

//更换mBitmap的图片
void Layer::setBMP(QPixmap *newBmp)
{
    mBitmap = newBmp;
    if(mBitmap == NULL)
    {
        //mBitmap为null表示此layer不包含图片,这也是允许的
        mFrameNum = 0;
    }
}
