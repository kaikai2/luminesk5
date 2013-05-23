#include <glCurvedAni.h>
#include "gl/gl.h"
#include "../logic/block.h"
#include "spectrum.h"
#include "bass.h"

Spectrum::Spectrum(unsigned int _size, unsigned int _height):size(_size),height(_height)
{
    data = new unsigned int[size * 3];
    for (unsigned int i = 0; i<size; i++)
    {
        data[i] = rand()%height;
    }
}

Spectrum::~Spectrum()
{
    delete [] data;
}

void Spectrum::step(std::vector<DWORD> &rVecCurrentChannel)
{
    //data[i]
    int nCount = 0;
    unsigned int *data3 = data + size * 2;
    for (unsigned int i = 0; i<size; i++)
    {
        data3[i] = 0;
    }
    for (int i = 0; i < rVecCurrentChannel.size(); i++)
    {
        unsigned int *data2 = data + size;
        float buf[256];
        if (BASS_ChannelGetData(
            rVecCurrentChannel[i],
            buf,
            BASS_DATA_FFT512
            ) == sizeof(buf))
        {
            float maxf = 0;
            for (unsigned int i = 0; i<256; i++)
            {
                if (maxf < buf[i])
                {
                    maxf = buf[i];
                }
            }
            for (unsigned int i = 0; i<size; i++)
            {
                data2[i] = height * (1 - buf[i * 50 / size] / maxf);
                if (data2[i] > height)
                    data2[i] = height;
                if (data2[i] < 0) 
                    data2[i] = 0;
                data3[i] += data2[i];
            }
            nCount++;
        }
    }

    if (nCount)
    {
        for (unsigned int i = 0; i<size; i++)
        {
            data[i] = (data[i] + 7 * data3[i]) / (nCount * 8);
            if (data[i] > height)
                data[i] = height;
            if (data[i] < 0) 
                data[i] = 0;
        }
    }
    else
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
}

void Spectrum::render()
{
    float r,g,b;
    int i, j, x, y;
    const int h = height*::size/3;
    //glPushMatrix();
    //glEnable(GL_DEPTH_TEST);
    //glTranslatef(0,0,-10.f);k
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_SRC_ALPHA);
    glBegin(GL_QUADS);
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

            glVertex2d(x+2,y+2);
            glVertex2d(x+::size,y+2);
            glVertex2d(x+::size,y+::size/3);
            glVertex2d(x+2,y+::size/3);
        }
        x += ::size;
    }
    glEnd();
    //glDisable(GL_DEPTH_TEST);
    //glPopMatrix();
}
