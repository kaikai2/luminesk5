#include <cstdio>
#include <windows.h>
#include <glCurvedAni.h>
#include "gl/gl.h"
#include "../effect/sound.h"
#include "lumines.h"
#include "../ui/menu.h"
#include "../ui/menuitem.h"
#include "../effect/lueffmgr.h"
#include "../skin/luskin.h"

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
    g_luSkin.release();
    
    LuEffectManager::release();
}

void Lumines::init(GameMode _gamemode)
{
    int i;
    gamemode = _gamemode;
    switch(gamemode)
    {
    case GM_SINGLE_CHALLENGE:
        //ai_enable = false;
        hand.init(this, 0, width, -3*size, -2*size);
        for(i = 0; i < 4; i++)
        {
            block t[4];
            genRandBlockGroup(t);
            hand.reload(width/2-1, t);
        }
        hand.setFallSpeed(80);
        break;
    case GM_MULTI_VS_CPU:
        //ai_enable = true;
        hand.init(this, 0, width, -3*size, -2*size);
        hand2.init(this, 0, width, (width+1)*size, -2*size);
        m_Splitter.init(this, &hand, &hand2, width, width/2);
        for(i = 0; i < 4; i++)
        {
            block t[4];
            genRandBlockGroup(t);
            hand.reload(width/2-1, t);
            hand2.reload(width/2-1, t);
        }
        hand.setFallSpeed(80);
        hand2.setFallSpeed(80);
        hand2.aithink();
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
    keybuffer.clear();
    //bg.init(480, 272);
    int size = LUMINES_BLOCK_SIZE + 1;
    scanner.init(this, /*font, */width*size, height*size);
    destoryed = 0;
    key.keyLeft = false;
    key.keyRight = false;
    //// keyUp = false;
    key.keyDown = false;
    key.keyRotate = false;
    key.keyRotate2 = false;

    level = 0;
    time = 0;
    score = 0;
    deleted = 0;
    
    bonus = 1;
    deleted_singleblock = 0;
    lastMapChangeBlockCount = 0;
    send(getReceiverId(), &Tick(), sizeof(Tick), 1000); 
    
    time = 0;
    scanner.scan();
    skinLevel.load("skins.ini");
}

void Lumines::start(GameMode _gamemode)
{
    init(_gamemode);
    //pSound->loadSkin("skin.ini");
    bPause = PR_NO_PAUSE;
    stepLevel();
}

void Lumines::updateScore(int add)
{
    score += add;
    if (score > hi_score)
        hi_score = score;
}

bool Lumines::msgHandle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (activeMenu.msgHandle(hWnd, msg, wParam, lParam))
    {
        return true;
    }
    switch(msg)
    {
        
    case WM_KEYDOWN:
        switch(wParam)
        {
        case 'a':case 'A':
            if (!key.keyLeft)
            {
                //keybuffer.push_back(BK_LEFT);
                hand.moveLeft();
                key.keyLeft = true;
            }
            return true;
        case 's':case 'S':
            if (!key.keyDown)
            {
                //keybuffer.push_back(BK_DOWN);
                hand.downStart();
                Sound *pSound = Sound::getInstance();
                pSound->playeffect(LSE_DOWN);
                key.keyDown = true;
            }
            return true;
        case 'd':case 'D':
            if (!key.keyRight)
            {
                //keybuffer.push_back(BK_RIGHT);
                hand.moveRight();
                key.keyRight = true;
            }
            return true;
        case 'j':case 'J':// anti-closewise
            if (!key.keyRotate2)
            {
                keybuffer.push_back(BK_X);
                key.keyRotate2 = true;
            }
            return true;
         case 'k':case 'K':// closewise
            if (!key.keyRotate)
            {
                keybuffer.push_back(BK_O);
                key.keyRotate = true;
            }
           return true;
        case 'w':case 'W':
            scanner.scan();
            return true;
        case 'p':case 'P':
            pauseGame();
            return true;
        case 'o':case 'O':
//            ai_enable = !ai_enable;
//            if (ai_enable)
//            {
                hand.aithink();
//            }
            return true;
        }
        break;
    case WM_KEYUP:
        switch(wParam)
        {
        case 'a':case 'A':
            key.keyLeft = false;
            return true;
        case 's':case 'S':
            hand.downFree();
            key.keyDown = false;
            return true;
        case 'd':case 'D':
            key.keyRight = false;
            return true;
        case 'j':case 'J':
            key.keyRotate2 = false;
            return true;
        case 'k':case 'K':
            key.keyRotate = false;
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
    activeMenu.step();
    Sound *pSound = Sound::getInstance();
    pSound->step();
    LuEffectManager::step(pSound->getCurActive());
    if (bPause)
        return;
    switch(gamemode)
    {
    case GM_SINGLE_CHALLENGE:
        hand.aistep();
        break;
    case GM_MULTI_VS_CPU:
        hand2.aistep();
        break;
    case GM_MULTI_VS_2P:
        break;
    }

    if (key.keyLeft && !key.keyRight)
    {
        keybuffer.push_back(BK_LEFT);
    }
    if (key.keyRight && !key.keyLeft)
    {
        keybuffer.push_back(BK_RIGHT);
    }
    if (key.keyDown)
    {
		if (hand.downLast())
			updateScore(1);
    }
    while (!keybuffer.empty())
    {
        btnkey key = keybuffer.front();
        keybuffer.pop_front();
        switch(key)
        {
        case BK_LEFT:
            hand.move(-1,0);
            break;
        case BK_RIGHT:
            hand.move(1,0);
            break;
        case BK_O:
            hand.rotate(true);
            break;
        case BK_X:
            hand.rotate(false);
            break;
        }
    }
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
            hand2.aithink();
            break;
        case GM_MULTI_VS_2P:
            break;
        }
        
    }
    
    int i, j, t, tj;

    hand.step();
    if (gamemode != GM_SINGLE_CHALLENGE)
        hand2.step();
    scanner.step();
    switch(gamemode)
    {
    case GM_SINGLE_CHALLENGE:
        break;
    case GM_MULTI_VS_CPU:
        m_Splitter.step();
        break;
    case GM_MULTI_VS_2P:
        break;
    }
    
    g_luSkin.setSkinChangeStep();

    for (i = 0; i < width; i++)
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
    for (i = 0; i < width; i++)
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
    for (i = 0; i < width - 1; i++)
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
    int curFrameTime = int(iSystem::GetInstance()->getTime() * 60);
    for (i = 0; i < width - 1; i++)
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
                    getBlock(i,j).leaderStartTime = curFrameTime;
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
    if (a)score++;
    
    for (i = 0; i < width; i++)
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

void Lumines::reload(Hand &aHand)
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
    float time = iSystem::GetInstance()->getTime();
    int frameTime = int(time * 60);

    const int basex = 104, basey = 72;
    LuEffectManager::renderBackground();
    glDisable(GL_LINE_SMOOTH);
    glPushMatrix();
        glBlendFunc(GL_ONE,GL_SRC_COLOR);

        glTranslated(basex, basey, 0);
        int i,j;
        cAni::Rect a;
        a.leftTop.x = 0;

        const int texblocksize = LUMINES_BLOCK_SIZE + 2;
        const int halfsize = texblocksize/2;
        a.rightBottom.x = a.leftTop.x + texblocksize;
        a.leftTop.y = texblocksize*5;
        a.rightBottom.y = a.leftTop.y + texblocksize;
        glColor3f(1,1,1);
        const SkinPack *skin_pack = NULL;
        //const Texture *block_tex = NULL;
		iAnimation *anim = iSystem::GetInstance()->createAnimation();
        for(int step=0;step<2;step++)
        {
            skin_pack = g_luSkin.getSkin(step == 0);
            if (!skin_pack->valid)
                continue;
            //block_tex = skin_pack->block_tex;
			anim->setAnimData(skin_pack->anim[SkinPack::AT_SpecialActivated], 0);
            float alpha = g_luSkin.getSkinChangeStep();

            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            for (j=0;j<height;j++)
            {
                for (i=0;i<width;i++)
                {
                    glPushMatrix();
                    glTranslated(i*size + halfsize, j*size + halfsize, 0);
                    //block_tex->DrawRect(a);
					anim->render(frameTime, 0);
                    glPopMatrix();
                }
            }
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);  
        }
		iSystem::GetInstance()->release(anim);
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
        }


        hand.render();
        if (gamemode != GM_SINGLE_CHALLENGE)
            hand2.render();

        if (!bPause)
            scanner.render();
        if (gamemode != GM_SINGLE_CHALLENGE)
        {
            m_Splitter.render();
        }

        LuEffectManager::render();

    glPopMatrix();

	cAni::iRenderer *pRender = iSystem::GetInstance()->getRenderer("gl");
	glClipState cs, cs2;

    glDisable(GL_TEXTURE_2D);    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_ALPHA_SATURATE,GL_ONE);
