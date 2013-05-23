#ifndef LUMINES_MOTION_BLUR_H
#define LUMINES_MOTION_BLUR_H

class LuMotionBlur
{
public:
    LuMotionBlur();
	virtual ~LuMotionBlur();
    void init(ogl2d::Texture *_tex);
    void step(); // call me after all that need to be in blur scene has been drawed.
    void render();
private:

    ogl2d::Texture *tex;
};

#endif