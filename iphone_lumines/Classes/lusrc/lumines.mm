#import <cstdio>
//#import <windows.h>
#import <ogl2d.h>
#import "sound.h"
#import "lumines.h"
#import "menu.h"
#import "menuitem.h"
#import "lueffmgr.h"
#import "luskin.h"


extern ogl2d::System *g_pSys;
extern ogl2d::TextureResource *g_pTexres;

LuSkin g_luSkin;

const int blockfallspeed = 4;
enum
{
    EVENT_TICK,
    EVENT_BONUS_END,
};
struct Tick
{
    Tick():type(EVENT_TICK){}
    int type;
};
struct BonusEnd
{
    BonusEnd():type(EVENT_BONUS_END){}
    int type;
};
Lumines::~Lumines()
{
    delete [] map;
    size_t i;

    for (i = 0; i < players.size(); i++)
        delete players[i];
    players.clear();
    hand = NULL;

    for (i = 0; i < scanners.size(); i++)
        delete scanners[i];
    scanners.clear();    

    splitters.clear();

    g_luSkin.release();
    
    LuEffectManager::release();
}

void Lumines::init(GameMode _gamemode)
{
    size_t i;
    for (i = 0; i < players.size(); i++)
        delete players[i];
    players.clear();
    hand = NULL;

    for (i = 0; i < scanners.size(); i++)
        delete scanners[i];
    scanners.clear();    

    splitters.clear();

    gamemode = _gamemode;
    switch(gamemode)
    {
    case GM_SINGLE_CHALLENGE:
        //ai_enable = false;
        hand = new Player();
        hand->init(this, 0, width, -3*size, -2*size);
        players.push_back(hand);
        for(i = 0; i < 4; i++)
        {
            block t[4];
            genRandBlockGroup(t);
            hand->reload(width/2-1, t);
        }
        hand->setFallSpeed(80);
        break;
    case GM_MULTI_VS_CPU:
        {
            //ai_enable = true;
            hand = new Player();
            hand->init(this, 0, width, -3*size, -2*size);
            players.push_back(hand);
            Player *ai = new AiPlayer();
            ai->init(this, 0, width, (width+1)*size, -2*size);
            players.push_back(ai);
            LuSplitter splitter;
            splitter.init(this, hand, ai, width, width/2);
            splitters.push_back(splitter);
            for(i = 0; i < 4; i++)
            {
                block t[4];
                genRandBlockGroup(t);
                hand->reload(width/2-1, t);
                ai->reload(width/2-1, t);
            }
            hand->setFallSpeed(80);
            ai->setFallSpeed(80);
            ai->aithink();
        }
        break;
    case GM_MULTI_VS_2CPU:
        {
            //ai_enable = true;
            hand = new Player();
            hand->init(this, 0, width, -3*size, -2*size);
            players.push_back(hand);
            Player *ai = new AiPlayer();
            ai->init(this, 0, width, (width+1)*size, -2*size);
            players.push_back(ai);
            LuSplitter splitter;
            splitter.init(this, hand, ai, width, width/2);
            splitters.push_back(splitter);
            for(i = 0; i < 4; i++)
            {
                block t[4];
                genRandBlockGroup(t);
                hand->reload(width/2-1, t);
                ai->reload(width/2-1, t);
            }
            hand->setFallSpeed(80);
            ai->setFallSpeed(80);
            ai->aithink();
        }
        break;
    case GM_MULTI_VS_2P:
//        ai_enable = false;
    default:
        assert(0);
        break;
    }

    clearAllEvents();

    srand(timeGetTime());
    isGameover = false;
    mapchange = false;
    destory = false;

    m_bSoundEnabled = true;
    if (map)
    {
        delete [] map;
    }
    map = new block[width*height];
    //assert(g_pSys != NULL);
    int size = LUMINES_BLOCK_SIZE + 1;
    Scanner *scanner = new Scanner(Scanner::LeftToRight);
    //Scanner *scanner = new Scanner(Scanner::TopToBottom);
    scanner->init(this, font, width*size, height*size);
    scanner->scan();
    scanners.push_back(scanner);
    destoryed = 0;

    level = 0;
    time = 0;
//    score = 0;
    deleted = 0;
    
    bonus = 1;
    deleted_singleblock = 0;
    lastMapChangeBlockCount = 0;
    send(getReceiverId(), &Tick(), sizeof(Tick), 1000); 
    
    time = 0;
    skinLevel.load("skins.ini");
}

