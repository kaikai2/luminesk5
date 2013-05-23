#pragma warning(disable:4786)

#include <glCurvedAni.h>
#include "gl/gl.h"
#include "ExplodeEffect.h"
using namespace cAni;

const int size = 10;

bool ExplodeEffect::inited = false;
//static Point3f scalelocus;
static const cAni::AnimData *scale_Locus = 0;
DWORD ExplodeEffect::curframe = 0;

ExplodeEffect::ExplodeEffect(cAni::iAnimResManager &rARM)
{
    if (!inited)
    {
        inited = true;
        scale_Locus = rARM.getAnimData("data/explode.xml");
        //bitstream data(1024);
        //data.write(LT_LINEAR);
        //data.write(20);
        //data.write(2);
        //data.write((int)0); data.write(Point3f(2,  2,  1));
        //data.write((int)20);data.write(Point3f(2.5,2.5,0));
        //data.rewind();
        //scalelocus.loadData(&data);
        curframe = 0;
    }
}

ExplodeEffect::~ExplodeEffect()
{
}

void ExplodeEffect::push(int x, int y, int type)
{
    node n;
    n.x = x;
    n.y = y;
    n.start = curframe;
    n.type = type;
    n.color[0] = 1;
    n.color[1] = 0;
    n.color[2] = 0;
    effects.push_back(n);
}

void ExplodeEffect::step()
{
    curframe++;
}

void ExplodeEffect::render()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    iAnimation *anim = iSystem::GetInstance()->createAnimation();

    ::std::vector< node >::iterator n;
    for (n = effects.begin(); n != effects.end();)
    {
        anim->setAnimData(scale_Locus, 0);
        if (!anim->checkEnd(curframe - n->start))
        {
            glClipState cs;
            cs.color = Point3f(n->color[0], n->color[1], n->color[2]);
            cs.position = Point2f(n->x, n->y);
            anim->render(curframe - n->start, &cs);
            ++n;
        }
        else
        {
            n = effects.erase(n);
        }
    }
}
