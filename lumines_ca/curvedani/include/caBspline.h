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

#define SP_COUNT 1023
struct spline
{
    static float s_dd[SP_COUNT + 1][4];
    static const float (&bspline_d(float d))[4];

    spline();
};

#endif __BSPLINE_H_