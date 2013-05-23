#pragma warning(disable:4786)

#import <ogl2d.h>

#import "ExplodeEffect.h"
using namespace ogl2d;

const int size = 10;

bool ExplodeEffect::inited = false;
static LocusLinear3f scalelocus;
DWORD ExplodeEffect::curframe = 0;

ExplodeEffect::ExplodeEffect()
{
    if (!inited)
    {
        inited = true;
        bitstream data(1024);
        data.write(LT_LINEAR);
        data.write(20);
        data.write(2);
        data.write((int)0); data.write(Point3f(2,  2,  1));
        data.write((int)20);data.write(Point3f(2.5,2.5,0));
        data.rewind();
        scalelocus.loadData(&data);
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
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(2);

    ::std::vector< node >::iterator n;
    for (n = effects.begin(); n != effects.end();)
    {
        Point3f p;
        if (scalelocus.getInterval(curframe - n->start, p))
        {
            glPushMatrix();
            glTranslatef(n->x, n->y, 0);
            glScalef(p.x, p.y,1);
            glColor4f(n->color[0], n->color[1], n->color[2], p.z);
            /*glBegin(GL_LINE_LOOP);
            glVertex2d(-size, -size);
            glVertex2d(size,  -size);
            glVertex2d(size,  size);
            glVertex2d(-size, size);
            //glEnd();*/
			GLfloat pfVertices[] ={-size,-size,   size,-size,   size,size,  -size,size }; 
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2,GL_FLOAT,0,pfVertices);

			glDrawArrays(GL_LINE_LOOP, 0, 4);
			glDisableClientState(GL_VERTEX_ARRAY);


            glPopMatrix();
            ++n;
        }
        else
        {
            n = effects.erase(n);
        }
    }
    glLineWidth(1);
    glDisable(GL_BLEND);
}