void Lumines::start(GameMode _gamemode)
{
    init(_gamemode);
    //pSound->loadSkin("skin.ini");
    bPause = PR_NO_PAUSE;
    stepLevel();
}
/*
void Lumines::updateScore(int add)
{
    score += add;
    if (score > hi_score)
        hi_score = score;
}
*/
bool Lumines::msgHandle(char key,bool msg)
{
    if (activeMenu.msgHandle(key,msg))
    {
        return true;
    }
    switch(msg)
    {
        
    case KEYDOWN:
        switch(key)
        {
        case 'a':case 'A':
            hand->handleMessage(BK_LEFT, true);
            return true;
        case 's':case 'S':
            hand->handleMessage(BK_DOWN, true);
            return true;
        case 'd':case 'D':
            hand->handleMessage(BK_RIGHT, true);
            return true;
        case 'j':case 'J':// anti-closewise
            hand->handleMessage(BK_O, true);
            return true;
         case 'k':case 'K':// closewise
            hand->handleMessage(BK_A, true);
           return true;
            /*
        case 'w':case 'W':
            scanner.scan();
            return true;
            */
        case 'p':case 'P':
            pauseGame();
            return true;
        case 'o':case 'O':
            hand->aithink();
            return true;
        }
        break;
    case KEYUP:
        switch(key)
        {
        case 'a':case 'A':
            hand->handleMessage(BK_LEFT, false);
            return true;
        case 's':case 'S':
            hand->handleMessage(BK_DOWN, false);
            return true;
        case 'd':case 'D':
            hand->handleMessage(BK_RIGHT, false);
            return true;
        case 'j':case 'J':
            hand->handleMessage(BK_O, false);
            return true;
        case 'k':case 'K':
            hand->handleMessage(BK_A, false);
            return true;
        case 'i':case 'I':
            {
                m_bSoundEnabled = !m_bSoundEnabled;
                Sound *pSound = Sound::getInstance();
                pSound->setEnable(m_bSoundEnabled);
            }
            return true;
        }
        break;
    }
    return false;
}
void Lumines::handleEvent()
{
    char buf[MAX_EVENT_DATA_SIZE];
    const Event *pEvent = NULL;
    while(pEvent = getEvent())
    {
        memcpy(buf, pEvent->getBuffer(), pEvent->getSize());
        switch(buf[0])
        {
        case EVENT_TICK:
            if (!bPause)
            {
                time++;
            }
            send(getReceiverId(), &Tick(), sizeof(Tick), 1000); 
            break;
        case EVENT_BONUS_END:
            if (bonus>1)
            {
                bonus = 1;
            }
            /*else
            {
                assert(0);
            }*/
            break;
        default:
            assert(0);
            break;
        }
    }
}
void Lumines::step()
{
    size_t i;
    activeMenu.step();
    Sound *pSound = Sound::getInstance();
    pSound->step();
    LuEffectManager::step();
    if (bPause)
        return;
    switch(gamemode)
    {
    case GM_SINGLE_CHALLENGE:
            hand->aistep();
        break;
    case GM_MULTI_VS_CPU:
    case GM_MULTI_VS_2CPU:
        {
            for (i = 0; i < players.size(); i++)
                players[i]->aistep();
        }
        break;
    case GM_MULTI_VS_2P:
        break;
    }
    hand->step();
    handleEvent();
    if (destory)
    {
        destory = false;
        destoryAllVisited();
        // think(*aiHand);
        switch(gamemode)
        {
        case GM_SINGLE_CHALLENGE:
            break;
        case GM_MULTI_VS_CPU:
        case GM_MULTI_VS_2P:
            {
                for (i = 0; i < players.size(); i++)
                {
                    if (players[i] != hand)
                        players[i]->aithink();
                }
            }
            break;
        }
    }
    
    int j, t, tj;

    if (gamemode != GM_SINGLE_CHALLENGE)
    {
        for (i = 0; i < players.size(); i++)
        {
            if (players[i] != hand)
                players[i]->step();
        }
    }
    for (i = 0; i < scanners.size(); i++)
    {
        scanners[i]->step();
    }
    switch(gamemode)
    {
    case GM_SINGLE_CHALLENGE:
        break;
    case GM_MULTI_VS_CPU:
        {
            for (i = 0; i < splitters.size(); i++)
            {
                splitters[i].step();
            }
        }
        break;
    case GM_MULTI_VS_2P:
        break;
    }
    
    g_luSkin.setSkinChangeStep();

    for (i = 0; (int)i < width; i++)
    {
        for (j = height-1; j>=0; j--)
        {
            block &b = getBlock(i,j);
            if (b.step())
            {
                //ti = map[t].getX() / size;
                tj = b.getY() / size;
                if (tj != j)
                {
                    assert(tj == j+1);
                    block &b2 = getBlock(i,tj);
                    assert(b2.type == BT_NONE);
                    b2 = b;
                    b.type = BT_NONE;
                    mapchange = true;
                }
            }
        }
    }
    if (!mapchange)
        return;

    mapchange = false;
    for (i = 0; (int)i < width; i++)
    {
        // check fall
        int h = height - 1;
        for (j = h; j >= 0;)
        {
            if (getBlock(i,j).type != BT_NONE)
            {
                if (j != h)
                    getBlock(i,j).moveTo(i*size, h*size, blockfallspeed);
                h--,j--;
            }
            else
            {
                for (t = j - 1; t >= 0 && getBlock(i,t).type == BT_NONE; t--)
                    ;
                if (t >= 0 && getBlock(i,t).type != BT_NONE)
                {
                    getBlock(i,t).moveTo(i*size, h*size, blockfallspeed);
                    h--;
                }
                j = t - 1;
            }
        }
    }
    for (i = 0; (int)i < width - 1; i++)
    {
        for (j = height - 2; j >= 0; j--)
        {
            if (getBlock(i,j).type == BT_NONE)
                continue;
            if (!getBlock(i,j).leader)
                continue;
            if (getBlock(i,j).type != getBlock(i+1,j).type ||
                getBlock(i,j).type != getBlock(i,j+1).type || 
                getBlock(i,j).type != getBlock(i+1,j+1).type)
            {
                getBlock(i,j).leader = false;
            }
        }
    }
    
    //*
    for (t = width * height - 1; t >= 0; t--)
    {
        // getBlock(i,j).leader = false;
        map[t].united = false;
        map[t].flood = false;
    }//*/
    bool a = false;
    int count = 0;
    for (i = 0; (int)i < width - 1; i++)
    {
        for (j = height - 2; j >= 0; j--)
        {
            if (getBlock(i,j).type == BT_NONE)
                continue;
            
            if (getBlock(i,j).type == getBlock(i+1,j).type &&
                getBlock(i,j).type == getBlock(i,j+1).type && 
                getBlock(i,j).type == getBlock(i+1,j+1).type)
            {
                if (!getBlock(i,j).leader)
                {
                    getBlock(i,j).leader = true;
                    char str[8];
                    if (lastMapChangeBlockCount)
                    {
                        count = lastMapChangeBlockCount + 1;
                        lastMapChangeBlockCount = count;
                    }
                    else
                        count++;
                    a = true;
                    //int count = floodFindLeader(i,j);
                    sprintf(str, "%d", count);
                    LuEffectManager::pushComboText(i * size+5, j * size+5, str, 0);
                    LuEffectManager::pushUnionEffect(i * size + size-1, j * size + size, 0);
                    LuEffectManager::pushShine(i*size+size, j*size+size);
                    pSound->playeffect(LSE_UNION);
                }
                getBlock(i,j).united = true;
                getBlock(i+1,j).united = true;
                getBlock(i+1,j+1).united = true;
                getBlock(i,j+1).united = true;
                
            }
        }
    }
    lastMapChangeBlockCount = count;
    //if (a)score++;
    
    for (i = 0; (int)i < width; i++)
    {
        for (j = height - 1; j >= 0 && getBlock(i,j).type != BT_NONE; j--)
        {
            if (getBlock(i,j).special && getBlock(i,j).united)
            {
                floodFind(i,j);
            }
        }
    }
}
struct Point2I
{
    int x,y;
    Point2I():x(0),y(0){}
    Point2I(int _x, int _y):x(_x),y(_y){}
};
int Lumines::floodFindLeader(int i, int j)
{
    ::std::vector< Point2I > stack;
    block *b = NULL;
    Point2I p;
    stack.push_back(Point2I(i,j));
    int count = 0;
    int map[]=
    {
                -1,-2,   0,-2,   1,-2,
        -2,-1,  -1,-1,   0,-1,   1,-1,    2,-1,
        -2,0,   -1,0,            1,0,     2,0,
        -2,1,   -1,1,    0,1,    1,1,     2,1,
                -1,2,    0,2,    1,2,
    };
    j = sizeof(map)/sizeof(map[0]);

    while(!stack.empty())
    {
        p = stack.back();
        stack.pop_back();
        b = &getBlock(p.x,p.y);
        if (b->type == BT_NONE || b->type == BT_BAD || b->floodleader || !b->leader)
            continue;
        b->floodleader = true;
        count++;
        for (i = 0; i < j; i++)
        {
            stack.push_back(Point2I(p.x + map[i*2], p.y + map[i*2+1]));
        }
    }
    for (i=height*width -1 ; i>=0; --i)
        Lumines::map[i].floodleader = false;
    return count;
}

