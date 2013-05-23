#ifndef LUMINES_H
#define LUMINES_H

#include <list>
#include <glCurvedAni.h>

#include "../gamelib/eventmanager.h"

#include "block.h"
#include "hand.h"
#include "scanner.h"

#include "../effect/lueffmgr.h"
#include "../level/lulevel.h"
#include "lusplitter.h"
#include "../ui/game.h"

enum btnkey
{
    BK_LEFT,
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
};
class Lumines : public Sender, public Receiver
{
public:
    Lumines(cAni::iAnimResManager &rARM/*ogl2d::Font *const fontlist[]*/):
        //font(fontlist[0])
        //  ,scorefont(fontlist[1])
          width(16),height(10)
          //,combotext(scorefont)
          ,map(NULL)
          //,spectrum(16,20)
    {
        LuEffectManager::create(rARM/*fontlist*/);
        hi_score = 0;
        bPause = PR_NO_PAUSE;
        init();
    }
    Lumines(cAni::iAnimResManager &rARM, /*ogl2d::Font *const fontlist[], */int w, int h):
        /*font(fontlist[0]), 
            scorefont(fontlist[1]), 
            */
            width(w),height(h),
            //combotext(scorefont),
            map(NULL)
            //,spectrum(w,h*2)
    {
		LuEffectManager::create(rARM/*fontlist*/);
        hi_score = 0;
        bPause = PR_NO_PAUSE;
        init();
    }
    virtual ~Lumines();
    bool msgHandle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void step();
    void render();
    void start(GameMode _gamemode);
    void pause(DWORD reason, bool enable);

    int getScannerX(){return scanner.getX();}
    int getWidth(){return width;}
    int getHeight(){return height;}
    int getThroughCount(){return scanner.getThroughCount();}
    bool canMoveBlockTo(int x, int y);
private:
    void init(GameMode _gamemode = GM_SINGLE_CHALLENGE);
    void genRandBlockGroup(block group[4]) const;
    block& getBlock(int i, int j) const;
    friend class Hand;
    friend class AiHand;
    friend class Scanner;
    void reportDeleted(int throughCount);
    void handleEvent();

    void putBlock(int i, int j, const block &ablock);
    void gameover();
    void reload(Hand &aHand);
    int scan(int i);
    void destoryAllVisited();
    void floodFind(int i, int j);
    int floodFindLeader(int i, int j);

    int countBlock(btype type);

    void updateScore(int add);
    void checkLevelUpdate(int count);
    void stepLevel();


    // bool ai_enable;

    int width, height;
    block *map;
    
    Hand hand;
    AiHand hand2;
    Scanner scanner;
    struct sKey
    {
        bool keyLeft;
        bool keyRight;
        //bool keyUp;
        bool keyDown;
        bool keyRotate; // clockwise
        bool keyRotate2;// anti-clockwise
    }key;
    ::std::list< btnkey > keybuffer;

    bool isGameover;
    bool mapchange;
    unsigned destoryed; // scan得到的需要destory的方块数量
    bool destory;       // 是否开始destory

    int level;
    int time;
    int score;
    int hi_score;
    int deleted;        // 消掉的联体block数量

    int deleted_singleblock;  // 消掉的单个block数量

    int bonus;          // 加倍

    /*ogl2d::Font *const font;
    ogl2d::Font *const scorefont;
    */
    int lastMapChangeBlockCount; // 最后一次地图变化时出现的联体数

    DWORD bPause;

    LuLevel skinLevel;
    bool m_bSoundEnabled;

    LuSplitter m_Splitter;
    GameMode gamemode;
};

#endif
