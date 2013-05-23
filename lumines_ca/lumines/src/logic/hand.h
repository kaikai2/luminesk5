#ifndef LUMINES_HAND_H
#define LUMINES_HAND_H

#include "block.h"
#include "../effect/lucolor.h"
#include "luminesrobot.h"

#define LUMINES_DEF_HAND_SPEED 60

class Lumines;

class Hand
{
public:
    Hand();
    void init(Lumines *_lumines, int _left, int _right, int _px, int _py);

    void setLimit(int _left, int _right);

    void rotate(bool clockwise);
    void moveLeft();
    void moveRight();
    void move(int dx, int dy);
    void downStart(); // 下 触发
    bool downLast();  // 下 持续按
    void downFree();  // 下 释放

    void step();
    void reload(int _x, const block blocks[4]);
    void render();
    void setFallSpeed(DWORD speed);
    
    BlockSet getBlockSet(){return blockset;}
    BlockSet getBlockSet(const block blocks[4])
    {
        return BlockSet(
            ((blocks[0].type == BT_B)?0x08:0) |
            ((blocks[1].type == BT_B)?0x04:0) |
            ((blocks[3].type == BT_B)?0x02:0) |
            ((blocks[2].type == BT_B)?0x01:0));
    }
    
    int getX(){return x;}
    int getY(){return y;}
    int getLeftLimit(){return left;}
    int getRightLimit(){return right;}

    virtual void aistep(){ }
    virtual void aithink(){ }

    void setThroughCount(int _throughCount)
    {
        assert(_throughCount >= 0);
        throughCount = _throughCount;
    }
    int getThroughCount(){return throughCount;}
protected:
    int hx; // delay count of x-axis movement
    int x, y;
    int left, right;
    bool moveBlockTo(int _x, int _y);
    void putBlocks();

    float /*px,*/ py;
    block newblock[4];
    block nextblock[4][4];

    BlockSet blockset;

    int delay; // reload后延时下落
    int tick;
    int fallspeed;
    
    Lumines *lumines;    // i will not change the lumines by this pointer

    int prepareX, prepareY; // nextblocks' position

    int throughCount;
};

class AiHand:public Hand
{
    typedef Hand Parent;
public:
    virtual void init(Lumines * const _lumines, int _left, int _right, int _px, int _py);
    virtual void aistep();

    virtual void aithink();
private:
    /// hand ai
    int ai_speed;
    int ai_maxspeed;

    int ai_tick;
    bool ai_valid;
    int ai_x, ai_rotate;
    LuminesRobot robot;
};
#endif//LUMINES_HAND_H