void Lumines::floodFind(int i, int j)
{
    block &b = getBlock(i,j);
    block *t;
    b.flood = true;
    t = &getBlock(i,j+1);
    if (t->type == b.type && !t->flood)
        floodFind(i, j+1);
    
    t = &getBlock(i,j-1);
    if (t->type == b.type && !t->flood)
        floodFind(i, j-1);
    
    t = &getBlock(i+1,j);
    if (t->type == b.type && !t->flood)
        floodFind(i+1, j);
    
    t = &getBlock(i-1,j);
    if (t->type == b.type && !t->flood)
        floodFind(i-1, j);
}

void Lumines::reload(Player &aHand)
{
    block t[4];
    genRandBlockGroup(t);
    aHand.reload(width/2-1, t);
    aHand.aithink();
}
void Lumines::gameover()
{
    LuEffectManager::pushComboText(170,70,"Game Over", 1);
    isGameover = true;
    Sound *pSound = Sound::getInstance();
    pSound->stopbg();
    pause(PR_GAME_OVER, true);
    showMenu();
}
void Lumines::putBlock(int i, int j, const block &ablock)
{
    if (ablock.type == BT_NONE)
        return;

    block &t = getBlock(i,j);
    if (t.type == BT_NONE)
    {
        assert(ablock.type == BT_A || ablock.type == BT_B);
        t = ablock;
        int x, y;
        block &down = getBlock(i,j+1);
        x = i * size;
        if (down.type == BT_A || down.type == BT_B)
        {
            y = down.getY() - size;
            t.setPos(x, y);
            t.moveTo(x, j * size);
        }
        else 
        {
            y = j * size;
            t.setPos(x, y);
        }
        mapchange = true;
    }
    else
    {
        assert(0);
    }
}