//    glEnable(GL_POLYGON_SMOOTH);
    //glDisable(GL_DEPTH_TEST);

	cs.color = Point3f(1,1,1);
	cs.alpha = 0.4f;
    cs.scale = Point2f(22, 18);

    char str[32];
	if (gamemode == GM_SINGLE_CHALLENGE)
	{
		const int text_x = 380;
		const int text_y = 60;
		const int text_n_y = text_y + 15;
		const int text_line_skip = 40;

        cs.position = Point2f(text_x,text_y);
		cs.text = "LEVEL";
		pRender->render(cs, cs2);

        cs.position = Point2f(text_x,text_y+text_line_skip);
		cs.text = "TIME";
		pRender->render(cs, cs2);

        cs.position = Point2f(text_x,text_y+text_line_skip*2);
		if (bonus > 1)
		{
			cs.color = Point3f(.2f, .8f, .8f);
			cs.alpha = .9f;
			cs.text = "Bonus";
		}
		else
		{
			cs.text = "SCORE";
		}
		pRender->render(cs, cs2);
		cs.color = Point3f(1.f, 1.f, 1.f);
		cs.alpha = .4f;

        cs.position = Point2f(text_x,text_y+text_line_skip*3);
        cs.text = "HI-SCORE";
        pRender->render(cs, cs2);

        cs.position = Point2f(text_x,text_y+text_line_skip*4);
        cs.text = "DELETED";
        pRender->render(cs, cs2);

		cs.color = Point3f(1.f, 1.f, 1.f);
		cs.alpha = .3f;
		glLineWidth(0.3f);

        cs.scale = Point2f(22, 16);

        cs.position = Point2f(text_x,text_n_y);
        sprintf(str, "%d", level);
        cs.text = str;
        pRender->render(cs, cs2);

        cs.position = Point2f(text_x,text_n_y+text_line_skip);
        sprintf(str, "%d", this->time);
        cs.text = str;
        pRender->render(cs, cs2);

        cs.position = Point2f(text_x,text_n_y+text_line_skip*2);
        if (bonus>1)
        {
            sprintf(str, "X %d", bonus);
            cs.color = Point3f(.2f, .8f, .8f);
            cs.alpha = .9f;
        }
        else
        {
            sprintf(str, "%d", score);
        }
        cs.text = str;
        pRender->render(cs, cs2);
        cs.color = Point3f(1.f, 1.f, 1.f);
        cs.alpha = .4f;

        cs.position = Point2f(text_x,text_n_y+text_line_skip*3);
        sprintf(str, "%d", hi_score); 
        cs.text = str;
        pRender->render(cs, cs2);

        cs.position = Point2f(text_x,text_n_y+text_line_skip*4);
        sprintf(str, "%d", deleted);
        cs.text = str;
        pRender->render(cs, cs2);
		
		//glPushMatrix();
		//	glTranslated(30,200,0);    glScaled(22,-16,1);
		//	sprintf(str, ai_enable?"CPU":"PLAYER");
		//	cs.text = str;
		//	pRender->render(cs, cs2);
		//glPopMatrix();
	}
    else
    {
        cs.scale = Point2f(22, 16);

        cs.position = Point2f(20, 20);
        sprintf(str, "%d", hand.getThroughCount());
        cs.text = str;
        pRender->render(cs, cs2);

        cs.position = Point2f(380, 20);
        sprintf(str, "%d", hand2.getThroughCount());
        cs.text = str;
        pRender->render(cs, cs2);
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
        hand.setFallSpeed(max(10, 80-level*3)); // once every x frames
        scanner.setScanSpeed(skindata->pace_speed);
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
            updateScore(10000);
        }
        else
        {
            LuEffectManager::pushComboText(156,50,"Unicolor Bonus 1000 pts!",1);
            updateScore(1000);
        }
    }

    if (mapchange)
    {
        LuEffectManager::flashBackground(count);
    }
}

int Lumines::scan(int i)
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
            int left = hand.getThroughCount();
            int right = hand2.getThroughCount();
            if (left < right)
                m_Splitter.moveLeft();
            else if (left > right)
                m_Splitter.moveRight();

            hand.setThroughCount(0);
            hand2.setThroughCount(0);
        }
        break;
    case GM_MULTI_VS_2P:
        break;
    }
    updateScore(bonus*throughCount*40);
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
