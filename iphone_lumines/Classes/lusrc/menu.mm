#import <ogl2d.h>
#import "menu.h"
#import "sound.h"


extern ogl2d::System *g_pSys;
extern ogl2d::Font *g_pfont;
extern ogl2d::Font *g_pscorefont;

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

bool Menu::msgHandle(char key,bool msg)
{
    if (!bHandle) return false;
    switch(msg)
    {
    case KEYDOWN:
        switch(key)
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
    case KEYUP:
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
 //   glDisable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_ALPHA);
    glColor4f(0,0,0,0.6f);
	GLfloat pfVertices[] ={0,0,   500,0,   500,300,  0,300}; 
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,pfVertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
//    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    for (int i=0;list[i].id != 0; i++)
    {
        glPushMatrix();
        glTranslatef((float)x, (float)y+i*20, -.5);
        glScalef(20,-20,0);
        if (i == select)
        {
            // draw
            glColor4f(1,0,0,1);
        }
        else
        {
            // draw normal
            glColor4f(1,1,1,1);
        }
        g_pfont->textOut(list[i].name);
        
        glPopMatrix();
    }
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