void Lumines::pause(DWORD reason, bool enable)
{
    if (enable)
    {
        bPause |= reason;
    }
    else
    {
        bPause &= ~reason;
    }
}

void Lumines::render()
{
	
	LuEffectManager::renderBackground();
	GLenum err = glGetError(); if (err != GL_NO_ERROR) NSLog(@"LuEffectManager::renderBackground(): 0x%04X", err);
    const int basex = 104, basey = 72;
   
    glDisable(GL_LINE_SMOOTH);
    glPushMatrix();
    {
        glBlendFunc(GL_ONE,GL_SRC_COLOR);

        glTranslatef(basex, basey, 0);
        int i,j;
        ogl2d::Rect a;
        a.left = 0;

        const int texblocksize = LUMINES_BLOCK_SIZE + 2;
        const int halfsize = texblocksize/2;
        a.right = a.left + texblocksize;
        a.top = texblocksize*5;
        a.bottom = a.top + texblocksize;
        glColor4f(1,1,1,1);
        const SkinPack *skin_pack = NULL;
        const Texture *block_tex = NULL;
        
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for(int step=0;step<2;step++)
        {
            skin_pack = g_luSkin.getSkin(step == 0);
            if (!skin_pack->valid)
                continue;
			GLenum err = glGetError(); if (err != GL_NO_ERROR) NSLog(@"if (!skin_pack->valid) continue; 0x%04X", err);
            block_tex = skin_pack->block_tex;
            float alpha = g_luSkin.getSkinChangeStep();

            for (j=0;j<height;j++)
            {
                for (i=0;i<width;i++)
                {
                    glPushMatrix();
                    glTranslatef(i*size + halfsize, j*size + halfsize, 0);
					 block_tex->DrawRect(a);
                    glPopMatrix();
                }
            }
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);  
        }
		LuEffectManager::renderSpectrum();

        // pass 0: render normal block
        for (i = width * height - 1 ; i >= 0; i--)
        {
            map[i].render(0);
        }
        for (j = 0; j < height - 1; j++)
        {
            for (i = 0; i < width - 1; i++)
            {
                block *t = NULL;
                t = &getBlock(i,j);
                if (t->leader)
                    t->drawleader();
            }
        }
        // pass 1: render destory and flood
        for (i = width * height - 1 ; i >= 0; i--)
        {
            map[i].render(1);
			GLenum err = glGetError(); if (err != GL_NO_ERROR) NSLog(@"5 0x%04X", err);
        }

        for (i = 0; i < int(players.size()); i++)
            players[i]->render();
		GLenum err = glGetError(); if (err != GL_NO_ERROR) NSLog(@"4 0x%04X", err);

        if (!bPause)
        {
            for (i = 0; i < int(scanners.size()); i++)
            {
                scanners[i]->render();
				GLenum err = glGetError(); if (err != GL_NO_ERROR) NSLog(@"3 0x%04X", err);
            }
        }
        if (gamemode != GM_SINGLE_CHALLENGE)
        {
            for (i = 0; i < int(splitters.size()); i++)
            {
                splitters[i].render();
				GLenum err = glGetError(); if (err != GL_NO_ERROR) NSLog(@"2 0x%04X", err);
            }
        }

        LuEffectManager::render();
		err = glGetError(); if (err != GL_NO_ERROR) NSLog(@"1 0x%04X", err);
    }
	
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
 

    glColor4f(1,1,1,0.4f);

    if (gamemode == GM_SINGLE_CHALLENGE)
    {
    const int text_x = 380;
    const int text_y = 60;
    const int text_n_y = text_y + 15;
    const int text_line_skip = 40;
    glPushMatrix();
        glTranslatef(text_x,text_y,0);    glScalef(22,-18,1);
        font->textOut("LEVEL");
    glPopMatrix();

    glPushMatrix();
        glTranslatef(text_x,text_y+text_line_skip,0);    glScalef(22,-18,1);
        font->textOut("TIME");
    glPopMatrix();

    if (bonus > 1)
    {
        glPushMatrix();
            glTranslatef(text_x,text_y+text_line_skip*2,0);    glScalef(22,-18,1);
            glColor4f(.2f,.8f,.8f,.9f);
            font->textOut("Bonus");
            glColor4f(1,1,1,0.4f);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
            glTranslatef(text_x,text_y+text_line_skip*2,0);    glScalef(22,-18,1);
            font->textOut("SCORE");
        glPopMatrix();
    }

    glPushMatrix();
        glTranslatef(text_x,text_y+text_line_skip*3,0);    glScalef(22,-18,1);
        font->textOut("HI-SCORE");
    glPopMatrix();

    glPushMatrix();
        glTranslatef(text_x,text_y+text_line_skip*4,0);    glScalef(22,-18,1);
        font->textOut("DELETED");
    glPopMatrix();

    char str[32];
    glColor4f(1,1,1,0.3f);
    glLineWidth(0.3f);

    glPushMatrix();
        glTranslatef(text_x,text_n_y,0);       glScalef(22,-16,1);
        sprintf(str, "%d", level);    scorefont->textOut(str);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(text_x,text_n_y+text_line_skip,0);    glScalef(22,-16,1);
        sprintf(str, "%d", time);     scorefont->textOut(str);
    glPopMatrix();

    if (bonus>1)
    {
        glPushMatrix();
            glTranslatef(text_x,text_n_y+text_line_skip*2,0);    glScalef(22,-16,1);
            sprintf(str, "X %d", bonus);
            glColor4f(.2f,.8f,.8f,.9f);
            scorefont->textOut(str);
            glColor4f(1,1,1,0.4f);
        glPopMatrix();
    }
    else
    {
        glPushMatrix();
            glTranslatef(text_x,text_n_y+text_line_skip*2,0);    glScalef(22,-16,1);
            sprintf(str, "%d", hand->getScore());
            scorefont->textOut(str);
        glPopMatrix();
    }

    glPushMatrix();
        glTranslatef(text_x,text_n_y+text_line_skip*3,0);    glScalef(22,-16,1);
        sprintf(str, "%d", hand->getHiscore()); scorefont->textOut(str);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(text_x,text_n_y+text_line_skip*4,0);    glScalef(22,-16,1);
        sprintf(str, "%d", deleted);  scorefont->textOut(str);
    glPopMatrix();
    }
    else
    {
        char str[32];
        glPushMatrix();
            glTranslatef(20, 20, 0);    glScalef(22,-16,1);
            sprintf(str, "%d", hand->getThroughCount());  scorefont->textOut(str);
        glPopMatrix();
	}
    activeMenu.render();
	
}

