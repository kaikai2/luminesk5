#pragma warning(disable:4786)

#import <ogl2d.h>

#import "unioneffect.h"
using namespace ogl2d;

const int size = 10;

bool UnionEffect::inited = false;
static LocusLinear3f scalelocus;
DWORD UnionEffect::curframe = 0;

UnionEffect::UnionEffect()
{
    if (!inited)
    {
        inited = true;
        bitstream data(256);
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

UnionEffect::~UnionEffect()
{
}

void UnionEffect::push(int x, int y, int type)
{
    node n;
    n.x = x;
    n.y = y;
    n.start = curframe;
    n.type = type;
    n.color[0] = 1;
    n.color[1] = 1;
    n.color[2] = 1;
    effects.push_back(n);
}

void UnionEffect::step()
{
    curframe++;
}

void UnionEffect::render()
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
			GLfloat pfVertices[] ={-size, -size,  size,  -size,  size,  size,  -size, size}; 
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2,GL_FLOAT,0,pfVertices);
			glDrawArrays(GL_LINE_LOOP, 0,4);

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
	glDisableClientState(GL_VERTEX_ARRAY);
}
