#include <windows.h>
#include <cstdio>
#include <algorithm>
#include <cassert>
#include "luminesrobot.h"

static btype s_posbt[16][4]=
{
    BT_A, BT_A, BT_A, BT_A,
    BT_A, BT_A, BT_A, BT_B,
    BT_A, BT_A, BT_B, BT_A,
    BT_A, BT_A, BT_B, BT_B,
    BT_A, BT_B, BT_A, BT_A,
    BT_A, BT_B, BT_A, BT_B,
    BT_A, BT_B, BT_B, BT_A,
    BT_A, BT_B, BT_B, BT_B,
    BT_B, BT_A, BT_A, BT_A,
    BT_B, BT_A, BT_A, BT_B,
    BT_B, BT_A, BT_B, BT_A,
    BT_B, BT_A, BT_B, BT_B,
    BT_B, BT_B, BT_A, BT_A,
    BT_B, BT_B, BT_A, BT_B,
    BT_B, BT_B, BT_B, BT_A,
    BT_B, BT_B, BT_B, BT_B,
};
//LuminesRobot::LuminesRobot(){}
//LuminesRobot::~LuminesRobot(){}
void LuminesRobot::thinkNextBlockI(
    Sets &baseSet,
    ::std::vector< int > &tops,
    ::std::vector< Sets > &sets,
    int level,
    btype *map, int width, int height, const BlockSet coming[3])
{
    int i, r, t;
    int top1, top2, score;
    BlockSet bs;
    top2 = tops[0];
    for (i=0; i < width-1; i++)
    {
        top1 = tops[i];
        top2 = tops[i+1];
        tops[i] = max(0, top1 - 2);
        tops[i+1] = max(0, top2 - 2);
        for (r = 0; r < 4; r++)
        {
            score = 0;
            bs = s_rotmap[coming[level]][r]; // rotate by y times
            if (top1>=0)
            {
                score += getlink(map, width, height, i, top1, s_posbt[bs][2]);
                t = i+top1*width;
                assert(t >=0 && t < width*height);
                map[t] = s_posbt[bs][2];
                if (top1>0)
                {
                    score += getlink(map, width, height, i, top1-1, s_posbt[bs][0]);
                    t = i+(top1-1)*width;
                    assert(t >=0 && t < width*height);
                    map[t] = s_posbt[bs][0];
                }
            }
            if (top2>=0)
            {
                score += getlink(map, width, height, i+1, top2, s_posbt[bs][3]);
                t = i+1+top2*width;
                assert(t >=0 && t < width*height);
                map[t] = s_posbt[bs][3];
                if (top2>0)
                {
                    score += getlink(map, width, height, i+1, top2-1, s_posbt[bs][1]);
                    t = i+1+(top2-1)*width;
                    assert(t >=0 && t < width*height);
                    map[t] = s_posbt[bs][1];
                }
            }
            if (level < MAX_LEVEL - 1)
            {

                thinkNextBlockI(
                    Sets(baseSet, i, r, score, max(0, min(top1,top2)-1)), 
                    tops, 
                    sets, 
                    level+1, map, width, height, coming);
            }
            else
            {
                sets.push_back(
                    Sets(baseSet, i, r, score, max(0, min(top1,top2)-1))
                    );
            }

            if (top1>=0)
            {
                map[i+top1*width] = BT_NONE;
                if (top1>0)
                    map[i+(top1-1)*width] = BT_NONE;
            }
            if (top2>=0)
            {
                map[i+1+top2*width] = BT_NONE;
                if (top2>0)
                    map[i+1+(top2-1)*width] = BT_NONE;
            }
        }
        tops[i] = top1;
        tops[i+1] = top2;
    }
}
void LuminesRobot::thinkNextBlock(btype *map, int width, int height, const BlockSet coming[3])
{
    int i, j;
    ::std::vector< int > tops(width,0);
    ::std::vector< Sets > sets((width-1)*4*(width-1)*4*(width-1)*4);
    sets.clear();
    tops.clear();
    for (i=0;i<width;i++)
    {
        for (j=height-1;j>=0;j--)
        {
            if (map[i+j*width] == BT_NONE)
                break;
        }
        tops.push_back(j);
    }
    Sets base;
    base.level = -1;
    thinkNextBlockI(base, tops, sets, 0, map, width, height, coming);

    assert(sets.size());
    ::std::sort(sets.begin(), sets.end());

    m_res.clear();
    
    LuRobotResultPair res;
    for (i = 0; i <= sets[0].level; i++)
    {
        res.x = sets[0].x[i];
        res.r = sets[0].r[i];
        m_res.push_back(res);
    }

}
void LuminesRobot::think(btype *map, int width, int height, const BlockSet coming[3])
{
    thinkNextBlock(map, width, height, coming);
    return;
    int i, j;
    int r, t;
    int score;
    BlockSet bs;
    int top1, top2;
    LuRobotResultPair res;
    ::std::vector< int > tops(width,0);
    ::std::vector< Set > sets((width-1)*4);
    sets.clear();
    tops.clear();

#ifdef _DEBUG
    OutputDebugString("\n");
#endif
    for (i=0;i<width;i++)
    {
        for (j=height-1;j>=0;j--)
        {
            if (map[i+j*width] == BT_NONE)
                break;
#ifdef _DEBUG
            if (map[i+j*width] == BT_A)
                OutputDebugString("a");
            else 
                OutputDebugString("b");
        }
        char n[3];
        sprintf(n,"%d\n",j);
        OutputDebugString(n);
#else
        }
#endif

        tops.push_back(j);
    }
    top2 = tops[0];
    for (i=0; i < width-1; i++)
    {
        top1 = top2;
        top2 = tops[i+1];
        for (r = 0; r < 4; r++)
        {
            score = 0;
            bs = s_rotmap[coming[0]][r]; // rotate by y times
            if (top1>=0)
            {
                score += getlink(map, width, height, i, top1, s_posbt[bs][2]);
                t = i+top1*width;
                assert(t >=0 && t < width*height);
                map[t] = s_posbt[bs][2];
                if (top1>0)
                {
                    score += getlink(map, width, height, i, top1-1, s_posbt[bs][0]);
                    t = i+(top1-1)*width;
                    assert(t >=0 && t < width*height);
                    map[t] = s_posbt[bs][0];
                }
            }
            if (top2>=0)
            {
                score += getlink(map, width, height, i+1, top2, s_posbt[bs][3]);
                t = i+1+top2*width;
                assert(t >=0 && t < width*height);
                map[t] = s_posbt[bs][3];
                if (top2>0)
                {
                    score += getlink(map, width, height, i+1, top2-1, s_posbt[bs][1]);
                    t = i+1+(top2-1)*width;
                    assert(t >=0 && t < width*height);
                    map[t] = s_posbt[bs][1];
                }
            }
            sets.push_back(Set(i, r, score, max(0, min(top1,top2)-1)));
               
            if (top1>=0)
            {
                map[i+top1*width] = BT_NONE;
                if (top1>0)
                    map[i+(top1-1)*width] = BT_NONE;
            }
            if (top2>=0)
            {
                map[i+1+top2*width] = BT_NONE;
                if (top2>0)
                    map[i+1+(top2-1)*width] = BT_NONE;
            }
        }
    }
    assert(sets.size());
    ::std::sort(sets.begin(), sets.end());

    m_res.clear();
    
    res.x = sets[0].x;
    res.r = sets[0].r;

    m_res.push_back(res);

