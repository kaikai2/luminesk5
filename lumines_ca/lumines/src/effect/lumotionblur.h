#ifndef LUMINES_MOTION_BLUR_H
#define LUMINES_MOTION_BLUR_H

#include <glCurvedani.h>

class LuMotionBlur
{
public:
    LuMotionBlur();
	virtual ~LuMotionBlur();
    void init(glTexture *_tex);
    void step(); // call me after all that need to be in blur scene has been drawed.
    void render();
private:

    glTexture *tex;
};

#endif
