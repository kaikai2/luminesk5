#import <ogl2d.h>
#import "bg.h"

void Background::flash(int size)
{
    int i = rand()%15 + 1;
    if (size < 15)
    {
        i &= ~(1<<(rand()%4));
    }
    if (size < 10)
    {
        i &= ~(1<<(rand()%4));
    }
    if (size < 5)
    {
        i &= ~(1<<(rand()%4));
    }
    flashAlpha = 0.5f;
    if (i&1)flashAlpha += 0.124f, c[0].flash();
    if (i&2)flashAlpha += 0.124f,c[1].flash();
    if (i&4)flashAlpha += 0.124f,c[2].flash();
    if (i&8)flashAlpha += 0.124f,c[3].flash();
}


void Background::init(int w, int h)
{
    width = w;
    height = h;
    flashAlpha = 1;
}
void Background::step()
{
    c[0].step();
    c[1].step();
    c[2].step();
    c[3].step();
    if (flashAlpha > 0.5f)
    {
        flashAlpha -= 0.01f;
    }
}
void Background::render()
{
    static float s = 0, d = 0;
    s += 0.1234f;
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
	glPushMatrix();
    glTranslatef(width/2,height/2,0);
    
    float alpha = 0.1f;
    int a[][2] =
    {
        GL_ONE,GL_ONE_MINUS_SRC_ALPHA,
        GL_ONE,GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_ALPHA,GL_ONE,
    };
    int n = 3, end = 2;
	glEnableClientState(GL_COLOR_ARRAY)
	;glEnableClientState(GL_VERTEX_ARRAY);
    for (int i=0;i<end;i++)
    {
		
        glBlendFunc(a[i%n][0],a[i%n][1]);
        glRotatef(s,0,0,1);        
		GLfloat pfColors[16] = {c[0].r,c[0].g,c[0].b,alpha,
								c[1].r,c[1].g,c[1].b,alpha,
								c[2].r,c[2].g,c[2].b,alpha,
								c[3].r,c[3].g,c[3].b,alpha};
		
		GLfloat pfVertices[] ={-width,-height,  width,-height,   width,height,   -width,height};
		
		glVertexPointer(2,GL_FLOAT,0,pfVertices);

		glColorPointer(4,GL_FLOAT,0,pfColors);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		
				

		
		GLenum err = glGetError(); if (err != GL_NO_ERROR) NSLog(@"glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 0x%04X", err);
		assert(glGetError()==GL_NO_ERROR);
		
        alpha*=.5f;
    }
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);


    glPopMatrix();

    glBlendFunc(GL_ONE,GL_SRC_ALPHA);
    glColor4f(0,0,0,flashAlpha);
	GLfloat pfVertices[] = {0,0,  width,0,   width,height,   0,height};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,pfVertices);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	glDisableClientState(GL_VERTEX_ARRAY);


}