#ifdef _DEBUG
    char s[20];
    sprintf(s, "[%x %x %d %d]\n", 
        coming[0], 
        s_rotmap[coming[0]][res.r], 
        res.x, 
        res.r);
    OutputDebugString(s);
#endif
}
inline int btcmp(btype a, btype b)
{
    if (a == b)return 2;
    if (b == BT_NONE)return 1;
    return 0;
}
int LuminesRobot::getlink(const btype *map, int width, int height, int x, int y, btype a)
{
    int score = 0;
    assert(a == BT_A || a == BT_B);
    if (x < 0 || x >= width || y < 0 || y >= height)
        return 0;
    if (x > 0)
    {
         score += btcmp(a, map[x-1+  y*width]);
         if (a == map[x-1+  y*width] && 
             (
             (y<height-1 && a == map[x+(y+1)*width] && a == map[x-1+ (y+1)*width])||
             (y>0 && a == map[x+(y-1)*width] && a == map[x-1+ (y-1)*width])
             ))
             score += 1;
    }
    if (x < width-1)
    {
        score += btcmp(a, map[x+1+  y*width]);
        if (a == map[x+1+  y*width] && 
            (
            (y<height-1 && a == map[x+(y+1)*width] && a == map[x+1+ (y+1)*width])||
            (y>0 && a == map[x+(y-1)*width] && a == map[x+1+ (y-1)*width])
            ))
            score += 1;
    }
    if (y > 0)
    {
        score += btcmp(a, map[x+(y-1)*width]);
        if (a == map[x+  (y-1)*width] && 
            (
            (x<width-1 && a == map[x+1+y*width] && a == map[x+1+ (y-1)*width])||
            (x>0 && a == map[x-1+y*width] && a == map[x-1+ (y-1)*width])
            ))
            score += 1;
    }
    if (y < height-1)
    {
        score += btcmp(a, map[x+(y+1)*width]);
        if (a == map[x+  (y+1)*width] && 
            (
            (x<width-1 && a == map[x+1+y*width] && a == map[x+1+ (y+1)*width])||
            (x>0 && a == map[x-1+y*width] && a == map[x-1+ (y+1)*width])
            ))
            score += 1;
    }
    return score;
}

bool LuminesRobot::getResult(int &x, int &r) // x is pos in map from left, y is 0 to 3 times rotate clock-wise
{
    if (m_res.empty())
        return false;
    x = m_res[0].x;
    r = m_res[0].r;
    m_res.erase(m_res.begin());
    return true;
}
