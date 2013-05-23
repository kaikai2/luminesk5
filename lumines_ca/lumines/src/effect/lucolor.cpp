#include <cstdlib>
#include "lucolor.h"
LuColor::LuColor()
{
    r = g = b = 0;
    dr = 0.01f;
    dg = 0.02f; 
    db = 0.03f;
}
inline float getrand()
{
    return 0.001f * (rand()%10 + 1);
}

void LuColor::flash()
{
    int i = rand();
    r = i&1?1.f:0.f;
    g = i&1?1.f:0.f;
    b = i&1?1.f:0.f;
    dr = -0.002f;
    dg = -0.002f;
    db = -0.002f;
}

void LuColor::step()
{
    r += dr;
    g += dg;
    b += db;
    if (r > 1)
    {
        r = 1;
        dr = -getrand();
    }
    if (g > 1)
    {
        g = 1;
        dg = -getrand();
    }
    if (b > 1)
    {
        b = 1;
        db = -getrand();
    }
    if (r < 0)
    {
        r = 0;
        dr = getrand();
    }
    if (g < 0)
    {
        g = 0;
        dg = getrand();
    }
    if (b < 0)
    {
        b = 0;
        db = getrand();
    }
}
