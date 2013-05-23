#include "lueffmgr.h"

#include "bg.h"
#include "combotext.h"
#include "unioneffect.h"
#include "ExplodeEffect.h"
#include "shine.h"
#include "spectrum.h"
#include "lumotionblur.h"

#ifndef SAFEDELETE
#define SAFEDELETE(a) \
    if(!(a))\
    ;   \
    else    \
    {       \
    delete (a); \
    (a) = NULL; \
    }
#endif
//extern ogl2d::TextureResource *g_pTexres;

class EffectPack
{
public:
    EffectPack(cAni::iAnimResManager &rARM/*ogl2d::Font *const fontlist[]*/)
        :/*font(fontlist[0])
        ,scorefont(fontlist[1])
        ,combotext(scorefont)
        ,*/
        combotext(rARM)
        ,unioneffect(rARM)
        ,explodeeffect(rARM)
        ,spectrum(16,30)
    {
    }
    void init()
    {
        bg.init(480, 272);
        unsigned char buf[512*512*3];

        motionblur.init(&glt);
    }
    void step(std::vector<DWORD> &rVecCurrentChannel)
    {
        bg.step();
        combotext.step();
        unioneffect.step();
        explodeeffect.step();
        shine.step();
        spectrum.step(rVecCurrentChannel);
    }
    void renderBackground()
    {
        //motionblur.render();
        bg.render();        
    }
    void renderSpectrum()
    {
        spectrum.render();
    }
    void render()
    {
        //motionblur.step();
        combotext.render();
        unioneffect.render();
        explodeeffect.render();
        shine.render();
    }
    Background bg;
    /*ogl2d::Font *const font;
    ogl2d::Font *const scorefont;*/
    ComboText combotext;
    UnionEffect unioneffect;
    ExplodeEffect explodeeffect;
    Shine shine;
    Spectrum spectrum;
    LuMotionBlur motionblur;

    glTexture glt;
};
static EffectPack *s_pInstance = NULL;
void LuEffectManager::release()
{
    SAFEDELETE(s_pInstance);
}
void LuEffectManager::create(cAni::iAnimResManager &rARM/*ogl2d::Font *const fontlist[]*/)
{
    release();
    s_pInstance = new EffectPack(rARM/*fontlist*/);
    if (s_pInstance != NULL)
    {
        s_pInstance->init();
    }
}
void LuEffectManager::step(std::vector<DWORD> &rVecCurrentChannel)
{
    if (s_pInstance != NULL)
        s_pInstance->step(rVecCurrentChannel);
}
void LuEffectManager::renderBackground()
{
    if (s_pInstance != NULL)
        s_pInstance->renderBackground();
}
void LuEffectManager::renderSpectrum()
{
    if (s_pInstance != NULL)
        s_pInstance->renderSpectrum();
}
void LuEffectManager::render()
{
    if (s_pInstance != NULL)
        s_pInstance->render();
}
void LuEffectManager::pushComboText(int x, int y, const char*str, int ani)
{
    if (s_pInstance != NULL)
    {
        s_pInstance->combotext.push(x, y, str, ani);
    }
}
void LuEffectManager::pushUnionEffect(int x,int y, int type)
{
    if (s_pInstance != NULL)
    {
        s_pInstance->unioneffect.push(x, y, type);
    }
}
void LuEffectManager::pushShine(int x, int y)
{
    if (s_pInstance != NULL)
    {
        s_pInstance->shine.push(x, y);
    }
}
void LuEffectManager::pushExplodeEffect(int x, int y, int type)
{
    if (s_pInstance != NULL)
    {
        s_pInstance->explodeeffect.push(x, y, type);
    }
}
void LuEffectManager::flashBackground(int count)
{
    if (s_pInstance != NULL)
    {
        s_pInstance->bg.flash(count);
    }
}
