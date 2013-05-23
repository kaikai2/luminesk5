#ifndef _LUMINES_EFFECT_MANAGER_H
#define _LUMINES_EFFECT_MANAGER_H

#import <ogl2d.h>
class LuEffectManager
{
public:
    static void pushComboText(int x, int y, const char*str, int ani);
    static void pushUnionEffect(int x,int y, int type);
    static void pushShine(int x, int y);
    static void pushExplodeEffect(int x, int y, int type);
    static void flashBackground(int count);

    static void create(ogl2d::Font *const fontlist[]);
    static void release();

    static void step();
    static void renderBackground();
    static void renderSpectrum();
    static void render();
};

#endif//_LUMINES_EFFECT_MANAGER_H