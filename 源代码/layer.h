#ifndef LAYER_H
#define LAYER_H

#include <QDebug>
#include <QPixmap>
#include <vector>
using namespace std;
//抽象类
class Layer
{
public:
    //0层为地图,1层为精灵,2层为第二层地图,3层为图片,动画图等,4层为最高层(上下左右,HIT,快捷键,菜单按钮等)
    static const int TOP_LAYER_LEVEL = 5;
    static const int BLOCK_SIZE = 30;

    QPixmap *mBitmap;//目标图片;
    int mFWidth;//图块宽度
    int mFHeight;//图块高度
    int mDstWidth;//图块绘制宽度
    int mDstHeight;//图块绘制高度
    int mX;//此Layer的左上角在屏幕上的x坐标
    int mY;//此Layer的左上角在屏幕上的y坐标
    int mLayerLevel;//layer的绘制优先级1,2,3,4,5......1在最底层,最先绘制,依此类推
    int mFrameNum;//总的图块数量
    vector<int> mFrameXCor;//所有图块的在图片上的x坐标
    vector<int> mFrameYCor;//所有图块的在图片上的y坐标
public:
    Layer(QPixmap *bmp, int fWidth, int fHeight,int level);
    ~Layer();
    void initializeFrames(QPixmap *bmp,int fWidth,int fHeight);
    int getHeight();
    int getWidth();
    void setDstSize(int width, int height);
    int getDstWidth();
    int getDstHeight();
    int getDstRightBottomX();
    int getDstRightBottomY();
    virtual int getX();
    virtual int getY();
    void setLayerLevel(int level);
    int getLayerLevel();
    void setBMP(QPixmap *newBmp);
    virtual void paint(QPixmap *canvas)=0;//绘制函数(纯虚函数)
    virtual void tick()=0;//下一帧动作(纯虚函数)
};

#endif // LAYER_H
