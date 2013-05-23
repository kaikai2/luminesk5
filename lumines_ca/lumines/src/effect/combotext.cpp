#pragma warning(disable:4786)

#include <glCurvedAni.h>
#include "gl/gl.h"
#include "combotext.h"

using namespace cAni;

bool ComboText::inited = false;
static const cAni::AnimData *g_anim_data[3] = {0};
//bitstream data(1024);

//data.write(LT_LINEAR);
//data.write(90);
//data.write(5);
//data.write((int)0); data.write(Point3f(40,-40,0));
//data.write((int)30);data.write(Point3f(30,-20,1));
//data.write((int)50);data.write(Point3f(30,-20,1));
//data.write((int)70);data.write(Point3f(100,-10,.3f));
//data.write((int)90);data.write(Point3f(200,-1,0));
//data.rewind();
//scalelocus[0].loadData(&data);

//data.rewind();
//data.write(LT_LINEAR);
//data.write(90);
//data.write(5);
//data.write((int)0); data.write(Point3f(40,-40,0));
//data.write((int)30);data.write(Point3f(30,-20,1));
//data.write((int)50);data.write(Point3f(30,-20,1));
//data.write((int)70);data.write(Point3f(100,-10,.3f));
//data.write((int)90);data.write(Point3f(200,-1,0));
//data.rewind();
//scalelocus[1].loadData(&data);

//// combo small / combo big arrow flash
//data.rewind();
//data.write(LT_B_SPLINE);
//data.write(40);
//data.write(4);
//data.write((int)0); data.write(Point3f(-50,100,1));
//data.write((int)12);data.write(Point3f(200,100,1));
//data.write((int)28);data.write(Point3f(250,100,1));
//data.write((int)40);data.write(Point3f(300,100,1));
//data.rewind();
//scalelocus[2].loadData(&data);
DWORD ComboText::curframe = 0;
//const ogl2d::Font *ComboText::font = NULL;

ComboText::ComboText(cAni::iAnimResManager &rARM/*const ogl2d::Font *_font*/)
{
    if (!inited)
    {
        //assert(_font != NULL);
        inited = true;
        g_anim_data[0] = rARM.getAnimData("data/combo0.xml");
        g_anim_data[1] = rARM.getAnimData("data/combo1.xml");
        g_anim_data[2] = rARM.getAnimData("data/combo2.xml");
        curframe = 0;
        //font = const_cast<Font *>(_font);
    }
}

ComboText::~ComboText()
{
}

void ComboText::push(int x, int y, const char* s, int ani)
{
    assert(ani >=0 && ani < 3);
    node n;
    n.x = x;
    n.y = y;
    n.start = curframe;
    n.s = s;
    n.ani = ani;
    n.color[0] = 1;
    n.color[1] = 1;
    n.color[2] = 1;
    texts.push_back(n);
}

void ComboText::step()
{
    curframe++;
}

void ComboText::render()
{
	//cAni::iRenderer *pRender = iSystem::GetInstance()->getRenderer("gl");

	
	iAnimation *anim = iSystem::GetInstance()->createAnimation();
    ::std::vector< node >::iterator n;
    for (n = texts.begin(); n != texts.end();)
    {
        //Point3f p;
        int ani = n->ani;
        if (ani < 0 || ani > 2)
        {
            assert(0);
            ani = 0;
        }
		anim->setAnimData(g_anim_data[ani], 0);
        //if (scalelocus[ani].getInterval(curframe - n->start, p))
		if (!anim->checkEnd(curframe - n->start))
        {
			glClipState cs;
            glPushMatrix();
            switch(ani)
            {
            case 0:case 1:
                glTranslated(n->x, n->y, 0);
                //glScalef(p.x, p.y, 1);
                
                //glColor4f(n->color[0], n->color[1], n->color[2], p.z);
				cs.color = Point3f(n->color[0], n->color[1], n->color[2]);
                cs.anchorOffPos = Point2f(0.24 * n->s.size(), 0.5f);
				//cs.alpha = p.z;
				//pRender->render(cs);
				cs.text = n->s;
				anim->render(curframe - n->start, &cs);
                break;
            case 2:
                //glTranslatef(p.x, p.y, 0);
                /*
                glBegin(GL_POLYGON);
                glVertex2f(-20,5);glVertex2f(20,5);
                glVertex2f(20,10);glVertex2f(30,0);
                glVertex2f(20,-10);glVertex2f(20,-5);
                glVertex2f(-20,-5);glVertex2f(-20,5);
                glEnd();
                */
                //glColor4f(n->color[0], n->color[1], n->color[2], p.z);
				cs.color = Point3f(n->color[0], n->color[1], n->color[2]);
				//cs.alpha = p.z;
				cs.text = n->s;
				anim->render(curframe - n->start, &cs);
                break;
            }
            glPopMatrix();
            ++n;
        }
        else
        {
            n = texts.erase(n);
        }
    }
	iSystem::GetInstance()->release(anim);
    glDisable(GL_BLEND);
}
