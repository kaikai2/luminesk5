#import <ogl2d.h>
#import "shine.h"

void Shine::push(int x, int y)
{
    node anode;
    anode.x = x;
    anode.y = y;
    anode.a1 = float(rand()%360);
    anode.a2 = float(rand() % 40 + 20);
    anode.len1 = float(rand() % 30+20);
    anode.len2 = float(rand() % 10 + 5);
    nodes.push_back(anode);
    anode.a1 += 120 + rand() % 51 - 25;
    anode.a2 += rand()%11-5;
    nodes.push_back(anode);
    anode.a1 += 120 + rand() % 51 - 25;
    anode.a2 += rand()%11-5;
    nodes.push_back(anode);
}

void Shine::step()
{
    ::std::vector< node >::iterator anode;
    for (anode = nodes.begin(); anode != nodes.end(); )
    {
        anode->len1 += 10;
        //anode->a2 -= 3;
        anode->len2 -= 2;
        if (anode->len2 < 2)//anode->a2 < 2.f)
        {
            anode = nodes.erase(anode);
        }
        else
        {
            ++anode;
        }
    }
}

void Shine::render()
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_COLOR);
    glColor4f(1,1,0,1);
    ::std::vector< node >::iterator anode;
    for (anode = nodes.begin(); anode != nodes.end(); ++anode)
    {
        glPushMatrix();
        glTranslatef(float(anode->x), float(anode->y), 0.f);
        glRotatef(anode->a1-anode->a2/2, 0, 0, 1);
        /*glBegin(GL_LINES);
        glVertex2f(anode->len1, 0);
        glVertex2f(anode->len1+anode->len2, 0);
        //glEnd();*/
		GLfloat pfVertices[] ={anode->len1, 0,   anode->len1+anode->len2, 0 };
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,0,pfVertices);
		glDrawArrays(GL_LINES, 0, 2);

        glRotatef(anode->a2, 0, 0, 1);
        /*glBegin(GL_LINES);
        glVertex2f(anode->len1+anode->len2, 0);
        glVertex2f(anode->len1, 0);
        //glEnd();*/
		GLfloat pfVertices2[] ={anode->len1+anode->len2, 0,   anode->len1, 0 };
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,0,pfVertices);
		glDrawArrays(GL_LINES, 0, 2);
        glPopMatrix();
    }
	glDisableClientState(GL_VERTEX_ARRAY);
}