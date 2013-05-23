#pragma warning(disable:4786)
#include <glCurvedAni.h>
#include "gl/gl.h"
#include "../effect/sound.h"
#include "lumines.h"
#include "hand.h"

Hand::Hand():x(0),y(0),tick(0),lumines(NULL),left(0),right(0),fallspeed(LUMINES_DEF_HAND_SPEED)
{
}
void Hand::setFallSpeed(DWORD speed)
{
    fallspeed = speed;
}
void Hand::setLimit(int _left, int _right)
{
    left = _left;
    right = _right;
    int tx = x;
    if (tx + 2 > right)
    {
        for(int i = 1; i <= tx + 2 - right; i++)
        {
            if (!moveBlockTo(tx - i, y))
            {
                putBlocks();
                break;
            }
        }
    }
    if (tx < left)
    {
        for(int i = 1; i <= left - tx; i++)
        {
            if (!moveBlockTo(tx + i, y))
            {
                putBlocks();
                break;
            }
        }
    }
}

void Hand::init(Lumines *_lumines, int _left, int _right, int _px, int _py)
{
    lumines = _lumines;
    hx = 0;
    py = 0.f;
    fallspeed = LUMINES_DEF_HAND_SPEED;
    delay = LUMINES_DEF_HAND_SPEED;
    setLimit(_left, _right);
    prepareX = _px;
    prepareY = _py;
    blockset = BS_AAAA;
    memset(newblock, 0, sizeof(block)*4);
    memset(nextblock, 0, sizeof(block)*16);
    throughCount = 0;
}
void Hand::rotate(bool clockwise)
{
    Sound *pSound = Sound::getInstance();
    if (!clockwise)
    {
        block t = newblock[0];
        for (int i=0; i<3; i++)
            newblock[i] = newblock[i+1];
        newblock[3] = t;
        pSound->playeffect(LSE_ROTB);
        blockset = s_rotmap[blockset][3];
        assert(blockset ==
            BlockSet(
        ((newblock[0].type == BT_B)?0x08:0) |
        ((newblock[1].type == BT_B)?0x04:0) |
        ((newblock[3].type == BT_B)?0x02:0) |
        ((newblock[2].type == BT_B)?0x01:0)));
    }
    else
    {
        block t = newblock[3];
        for (int i=3; i>0; i--)
            newblock[i] = newblock[i-1];
        newblock[0] = t;
        pSound->playeffect(LSE_ROTA);
        blockset = s_rotmap[blockset][1];
        assert(blockset ==
            BlockSet(
        ((newblock[0].type == BT_B)?0x08:0) |
        ((newblock[1].type == BT_B)?0x04:0) |
        ((newblock[3].type == BT_B)?0x02:0) |
        ((newblock[2].type == BT_B)?0x01:0)));

    }
    newblock[0].setPos(0, 0);
    newblock[1].setPos(size, 0);
    newblock[2].setPos(size, size);
    newblock[3].setPos(0, size);
}

void Hand::downStart()
{
    move(0, size);
}

bool Hand::downLast()
{
    if (y >= -1)
    {
        move(0, size);
		return true;
    }
	return false;
}

void Hand::putBlocks()
{
    assert(lumines);

    lumines->putBlock(x+1, y+1, newblock[2]);
    lumines->putBlock(x, y+1, newblock[3]);
    if (y >= 0)
    {
        lumines->putBlock(x, y, newblock[0]);
        lumines->putBlock(x+1, y, newblock[1]);
    }
    lumines->reload(*this);
}

