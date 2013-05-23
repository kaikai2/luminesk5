#include "glCurvedAni.h"
#include "caInterpolaters.h"
#include <gl/glaux.h>

#pragma comment(lib, "glaux")

cAni::iAnimation *anim = 0;
int time;
int gWidth = 0, gHeight = 0;
void CALLBACK RenderFunc()
{
    assert(anim);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBegin(GL_LINE_LOOP);
    glVertex2i(10, 10);
    glVertex2i(gWidth - 10, 10);
    glVertex2i(gWidth - 10, gHeight - 10);
    glVertex2i(10, gHeight - 10);
    glVertex2i(10, 10);
    glEnd();
    glBegin(GL_LINES);
    glVertex2i(10, gHeight/2);
    glVertex2i(gWidth - 10, gHeight/2);
    glVertex2i(gWidth/2, 10);
    glVertex2i(gWidth/2, gHeight - 10);
    glEnd();
    glTranslatef(gWidth/2.f,gHeight/2.f,0);
    //glScaled(1, -1, 1);
    time = int(iSystem::GetInstance()->getTime() * 60);
    anim->render(time, 0);
    auxSwapBuffers();
}

void CALLBACK FrameFunc()
{
    Sleep(1);
    RenderFunc();
}

void Resize(int width, int height, int x/* = 0*/, int y/* = 0*/)
{
    glMatrixMode(GL_PROJECTION);    /* prepare for and then */ 
    glLoadIdentity();               /* define the projection */
    //GLdouble size;
    //GLdouble aspect;
    //size = (GLdouble)(width >= height ? width : height);
    //if (width <= height)
    //{
    //    aspect = (GLdouble)height/(GLdouble)width;
    //}
    //else
    //{
    //    aspect = (GLdouble)width/(GLdouble)height;
    //}
    glOrtho(0, width, height, 0, -100000.0, 100000.0);

    // Make the world and window coordinates coincide so that 1.0 in
    // model space equals one pixel in window space.
    glScaled(1, 1, 1.0);

    //glOrtho(0, w-1, h-1, 0, -100.0f, 100.0);/* transformation */
    glMatrixMode(GL_MODELVIEW);  /* back to modelview matrix */
    glLoadIdentity();
    glViewport(x, y, width, height);      /* define the viewport */
}

void CALLBACK ReshapeFunc(GLsizei w, GLsizei h)
{
    Resize(w, h, 0, 0);
    gWidth = w;
    gHeight = h;
}

int APIENTRY WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{	
    //ÉèÖÃHGEÊôÐÔ
    auxInitDisplayMode(AUX_RGBA | AUX_DOUBLE);
    auxInitPosition(0, 0, 800, 600);
    ReshapeFunc(800, 600);
    auxInitWindow("CurvedAni|gl port demo");
    auxIdleFunc(FrameFunc);
    auxReshapeFunc(ReshapeFunc);
    
    glCurvedAniSystem system;
    cAni::iAnimResManager &armanager = *iSystem::GetInstance()->createAnimResManager();
    anim = iSystem::GetInstance()->createAnimation();
    anim->setAnimData(armanager.getAnimData("explode.xml"), 0); 
    anim->startAnim(int(iSystem::GetInstance()->getTime() * 60), 0);
    //anim->startAnim(0, 0);

    auxMainLoop(RenderFunc);

    iSystem::GetInstance()->release(anim);
    iSystem::GetInstance()->release(&armanager);
    return 0;
}