int Lumines::countBlock(btype type)
{
    int count = 0;
    for (int i=0; i< width*height; i++)
    {
        if (map[i].type == type)
            count++;
    }
    return count;
}
void Lumines::stepLevel()
{
    const SkinData *skindata = skinLevel.step(level);
    if (skindata != NULL)
    {
        g_luSkin.load(skindata->path, skindata->name);
        hand->setFallSpeed(max(10, 80-level*3)); // once every x frames
        for (size_t i = 0; i < scanners.size(); i++)
        {
            scanners[i]->setScanSpeed(skindata->pace_speed);
        }
        Sound *pSound = Sound::getInstance();
        pSound->playbg();
    }
}
void Lumines::checkLevelUpdate(int count)
{
    deleted_singleblock += count;
    if (deleted_singleblock >= 100)
    {
        deleted_singleblock -= 100;
        level++;
        stepLevel();
    }
}

void Lumines::destoryAllVisited()
{
    int count = 0;
    for (int i=0; i< width*height; i++)
    {
        if (map[i].type != BT_NONE && map[i].destory)
        {
            map[i].type = BT_NONE;
            mapchange = true;
            count++;
            if (map[i].leader)
            {
                int x, y;
                x = i % width * size + size;
                y = i / width * size + size;
                LuEffectManager::pushExplodeEffect(x, y, 0);
            }
        }
    }
    if (count)
    {
        Sound *pSound = Sound::getInstance();
        pSound->playeffect(LSE_VANISH);
        
        checkLevelUpdate(count); //
    }
    destoryed = 0;

    int counta = countBlock(BT_A);
    int countb = countBlock(BT_B);
    if (counta==0 || countb==0)
    {
        if (counta==0 && countb==0)
        {
            LuEffectManager::pushComboText(156,50,"All Deleted Bonus 10000 pts!",1);
            hand->updateScore(10000);
        }
        else
        {
            LuEffectManager::pushComboText(156,50,"Unicolor Bonus 1000 pts!",1);
            hand->updateScore(1000);
        }
    }

    if (mapchange)
    {
        LuEffectManager::flashBackground(count);
    }
}
int Lumines::flash()
{
    return 0;
}
int Lumines::scan_x(int i)
{
    int count = 0;
    int leaderCount = 0;
    for (int j=0;j<height;j++)
    {
        if (getBlock(i,j).type == BT_NONE)
            continue;

        if (getBlock(i,j).united || getBlock(i,j).flood)
        {
            getBlock(i,j).destory = true;
            destoryed++;
            count++;
            if (getBlock(i,j).leader)
                leaderCount++;
        }
        else if (getBlock(i,j).leader)
        {
            if (getBlock(i,j).type == getBlock(i+1,j).type &&
                getBlock(i,j).type == getBlock(i,j+1).type &&
                getBlock(i,j).type == getBlock(i+1,j+1).type)
            {
                getBlock(i,j).leader = false;
            }
        }
    }
    if ((count == 0 || i == width-1) && destoryed != 0)
    {
        destory = true;
    }
    return leaderCount;
}

