#include <glCurvedAni.h>
#include "gl/gl.h"
#include "menu.h"
#include "../effect/sound.h"
/*
extern ogl2d::Font *g_pfont;
extern ogl2d::Font *g_pscorefont;
*/
Menu::Menu(const MenuItem *_list, MenuCallBack fn):list(_list)
{
    callbackfn = fn;
    select = 0;
    x = 150;
    y = 120;
    bHandle = true;
    visible = true;
}

//Menu::~Menu(){}

bool Menu::msgHandle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (!bHandle)
        return false;
    switch(msg)
    {
    case WM_KEYDOWN:
        switch(wParam)
        {
        case 'w':case 'W':
            selectPrep();
            return true;
        case 's':case 'S':
            selectNext();
            return true;
        case 'a':case 'A':
            turnLeft();
            return true;
        case 'd':case 'D':
            turnRight();
            return true;
        case 'o':case 'O':
        case 'j':case 'J':
            run();
            return true;
        }
        break;
    case WM_KEYUP:
        break;
    }
    return false;
}

void Menu::step()
{
}

void Menu::render()
{
    if (!visible)
		return;
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_ALPHA);
    glColor4f(0,0,0,0.6f);
    glBegin(GL_QUADS);
    glVertex2d(0,0);
    glVertex2d(500,0);
    glVertex2d(500,300);
    glVertex2d(0,300);
    glEnd();
    glPopMatrix();

    glPushMatrix();
	cAni::iRenderer *pRender = iSystem::GetInstance()->getRenderer("gl");

	glClipState cs, cs2;
    cs.scale = Point2f(20, 20);
    glTranslatef(0, 0, -.5);
    for (int i=0;list[i].id != 0; i++)
    {
        cs.position = Point2f(x, (float)y+i*20);
        	
		cs.text = list[i].name;
        if (i == select)
        {
            // draw
            cs.color = Point3f(1,0,0);
        }
        else
        {
            // draw normal
			cs.color = Point3f(1,1,1);
        }
		pRender->render(cs, cs2);
    }
    glPopMatrix();
}
void Menu::selectNext()
{
    if (list[select + 1].id == 0)
        return;
    Sound *pSound = Sound::getInstance();
    if (pSound)
        pSound->playeffect(LSE_LEFT);
    select = select + 1;
}
void Menu::selectPrep()
{
    if (select > 0)
    {
        select = select - 1;
    Sound *pSound = Sound::getInstance();
    if (pSound)
        pSound->playeffect(LSE_LEFT);
    }
}
void Menu::turnLeft()
{

}
void Menu::turnRight()
{
}
void Menu::run()
{
    if (callbackfn != NULL)
    {
        Sound *pSound = Sound::getInstance();
        if (pSound)
            pSound->playeffect(LSE_VANISH);
        callbackfn(list[select].id);
    }
}
