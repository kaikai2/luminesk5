#ifndef LUMINES_SKIN_H
#define LUMINES_SKIN_H

#include <glCurvedAni.h>

using namespace cAni;

struct SkinPack
{
    bool valid;
    enum AnimType
    {
        AT_BlockA,
        AT_BlockB,
        AT_Special,
        AT_SpecialConnected,
        AT_Activated,
        AT_SpecialActivated,

        AT_UnionA,
        AT_UnionB,

        NumAnimType,
    };
    const AnimData *anim[NumAnimType];
    SkinPack() : valid(false)
    {
        for (int i = 0; i < NumAnimType; i++)
            anim[i] = 0;
    }
    virtual ~SkinPack()
    {
        release();
    }
    void loadFromFile(const char *name, const char* szFilename);
    void loadFromFile(AnimType at, const char* szFilename);
    void release();

    static iAnimResManager *resManager;
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
