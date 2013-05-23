#ifndef __BSPLINE_H_
#define __BSPLINE_H_

/// b-spline
/*
                {  -d^3 + 3d^2 - 3d + 1  }
{x1,x2,x3,x4} . {  3d^3 - 6d^2 + 0d + 4  }
{y1,y2,y3,y4}   { -3d^3 + 3d^2 + 3d + 1  }
                {   d^3 + 0d^2 + 0d + 0  }

  x = {x1(-d^3+3d^2-3d+1)+x2(d^3-6d^2+4)+x3(-d^3+3d^2+3d+1)+x4(d^3) } / 6
  y = {y1(-d^3+3d^2-3d+1)+y2(d^3-6d^2+4)+y3(-d^3+3d^2+3d+1)+y4(d^3) } / 6
*/
struct sbspline
{
    float x1, y1;
    float x2, y2;
    float x3, y3;
    float x4, y4;
    float x,  y;
};
extern sbspline sBspLine;
void bspline(float d);

void makeBS(int sn, float xs[], float ys[], float ts[], int t, float &ox, float &oy);

void DrawBS(int sn, float xs[], float ys[]);

/// bspline_d
/// @note 计算特征值d的b样条系数，保存在g_dd数组中
/// @param float d 特征值，取值范围[0..1]
extern void bspline_d(float d);
extern float g_dd[4];

#endif __BSPLINE_H_