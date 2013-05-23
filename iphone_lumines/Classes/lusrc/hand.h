#ifndef LUMINES_HAND_H
#define LUMINES_HAND_H

#import "block.h"
#import "lucolor.h"
#import "luminesrobot.h"

#define LUMINES_DEF_HAND_SPEED 60

enum btnkey
{
    BK_FIRST = 0,

    BK_LEFT = 0,
    BK_RIGHT,
    BK_UP,
    BK_DOWN,
    BK_SELECT,
    BK_START,
    BK_O,   // O circle
    BK_X,   // X cross
    BK_A,   // A triangle
    BK_M,   // M rectangle
    BK_L,
    BK_R,

    BK_LAST,
    BK_COUNT = BK_LAST,
};
class Lumines;
class Btnkey
{
public:
    Btnkey()
    {
        initBtnkey();
    }
    void initBtnkey()
    {
        memset(keyDown, 0, sizeof(keyDown));
        keybuffer.clear();
    }
    virtual void handleMessage(btnkey btn, bool down);
    bool getLastStatus(btnkey btn) const
    {
        assert(btn >= BK_FIRST && btn < BK_LAST);
        return keyDown[btn];
    }
    virtual void step();
    void addKey(btnkey btn)
    {
        keybuffer.push_back(btn);
    }
protected:
    bool keyDown[BK_COUNT];
    ::std::list< btnkey > keybuffer;
};

class Player: public Btnkey
{
public:
    typedef Btnkey Parent;
    Player();
    void init(Lumines * const _lumines, int _left, int _right, int _px, int _py);
    void handleMessage(btnkey btn, bool down);
    void step();

    void setLimit(int _left, int _right);

    void rotate(bool clockwise);
    void moveLeft();
    void moveRight();
    void move(int dx, int dy);
    void downStart(); // 下 触发
    void downLast();  // 下 持续按
    void downFree();  // 下 释放
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

    void updateScore(int add)
    {
        score += add;
        if (score > hi_score)
            hi_score = score;
    }
    int getScore() const {return score;}
    int getHiscore() const {return hi_score;}
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
    
    Lumines * const lumines;    // i will not change the lumines by this pointer

    int prepareX, prepareY; // nextblocks' position

    int throughCount;

    int score, hi_score;
};

class AiPlayer:public Player
{
    typedef Player Parent;
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