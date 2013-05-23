#import "lueffmgr.h"

#import "bg.h"
#import "combotext.h"
#import "unioneffect.h"
#import "ExplodeEffect.h"
#import "shine.h"
#import "spectrum.h"
#import "lumotionblur.h"

extern ogl2d::TextureResource *g_pTexres;

class EffectPack
{
public:
    EffectPack(ogl2d::Font *const fontlist[])
        :font(fontlist[0])
        ,scorefont(fontlist[1])
        ,combotext(scorefont)
        ,spectrum(16,30)
    {
    }
    void init()
    {
        bg.init(320, 480);
        unsigned char buf[512*512*3];
        motionblur.init(g_pTexres->NewTexture(buf,512,512,3));
    }
    void step()
    {
        bg.step();
        combotext.step();
        unioneffect.step();
        explodeeffect.step();
        shine.step();
        spectrum.step();
    }
    void renderBackground()
    {
//        motionblur.render();
       bg.render();        
    }
    void renderSpectrum()
    {
        spectrum.render();
    }
    void render()
    {
//        motionblur.step();
        combotext.render();
        unioneffect.render();
        explodeeffect.render();
        shine.render();
    }
    Background bg;
    ogl2d::Font *const font;
    ogl2d::Font *const scorefont;
    ComboText combotext;
    UnionEffect unioneffect;
    ExplodeEffect explodeeffect;
    Shine shine;
    Spectrum spectrum;
    LuMotionBlur motionblur;
};
static EffectPack *s_pInstance = NULL;
void LuEffectManager::release()
{
    SAFEDELETE(s_pInstance);
}
void LuEffectManager::create(ogl2d::Font *const fontlist[])
{
    release();
    s_pInstance = new EffectPack(fontlist);
    if (s_pInstance != NULL)
    {
        s_pInstance->init();
    }
}
void LuEffectManager::step()
{
    if (s_pInstance != NULL)
        s_pInstance->step();
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