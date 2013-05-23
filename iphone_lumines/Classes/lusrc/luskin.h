#ifndef LUMINES_SKIN_H
#define LUMINES_SKIN_H

#import <ogl2d.h>

using namespace ogl2d;

struct SkinPack
{
    bool valid;
    TexturePtr block_tex;
    SkinPack():valid(false),block_tex(){}
    virtual ~SkinPack(){release();}
    void loadFromFile(const char* szFilename);
    void release();
};


class LuSkin
{
public:
    //LuSkin();
    virtual ~LuSkin();
    void load(const char* path, const char* szFilename);
    void setSkinChangeStep();
    void release();
    const SkinPack * getSkin(bool isCurrent){return isCurrent?&current:&next;}
    float getSkinChangeStep(){return step;}
private:
    float step;
    SkinPack current, next;
};  
#endif//LUMINES_SKIN_H