bool Hand::moveBlockTo(int _x, int _y)
{
    assert(lumines);

    // must move and only move one step in only one direction
    if (x == _x && _y - y == 1 || y == _y && (_x - x == 1 || _x - x == -1))
    {
    }
    else
    {
        return false;
    }


    if (_x >= left && _x < right - 1 && (_y == -2 || 
        lumines->getBlock(_x+1, _y+1).type == BT_NONE &&
        lumines->getBlock(_x,   _y+1).type == BT_NONE &&
        (_y < 0 ||
        lumines->getBlock(_x+1, _y).type == BT_NONE &&
        lumines->getBlock(_x,   _y).type == BT_NONE)
        )
        )
    {
        x = _x;
        y = _y;
        return true;
    }
    
    if (y != _y) // dropped
    {
        if (y >= -1)
        {
            putBlocks();
        }
        else
        {
            if (x + 2 < right && 
                lumines->getBlock(x+1, 0).type == BT_NONE && 
                lumines->getBlock(x+2, 0).type == BT_NONE)
            {
                x++;
                hx=0;
            }
            else if (
                x - 1 >= left &&
                lumines->getBlock(x-1, 0).type == BT_NONE && 
                lumines->getBlock(x, 0).type == BT_NONE)
            {
                x--;
                hx=0;
            }
            else
            {
                lumines->gameover();
            }
        }
    }
    else
    {
        aithink();
    }
    return false;
}

void Hand::moveLeft()
{
    moveBlockTo(x-1, y);
    hx = 17;
    Sound *pSound = Sound::getInstance();
    pSound->playeffect(LSE_LEFT);
}
void Hand::moveRight()
{
    moveBlockTo(x+1, y);
    hx = -17;
    Sound *pSound = Sound::getInstance();
    pSound->playeffect(LSE_RIGHT);
}

void Hand::move(int dx, int dy)
{
    assert(lumines);
    const int d = 0;
    if (dx)
    {
        hx += dx;
        if (dx > 0 && hx >= d)
        {
            if (hx == d)
            {
                Sound *pSound = Sound::getInstance();
                pSound->playeffect(LSE_RIGHT_SLIDE);
            }
            dx = 1;
            hx = 0;
        }
        else if (dx < 0 && hx <= -d)
        {
            if (hx == -d)
            {
                Sound *pSound = Sound::getInstance();
                pSound->playeffect(LSE_LEFT_SLIDE);
            }
            dx = -1;
            hx = 0;
        }
        else
            return;
    }
    dx += x;
    if (dx < left || dx+1 >= right)
        return;
    py += dy;
    dy = int((py) / size);
    moveBlockTo(dx, dy);
}

void Hand::downFree()
{/*
    int t = (int)py % size;
    if (t)
    {
        move(0, size - t);
    }
    */
}

void Hand::step()
{
    if (++tick >= fallspeed)
    {
        tick = 0;
        int ty = size - (int)py % size;
        move(0, ty);
    }
    y = int(py/size);
    newblock[0].step();
    newblock[1].step();
    newblock[2].step();
    newblock[3].step();
    for (int i = 0; i < 16; i++)
    {
        nextblock[0][i].step();
    }
}

void Hand::reload(int _x, const block blocks[4])
{
    int i, j;
    for (i=0;i<4;i++)
    {
        nextblock[3][i] = newblock[i] = nextblock[0][i];
        nextblock[0][i] = nextblock[1][i];
        nextblock[1][i] = nextblock[2][i];
        nextblock[2][i] = blocks[i];
    }
    for (j=0;j<3;j++)
    {
        nextblock[j][0].setPos(0,       j * 3 * size + size * 3);
        nextblock[j][0].moveTo(0,       j * 3 * size, 2);
        nextblock[j][1].setPos(size,    j * 3 * size + size * 3);
        nextblock[j][1].moveTo(size,    j * 3 * size, 2);
        nextblock[j][3].setPos(0,       j * 3 * size + size * 3 + size);
        nextblock[j][3].moveTo(0,       j * 3 * size + size, 2);
        nextblock[j][2].setPos(size,    j * 3 * size + size * 3 + size);
        nextblock[j][2].moveTo(size,    j * 3 * size + size, 2);
    }
    j = 3;
    nextblock[j][0].setPos(0,       - 3 * size + size * 3);
    nextblock[j][0].moveTo(0,       - 3 * size, 2);
    nextblock[j][1].setPos(size,    - 3 * size + size * 3);
    nextblock[j][1].moveTo(size,    - 3 * size, 2);
    nextblock[j][3].setPos(0,       - 3 * size + size * 3 + size);
    nextblock[j][3].moveTo(0,       - 3 * size + size, 2);
    nextblock[j][2].setPos(size,    - 3 * size + size * 3 + size);
    nextblock[j][2].moveTo(size,    - 3 * size + size, 2);

    x = (left + right)/2 - 1;
    y = -2;
    hx = 0;

    newblock[0].setPos(0, 0);
    newblock[1].setPos(size, 0);
    newblock[2].setPos(size, size);
    newblock[3].setPos(0, size);

    blockset = BlockSet(
        ((newblock[0].type == BT_B)?0x08:0) |
        ((newblock[1].type == BT_B)?0x04:0) |
        ((newblock[3].type == BT_B)?0x02:0) |
        ((newblock[2].type == BT_B)?0x01:0));
    
        // px = x * size;
    py = (float)y * size;
    tick = -delay;
}

