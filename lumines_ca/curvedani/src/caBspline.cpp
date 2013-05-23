#include "caBspline.h"

float spline::s_dd[SP_COUNT + 1][4];
static spline spl;
float (*g_dd)[4];

spline::spline()
{
    const float one_sixth = 1.0f/6.0f;
    float d,d2,d3,t1,t2,t3;
    int i = 0;
    for (d=0.0f; i <= SP_COUNT; d += 1.0f / SP_COUNT)
    {
        d2 = d * d;
        d3 = d2 * d;
        t1 = -d3 + 3*d2 - 3*d + 1.0f;
        t2 = 3*d3 - 6*d2 + 4.0f;
        t3 = -3*d3 +3*d2 + 3*d +1.0f;
        s_dd[i][0] = t1 * one_sixth;
        s_dd[i][1] = t2 * one_sixth;
        s_dd[i][2] = t3 * one_sixth;
        s_dd[i][3] = d3 * one_sixth;
        ++i;
    }
}

const float (&spline::bspline_d(float d))[4]
{
    /*
    float d2 = d * d;
    float d3 = d2 * d;
    d *= 3.0f;
    d2 *= 3.0f;
    float t1 = d2 - d + 1.0f - d3;
    float t3 = 3.0f * d3 - d2;
    float t2 = t3 - d2 + 4.0f;
    t3 = d + 1.0f - t3; 
    g_dd[0] = t1*one_sixth;
    g_dd[1] = t2*one_sixth;
    g_dd[2] = t3*one_sixth;
    g_dd[3] = d3*one_sixth;
    /*/
    int i = (int)(d * SP_COUNT);
    if (i > SP_COUNT) i = SP_COUNT;
    else if (i < 0) i = 0;
    return s_dd[i];
    //*/
}