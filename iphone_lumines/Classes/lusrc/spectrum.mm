#import <ogl2d.h>
#import "block.h"
#import "spectrum.h"

Spectrum::Spectrum(unsigned int _size, unsigned int _height):size(_size),height(_height)
{
    data = new unsigned int[size];
    for (unsigned int i = 0; i<size; i++)
    {
        data[i] = rand()%height;
    }
}

Spectrum::~Spectrum()
{
    delete [] data;
}

void Spectrum::step()
{
    for (unsigned int i = 0; i<size; i++)
    {
        data[i] += rand()%3-1;
        if (data[i] > height)
            data[i] = height;
        if (data[i] < 0) 
            data[i] = 0;
    }
}

void Spectrum::render()
{
    float r,g,b;
    unsigned int i, j, x, y;
    const unsigned int h = height*::size/3;
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_SRC_ALPHA);
	x = -1;
    for (i = 0; i<size; i++)
    {
        y = h - 1;
        for (j = height - 1; j > data[i]; j--)
        {
            y -= ::size/3;
            r = (1 - (float)j / ((float)height)) * .3f;
            g = (float)j / ((float)height) * .3f;
            b = 0;//(float)data[i] / (float)height;
            glColor4f(r,g,b,.99f);

			GLfloat pfVertices[] = {x+2,y+2,  x+::size,y+2,   x+::size,y+::size/3,   x+2,y+::size/3};
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2,GL_FLOAT,0,pfVertices);
			
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glDisableClientState(GL_VERTEX_ARRAY);

        }
        x += ::size;
    }

}
