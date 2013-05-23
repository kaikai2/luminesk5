#ifndef LUMINES_BG_H
#define LUMINES_BG_H

#import "lucolor.h"

class Background
{
public:
    void init(int w, int h);
    void step();
    void render();
    void flash(int size);
private:
    int width, height;
    LuColor c[4];
    float flashAlpha;
};

#endif//LUMINES_BG_H