void Hand::render()
{
    int i;
    glPushMatrix();
        glTranslated(x * size, py, 0);
        newblock[0].render(0);
        newblock[1].render(0);
        newblock[2].render(0);
        newblock[3].render(0);
    glPopMatrix();
    glPushMatrix();
        glTranslated(prepareX, prepareY,0);
        cAni::Rect clipper;
		clipper.leftTop = cAni::Point2s(0, 0);
        clipper.rightBottom = cAni::Point2s(size*2, size*8);
        for (i = 0; i < 4; i++)
        {
            nextblock[i][0].render(0, &clipper);
            nextblock[i][1].render(0, &clipper);
            nextblock[i][2].render(0, &clipper);
            nextblock[i][3].render(0, &clipper);
        }
    glPopMatrix();
}
////////////////////
// AiHand

void AiHand::init(Lumines * const _lumines, int _left, int _right, int _px, int _py)
{
    Parent::init(_lumines, _left, _right, _px, _py);
    ai_speed = 2;
    ai_maxspeed = 10;
    ai_tick = 0;
}

void AiHand::aithink()
{
    int i, j, x, y;
    int left,right;
    int height = lumines->getHeight();

    if (ai_speed > 30)
    {
        if (rand()%10 == 0)
        {
            ai_speed--;
            ai_tick = 0;
        }
    }
    ai_valid = robot.getResult(ai_x, ai_rotate);
    if (!ai_valid)
    {
        x = getX();
        y = min(getY()+1, /*height*/10 -1);
        if (y < 0)
        {
            left = getLeftLimit();
            right = getRightLimit();
        }
        else
        {
            for (i = x; i < getRightLimit(); i++)
            {
                if (lumines->getBlock(i, y).type != BT_NONE)
                    break;
            }
            right = min(getRightLimit(), i + 1);
            for (i = x; i >= getLeftLimit(); i--)
            {
                if (lumines->getBlock(i, y).type != BT_NONE)
                    break;
            }
            left = max(getLeftLimit(), i + 1);
            if (right <= left)
                return;
        }
        btype *tmap = new btype[(right-left) * height];
        for (i=left;i<right;i++)
        {
            for (j=0;j < height;j++)
            {
                tmap[i-left+j*(right-left)] = lumines->getBlock(i, j).type;
            }
        }
        BlockSet sets[3];
        sets[0] = getBlockSet();
        sets[1] = getBlockSet(nextblock[0]);
        sets[2] = getBlockSet(nextblock[1]);
        robot.think(tmap, right-left, height, sets);
        delete [] tmap;
        ai_valid = robot.getResult(ai_x, ai_rotate);
        ai_x += left;
    }
}
void AiHand::aistep()
{
    if (!ai_valid)
        return;
    ai_tick++;
    if (ai_tick >= ai_speed)
    {
        ai_speed = rand()%ai_maxspeed + 20;
        ai_tick = 0;
        int x = getX();
        if (ai_x < x)moveLeft();
        else if (ai_x > x)moveRight();
        else if (ai_rotate == 0)
        {
            downStart();
        }
        
        if (ai_rotate != 0)
        {
            if (ai_rotate <= 2)
            {
                ai_rotate--;
                rotate(true);
            }
            else
            {
                ai_rotate = (ai_rotate+1)%4;
                rotate(false);
            }
        }
    }
    else
    {
        int x = getX();
        if (ai_x < x)
            move(-1,0);
        else if(ai_x > x)
            move(1,0);
        else 
        {
            if (ai_rotate == 0 && abs(lumines->getScannerX() - x) > 1)
            {
                downLast();
            }
        }
    }
}
