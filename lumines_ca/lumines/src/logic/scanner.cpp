#include <cstdio>
#include <glCurvedAni.h>
#include "gl/gl.h"
#include "lumines.h"
#include "scanner.h"
#include "block.h"

void Scanner::init(Lumines *const _lumines, /*const ogl2d::Font *_font, */int _pixwidth, int _pixheight)
{
    lumines = _lumines;
    //font = _font;
    width = _pixwidth;
    height = _pixheight;
    x = 0;
    bScan = false;
    throughCount = 0;
    scan_speed = 1;
    last_pos = -1;
}
void Scanner::step()
{
    if (!bScan)
        return;

    x += scan_speed;
    if (x >= width)
    {
        x = 0;
        lumines->reportDeleted(throughCount);
        throughCount = 0;
        // bScan = false;
    }
    int pos = int(x)/(LUMINES_BLOCK_SIZE + 1);
    if (pos != last_pos)
    {
        last_pos = pos;
        assert(lumines);
        throughCount += lumines->scan(pos);
    }
}
void Scanner::setScanSpeed(float speed)
{
    scan_speed = width / (speed*60);
}
void Scanner::scan()
{
    bScan = true;
}
void Scanner::render()
{
	cAni::iRenderer *pRender = iSystem::GetInstance()->getRenderer("gl");
	glClipState cs, cs2;
    cs.scale = Point2f(20, 10);

    glPushMatrix();
		cs.color = Point3f(1, .8f, 0);
		char str[32];
		sprintf(str,"%d",throughCount);
        cs.position = Point2f(x, 0);
		//glTranslatef(float(x - 8 *strlen(str)), -5, 0);
        cs.anchorOffPos = Point2f(0.5 * strlen(str), 0.5f);
		cs.text = str;
		pRender->render(cs, cs2);
    glPopMatrix();

    glColor4f(1,.5f,0,.4f);
    glLineWidth(3.f);
    glBegin(GL_LINES);
		glVertex3d(x,0,0.9f);
		glVertex3d(x,height+1,0.9f);
    glEnd();

    glColor4f(1,1,.9f,1);
    glLineWidth(.5f);
    glBegin(GL_LINES);
		glVertex3d(x,0,0.9f);
		glVertex3d(x,height+1,0.9f);
    glEnd();

    const int lightwidth = 50;
    float left = max(-1, x - lightwidth);
    float right = x - lightwidth + width;
    if (right >= width)right -= width;
    float scale = 1.f - (x - left)/(float)lightwidth;

    float color[4]={1,.8f,0,0.3f};
    glBegin(GL_QUADS);
		glColor4f(color[0],color[1],color[2],color[3]);
		glVertex2d(x, 0);          glVertex2d(x, height+1);
		glColor4f(color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale);
		glVertex2d(left, height+1);   glVertex2d(left, 0);
		if (right > left)
		{
			glColor4d(0,0,0,0);
			glVertex2d(right, 0);      glVertex2d(right, height+1);
			glColor4f(color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale);
			glVertex2d(width, height+1);  glVertex2d(width, 0);
		}
    glEnd();
}
