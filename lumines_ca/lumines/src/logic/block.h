#ifndef LUMINES_BLOCK_H
#define LUMINES_BLOCK_H

#define LUMINES_BLOCK_SIZE 16
const int size = LUMINES_BLOCK_SIZE + 1;

#include <glCurvedAni.h>

using cAni::iAnimation;

enum btype
{
    BT_NONE,
    BT_BAD,     // out
    BT_A,
    BT_B,
};
/*  BlockSet
the set of blocks in hand
BS_AAAB will be:
    AA
    AB
BS_ABAA will be:
    AB
    AA
*/
enum BlockSet
{
    BS_AAAA = 0,
    BS_AAAB,
    BS_AABA,
    BS_AABB,
    BS_ABAA,
    BS_ABAB,
    BS_ABBA,
    BS_ABBB,
    BS_BAAA,
    BS_BAAB,
    BS_BABA,
    BS_BABB,
    BS_BBAA,
    BS_BBAB,
    BS_BBBA,
    BS_BBBB,
};
extern const BlockSet s_rotmap[16][4];

class block
{
public:
    block():type(BT_NONE),special(false),destory(false),united(false),leader(false),x(0),y(0),destx(0),desty(0),/*vx(0), vy(0),*/flood(false),floodleader(false)
    {
        //anim = iSystem::GetInstance()->createAnimation();
    }
    block(btype bt):type(bt),special(false),destory(false),united(false),leader(false),x(0),y(0),destx(0),desty(0),/*vx(0), vy(0),*/flood(false),floodleader(false)
    {
        //anim = iSystem::GetInstance()->createAnimation();
    }
    ~block()
    {
        //iSystem::GetInstance()->release(anim);
        //anim = 0;
    }
    block &operator = (const block &rhs)
    {
        type = rhs.type;
        special = rhs.special; // special block has ability of what?
        destory = rhs.destory;
        united = rhs.united; // 
        leader = rhs.leader; // become leader block, the top-left one of the four united blocks
        flood = rhs.flood;       // flood visited
        floodleader = rhs.floodleader; // leader flood visited 
        leaderStartTime = rhs.leaderStartTime;
        x = rhs.x;
        y = rhs.y;
        destx = rhs.destx;
        desty = rhs.desty;
        speed = rhs.speed;
        // anim = rhs.anim; // no anim
        return *this;
    }

    btype type; 
    void setPos(int _x, int _y)
    {
        destx = x = _x;
        desty = y = _y;
    }
    void moveTo(int _x, int _y, int _speed = 8)//, int _evenmove = 0)
    {
        destx = _x;
        desty = _y;
        speed = _speed;
        //evenmove = _evenmove;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    bool step();
    void rand();
    void render(int pass, const cAni::Rect *clipper = NULL) const;
    void drawleader() const;
    bool special:1; // special block has ability of what?
    bool destory:1;
    bool united:1; // 
    bool leader:1; // become leader block, the top-left one of the four united blocks
    bool flood:1;       // flood visited
    bool floodleader:1; // leader flood visited 
    int leaderStartTime;
private:
    int x, y;
    int destx, desty;
    //int vx, vy;
    int speed;
    //iAnimation *anim;
    //int evenmove; // 1 if move speed is a constant, 0 if not
};

#endif//LUMINES_BLOCK_H
