#import <cstdio>
#import <ogl2d.h>

#import "lumines.h"
#import "scanner.h"
#import "block.h"

void Scanner::init(Lumines *const _lumines, const ogl2d::Font *_font, int _pixwidth, int _pixheight)
{
    lumines = _lumines;
    font = _font;
    width = _pixwidth;
    height = _pixheight;
    scan_pos = 0;
    bScan = false;
    throughCount = 0;
    scan_speed = 1;
    last_pos = -1;
}
void Scanner::step()
{
    if (!bScan)
        return;

    scan_pos += scan_speed;
    if (scan_pos >= getScanLength())
    {
        scan_pos = 0;
        lumines->reportDeleted(throughCount);
        throughCount = 0;
        // bScan = false;
    }
    int pos = int(scan_pos)/(LUMINES_BLOCK_SIZE + 1);
    if (pos != last_pos)
    {
        last_pos = pos;
        assert(lumines);
        if (cDirection == LeftToRight)
            throughCount += lumines->scan_x(pos);
        else // if (cDirection == TopToBottom)
            throughCount += lumines->scan_y(pos);
    }
}
void Scanner::setScanSpeed(float speed)
{
    scan_speed = getScanLength() / (speed*60);
}
void Scanner::scan()
{
    bScan = true;
}
void Scanner::render()
{
		
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    glColor4f(1,.8f,0,1);
    char str[32];
    sprintf(str,"%d",throughCount);
    if (cDirection == LeftToRight)
    {

		
        glTranslatef(float(scan_pos - 8 *strlen(str)), -5, 0);
        glScalef(20,-10,1);
        font->textOut(str);
        glPopMatrix();
        glColor4f(1,.5f,0,.4f);
        glLineWidth(3.f);
		GLfloat pfVertices[] ={scan_pos,0,0.9f,  scan_pos,height+1,0.9f}; 
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,pfVertices);
		glDrawArrays(GL_LINES, 0,2 );

        glColor4f(1,1,.9f,1);
        glLineWidth(.5f);
		GLfloat pfVertices2[] ={scan_pos,0,0.9f,  scan_pos,height+1,0.9f}; 
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,pfVertices2);
		glDrawArrays(GL_LINES, 0,2 );

        const int lightwidth = 50;
        float left = max(-1, scan_pos - lightwidth);
        float right = scan_pos - lightwidth + width;
        if (right >= width) right -= width;
        float scale = 1.f - (scan_pos - left)/(float)lightwidth;

        float color[4]={1,.8f,0,0.3f};
		GLfloat pfColors[]={color[0],color[1],color[2],color[3],
							color[0],color[1],color[2],color[3],
							color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale,
							color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale};
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT,0,pfColors);
		GLfloat pfVertices3[] ={scan_pos, 0,   scan_pos, height+1,   left, height+1,	left, 0}; 
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,0,pfVertices3);
		glDrawArrays(GL_TRIANGLE_FAN, 0,4);

        if (right > left)
        {
			GLfloat pfColors[]={0,0,0,0,	0,0,0,0,
							color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale,
							color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale};
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4,GL_FLOAT,0,pfColors);

			GLfloat pfVertices[] ={right,0,   right, height+1,   width, height+1,	width, 0}; 
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2,GL_FLOAT,0,pfVertices);

			glDrawArrays(GL_TRIANGLE_FAN,0,4);
        }
		
    }
    else
    {
        glTranslatef(-5, float(scan_pos - 8 *strlen(str)), 0);
        glScalef(20,-10,1);
        font->textOut(str);
        glPopMatrix();
        glColor4f(1,.5f,0,.4f);
        glLineWidth(3.f);
		GLfloat pfVertices[] ={0,scan_pos, 0.9f,  width+1,scan_pos, 0.9f}; 
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,pfVertices);
		glDrawArrays(GL_LINES, 0,2 );

        glColor4f(1,1,.9f,1);
        glLineWidth(.5f);
		GLfloat pfVertices2[] ={0,scan_pos, 0.9f,  width+1,scan_pos, 0.9f}; 
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,pfVertices2);
		glDrawArrays(GL_LINES, 0,2 );

        const int lightheight = 50;
        float top = max(-1, scan_pos - lightheight);
        float bottom = scan_pos - lightheight + height;
        if (bottom >= height) bottom -= height;
        float scale = 1.f - (scan_pos - top)/(float)lightheight;

        float color[4]={1,.8f,0,0.3f};
		GLfloat pfColors[]={color[0],color[1],color[2],color[3],
							color[0],color[1],color[2],color[3],
							color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale,
							color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale};
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT,0,pfColors);

		GLfloat pfVertices3[] ={0, scan_pos,   width+1, scan_pos,   width+1,top,	0, top};
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,0,pfVertices3);
		glDrawArrays(GL_TRIANGLE_FAN, 0,4 );
        if (bottom > top)
        {
			GLfloat pfColors[]={0,0,0,0,
								0,0,0,0,
								color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale,
								color[0]*scale,color[1]*scale,color[2]*scale,color[3]*scale};
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4,GL_FLOAT,0,pfColors);

			GLfloat pfVertices3[] ={0, scan_pos,   width+1, scan_pos,   width+1,top,	0, top}; 
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2,GL_FLOAT,0,pfVertices3);

			 glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
    }

    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}
