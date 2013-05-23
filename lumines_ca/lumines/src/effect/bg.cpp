#include <glCurvedAni.h>
#include "windows.h"
#include "gl/gl.h"
#include "bg.h"

void Background::flash(int size)
{
    int i = rand()%15 + 1;
    if (size < 15)
    {
        i &= ~(1<<(rand()%4));
    }
    if (size < 10)
    {
        i &= ~(1<<(rand()%4));
    }
    if (size < 5)
    {
        i &= ~(1<<(rand()%4));
    }
    flashAlpha = 0.5f;
    if (i&1)flashAlpha += 0.124f, c[0].flash();
    if (i&2)flashAlpha += 0.124f,c[1].flash();
    if (i&4)flashAlpha += 0.124f,c[2].flash();
    if (i&8)flashAlpha += 0.124f,c[3].flash();
}


void Background::init(int w, int h)
{
    width = w;
    height = h;
    flashAlpha = 1;
}
void Background::step()
{
    c[0].step();
    c[1].step();
    c[2].step();
    c[3].step();
    if (flashAlpha > 0.5f)
    {
        flashAlpha -= 0.01f;
    }
}
void Background::render()
{
    static float s = 0, d = 0;
    s += 0.1234f;
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    //  glBlendFunc(GL_ONE,GL_SRC_COLOR);
    /*
    glMatrixMode(GL_COLOR);
    GLfloat colorMat[16]=
    {
        0.3f,0.3f,0.3f,0.0f,
        0.5f,0.5f,0.5f,0.0f,
        0.2f,0.2f,0.2f,0.0f,
        0.0f,0.0f,0.0f,3.3f,
    };
    glLoadMatrixf(colorMat);
    glMatrixMode(GL_MODELVIEW);
    //*/
    glPushMatrix();
    glTranslated(width/2,height/2,0);
    
    float alpha = 0.1f;
    //glEnable(GL_POLYGON_SMOOTH);
    int a[][2] =
    {
        GL_ONE,GL_ONE_MINUS_SRC_ALPHA,
        GL_ONE,GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_ALPHA,GL_ONE,
    };
    int n = 3, end = 2;
//  glBlendFunc();
    for (int i=0;i<end;i++)
    {
        glBlendFunc(a[i%n][0],a[i%n][1]);
        glRotatef(s,0,0,1);        
        glBegin(GL_QUADS);
        glColor4f(c[0].r,c[0].g,c[0].b,alpha);
        glVertex2d(-width,-height);
        glColor4f(c[1].r,c[1].g,c[1].b,alpha);
        glVertex2d(width,-height);
        glColor4f(c[2].r,c[2].g,c[2].b,alpha);
        glVertex2d(width,height);
        glColor4f(c[3].r,c[3].g,c[3].b,alpha);
        glVertex2d(-width,height);
        glEnd();
        alpha*=.5f;
    }
    glDisable(GL_POLYGON_SMOOTH);

    glPopMatrix();
    glBlendFunc(GL_ONE,GL_SRC_ALPHA);
    glColor4f(0,0,0,flashAlpha);
    glBegin(GL_QUADS);
    glVertex2d(0,0);
    glVertex2d(width,0);
    glVertex2d(width,height);
    glVertex2d(0,height);
    glEnd();
}
