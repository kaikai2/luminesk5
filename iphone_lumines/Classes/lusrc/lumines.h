#ifndef LUMINES_H
#define LUMINES_H

//#import <list>
#import <vector>
#import <eventmanager.h>

#import "block.h"
#import "hand.h"
#import "scanner.h"

#import "lueffmgr.h"
#import "lulevel.h"
#import "lusplitter.h"
#import <timegettime.h>

using std::vector;

enum PauseReason
{
    PR_NO_PAUSE = 0,
    PR_GAME_OVER = 2,
    PR_PAUSE = 4,
};
enum GameMode
{
    GM_SINGLE_CHALLENGE = 0,
    GM_MULTI_VS_CPU = 1,
    GM_MULTI_VS_2P = 2,
    GM_MULTI_VS_2CPU = 3,
};
class Lumines : public Sender, public Receiver
{
public:
    Lumines(ogl2d::Font *const fontlist[]):
        font(fontlist[0])
          ,scorefont(fontlist[1])
          ,width(16),height(10)
          //,combotext(scorefont)
          ,map(NULL)
          //,spectrum(16,20)
    {
        LuEffectManager::create(fontlist);
        bPause = PR_NO_PAUSE;
        init();
    }
    Lumines(ogl2d::Font *const fontlist[], int w, int h):
        font(fontlist[0]), 
            scorefont(fontlist[1]), 
            width(w),height(h),
            //combotext(scorefont),
            map(NULL)
            //,spectrum(w,h*2)
    {
        LuEffectManager::create(fontlist);
        bPause = PR_NO_PAUSE;
        init();
    }
    virtual ~Lumines();
    bool msgHandle(char key,bool msg);
    void step();
    void render();
    void start(GameMode _gamemode);
    void pause(DWORD reason, bool enable);

    int getScannerCount() const
    {
        return scanners.size();
    }
    int getScannerPos(size_t index) const
    {
        assert(index < scanners.size());
        assert(scanners[index] != NULL);
        return scanners[index]->getPos();
    }
    int getWidth(){return width;}
    int getHeight(){return height;}
    // int getThroughCount(){return scanner.getThroughCount();}
    bool canMoveBlockTo(int x, int y);
private:
    void init(GameMode _gamemode = GM_SINGLE_CHALLENGE);
    void genRandBlockGroup(block group[4]) const;
    block& getBlock(int i, int j) const;
    friend class Player;
    friend class AiPlayer;
    friend class Scanner;
    void reportDeleted(int throughCount);
    void handleEvent();

    void putBlock(int i, int j, const block &ablock);
    void gameover();
    void reload(Player &aHand);
    int scan_x(int i); // ��ͨ������ɨ
    int scan_y(int j); // ���⣬����ɨ
    int flash(); // һ����ֱ�����ٴ�������
    void destoryAllVisited();
    void floodFind(int i, int j);
    int floodFindLeader(int i, int j);

    int countBlock(btype type);

    void checkLevelUpdate(int count);
    void stepLevel();


    // bool ai_enable;

    int width, height;
    block *map;
    
    Player* hand;    // �������
    vector< Player* > players;  // �������
    vector< LuSplitter > splitters; // �ָ��ߣ��뵱ǰ��������һ��

    vector< Scanner* > scanners; // ɨ����
    vector< Boundary > boundarys; // �߽��ߣ���ɨ��������һֱ

    bool isGameover;
    bool mapchange;
    unsigned destoryed; // scan�õ�����Ҫdestory�ķ�������
    bool destory;       // �Ƿ�ʼdestory

    int level;
    int time;
    /*
    int score;
    int hi_score;
    */
    int deleted;        // ����������block����

    int deleted_singleblock;  // �����ĵ���block����

    int bonus;          // �ӱ�

    ogl2d::Font *const font;
    ogl2d::Font *const scorefont;
    int lastMapChangeBlockCount; // ���һ�ε�ͼ�仯ʱ���ֵ�������

    DWORD bPause;

    LuLevel skinLevel;
    bool m_bSoundEnabled;

    
    GameMode gamemode;
};

#endif