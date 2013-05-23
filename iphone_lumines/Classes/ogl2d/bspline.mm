#pragma warning(disable: 4786)
// zhu #import <windows.h>
#import <OpenGLES/ES1/gl.h>
#import <assert.h>
#import "bspline.h"


/// b-spline
/*
                {  -d^3 + 3d^2 - 3d + 1  }
{x1,x2,x3,x4} . {  3d^3 - 6d^2 + 0d + 4  }
{y1,y2,y3,y4}   { -3d^3 + 3d^2 + 3d + 1  }
                {   d^3 + 0d^2 + 0d + 0  }

  x = {x1(-d^3+3d^2-3d+1)+x2(d^3-6d^2+4)+x3(-d^3+3d^2+3d+1)+x4(d^3) } / 6
  y = {y1(-d^3+3d^2-3d+1)+y2(d^3-6d^2+4)+y3(-d^3+3d^2+3d+1)+y4(d^3) } / 6
*/
sbspline sBspLine;
float g_dd[4];
#define SP_COUNT 1023

float s_dd[SP_COUNT + 1][4];
const float one_sixth = 1.0f/6.0f;

void initbspline()
{
    float d,d2,d3,t1,t2,t3;
    int i = 0;
    for (d=0.0f; i<=SP_COUNT; d+=1.0f / SP_COUNT)
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

struct splineInitializer
{
    splineInitializer()
    {
        initbspline();
    }
};
static splineInitializer spl;


void bspline(float d)
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
    sBspLine.x = (sBspLine.x1*t1 + sBspLine.x2*t2 + sBspLine.x3*t3 + sBspLine.x4*d3)*one_sixth;
    sBspLine.y = (sBspLine.y1*t1 + sBspLine.y2*t2 + sBspLine.y3*t3 + sBspLine.y4*d3)*one_sixth;
    /*/
    int i = (int)(d * SP_COUNT);
    if (i > SP_COUNT) i = SP_COUNT;
    if (i < 0) i = 0;
    float t1 = s_dd[i][0];
    float t2 = s_dd[i][1];
    float t3 = s_dd[i][2];
    float d3 = s_dd[i][3];
    sBspLine.x = sBspLine.x1*t1 + sBspLine.x2*t2 + sBspLine.x3*t3 + sBspLine.x4*d3;
    sBspLine.y = sBspLine.y1*t1 + sBspLine.y2*t2 + sBspLine.y3*t3 + sBspLine.y4*d3;
    //*/
}

void bspline_d(float d)
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
    if (i < 0) i = 0;
    g_dd[0] = s_dd[i][0];
    g_dd[1] = s_dd[i][1];
    g_dd[2] = s_dd[i][2];
    g_dd[3] = s_dd[i][3];
    //*/

}

void makeBS(int sn, float xs[], float ys[], float ts[], int t, float &ox, float &oy)
{
    float dd;
    int j;
    
    // j = (t/200) % (sn - 4 + 1);
    while (1)
    {
        for (j=0;j<sn-4+1;j++)
        {
            if (t<ts[j])
                break;
        }
        if (j < sn-4+1)
            break;
        t -= (int)ts[sn-1];
    } 
    dd = (t-ts[j-1]) / (ts[j] - ts[j-1]);

    sBspLine.x1 = xs[j];
    sBspLine.y1 = ys[j];
    sBspLine.x2 = xs[j+1];
    sBspLine.y2 = ys[j+1];
    sBspLine.x3 = xs[j+2];
    sBspLine.y3 = ys[j+2];
    sBspLine.x4 = xs[j+3];
    sBspLine.y4 = ys[j+3];
    bspline(dd);
    ox = sBspLine.x;
    oy = sBspLine.y;
}

void DrawBS(int sn, float xs[], float ys[])
{
    float n,x,y,dd;
    int j,i;
    
    glDisable(GL_TEXTURE_2D);
//    glColor3f(0,1,0);
    /*glBegin(GL_LINE_STRIP);
    for (j=0;j<sn;j++)
        glVertex2f(xs[j],ys[j]);
    //glEnd();*/

  //  glColor3f(1,1,1);

    /*glBegin(GL_LINE_STRIP);
    n=8;
    for (j=0;j < sn - 4 + 1;j++)
    {
        dd=0.f;
        for (i=0;i<n;i++)
        {
            sBspLine.x1 = xs[j];
            sBspLine.y1 = ys[j];
            sBspLine.x2 = xs[j+1];
            sBspLine.y2 = ys[j+1];
            sBspLine.x3 = xs[j+2];
            sBspLine.y3 = ys[j+2];
            sBspLine.x4 = xs[j+3];
            sBspLine.y4 = ys[j+3];
            bspline(dd);
            x = sBspLine.x;
            y = sBspLine.y;

            //glColor3f((j*n+i)/(5*n),(j*n+i)/(5*n),(j*n+i)/(5*n));
            glVertex2f(x, y);
            dd += 1.0f/n;
        }
    }
    bspline(dd);
    x = sBspLine.x;
    y = sBspLine.y;

    //glColor3f((j*n+i)/(5*n),(j*n+i)/(5*n),(j*n+i)/(5*n));
    glVertex2f(x, y);

    //glEnd();*/
    glEnable(GL_TEXTURE_2D);
}