int Lumines::scan_y(int j)
{
    int count = 0;
    int leaderCount = 0;
    for (int i=0;i<width;i++)
    {
       if (getBlock(i,j).type == BT_NONE)
            continue;

        if (getBlock(i,j).united || getBlock(i,j).flood)
        {
            getBlock(i,j).destory = true;
            destoryed++;
            count++;
            if (getBlock(i,j).leader)
                leaderCount++;
        }
        else if (getBlock(i,j).leader)
        {
            if (getBlock(i,j).type == getBlock(i+1,j).type &&
                getBlock(i,j).type == getBlock(i,j+1).type &&
                getBlock(i,j).type == getBlock(i+1,j+1).type)
            {
                getBlock(i,j).leader = false;
            }
        }
    }
    if ((count == 0 || j == height-1) && destoryed != 0)
    {
        destory = true;
    }
    return leaderCount;
}
bool Lumines::canMoveBlockTo(int x, int y)
{
    assert(0 && "not implemented yet");
    return true;
}

void Lumines::reportDeleted(int throughCount)
{
    deleted += throughCount;
    if (throughCount >= 4)
    {
        Sound *pSound = Sound::getInstance();
        bonus = throughCount;
        send(getReceiverId(), &BonusEnd(), sizeof(BonusEnd), 1000); 
        if (throughCount <= 6)
        {
            LuEffectManager::pushComboText(0,0,"Combo", 2);
            if (pSound)
                pSound->playeffect(LSE_COMBO_SMALL);
        }
        else
        {
            LuEffectManager::pushComboText(0,0,"Great Combo!", 2);
            if (pSound)
                pSound->playeffect(LSE_COMBO_BIG);
        }
    }
    switch(gamemode)
    {
    case GM_SINGLE_CHALLENGE:
        break;
    case GM_MULTI_VS_CPU:
        {
            /*
            int left = hand->getThroughCount();
            int right = hand2.getThroughCount();
            if (left < right)
                m_Splitter.moveLeft();
            else if (left > right)
                m_Splitter.moveRight();
            hand->setThroughCount(0);
            hand2.setThroughCount(0);
            */
        }
        break;
    case GM_MULTI_VS_2P:
        break;
    }
    hand->updateScore(bonus*throughCount*40);
}

block& Lumines::getBlock(int i, int j) const
{
    static block badblock(BT_BAD);
    if (i < 0 || i >= width || j < 0 || j >= height)
        return badblock;
    
    return map[i+j*width];
}

void Lumines::genRandBlockGroup(block group[4]) const
{
    for(int i=0;i<4;i++)
    {
        group[i].rand();
    }
    group[0].setPos(0, 0);
    group[1].setPos(size, 0);
    group[2].setPos(size, size);
    group[3].setPos(0, size);
}
