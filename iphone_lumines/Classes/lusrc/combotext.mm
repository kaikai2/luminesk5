#pragma warning(disable:4786)

#import <ogl2d.h>

#import "combotext.h"

using namespace ogl2d;

bool ComboText::inited = false;

LocusBase *scalelocus[3];

class Dummy
{
public:
    Dummy()
    {
        bitstream data(1024);
        
        data.write(LT_LINEAR);
        data.write(90);
        data.write(5);
        data.write((int)0); data.write(Point3f(40,-40,0));
        data.write((int)30);data.write(Point3f(30,-20,1));
        data.write((int)50);data.write(Point3f(30,-20,1));
        data.write((int)70);data.write(Point3f(100,-10,.3f));
        data.write((int)90);data.write(Point3f(200,-1,0));
        data.rewind();
        scalelocus[0] = new LocusLinear3f;
        scalelocus[0]->loadData(&data);
        
        data.rewind();
        data.write(LT_LINEAR);
        data.write(90);
        data.write(5);
        data.write((int)0); data.write(Point3f(40,-40,0));
        data.write((int)30);data.write(Point3f(30,-20,1));
        data.write((int)50);data.write(Point3f(30,-20,1));
        data.write((int)70);data.write(Point3f(100,-10,.3f));
        data.write((int)90);data.write(Point3f(200,-1,0));
        data.rewind();
        scalelocus[1] = new LocusLinear3f;
        scalelocus[1]->loadData(&data);
        
        // combo small / combo big arrow flash
        data.rewind();
        data.write(LT_B_SPLINE);
        data.write(40);
        data.write(4);
        data.write((int)0); data.write(Point3f(-50,100,1));
        data.write((int)12);data.write(Point3f(200,100,1));
        data.write((int)28);data.write(Point3f(250,100,1));
        data.write((int)40);data.write(Point3f(300,100,1));
        data.rewind();
        scalelocus[2] = new LocusBspline3f;
        scalelocus[2]->loadData(&data);
        
        
        
    }
    ~Dummy()
    {
        delete scalelocus[0];
        delete scalelocus[1];
        delete scalelocus[2];
    }

};
static Dummy s_summy;
DWORD ComboText::curframe = 0;
const ogl2d::Font *ComboText::font = NULL;

ComboText::ComboText(const ogl2d::Font *_font)
{
    if (!inited)
    {
        assert(_font != NULL);
        inited = true;
        curframe = 0;
        font = const_cast<Font *>(_font);
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
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    ::std::vector< node >::iterator n;
    for (n = texts.begin(); n != texts.end();)
    {
        Point3f p;
        int ani = n->ani;
        if (ani < 0 || ani > 2)
        {
            assert(0);
            ani = 0;
        }
        switch(ani)
        {
        case 0:case 1:
            if (((LocusLinear3f*)scalelocus[ani])->getInterval(curframe - n->start, p))
            {
                glPushMatrix();
                glTranslatef(n->x, n->y, 0);
                glScalef(p.x, p.y, 1);
                glTranslatef(-0.25 * n->s.size(), -0.5f, 0);
                glColor4f(n->color[0], n->color[1], n->color[2], p.z);
                font->textOut(n->s.c_str());
                glPopMatrix();
                ++n;
            }
            else
            {
                n = texts.erase(n);
            }
            break;
            case 2:
            if (((LocusBspline3f*)scalelocus[ani])->getInterval(curframe - n->start, p))
            {
                glPushMatrix();
                glTranslatef(p.x, p.y, 0);
            
                
                glScalef(20,-20,1);
                glColor4f(n->color[0], n->color[1], n->color[2], p.z);
                font->textOut(n->s.c_str());
                glPopMatrix();
                ++n;
            }
            else
            {
                n = texts.erase(n);
            }
            break;
        }
    }
    glDisable(GL_BLEND);
}
