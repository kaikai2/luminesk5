#ifndef LUMINES_LEVEL_H
#define LUMINES_LEVEL_H

#include <vector>

struct SkinData
{
    int levelNeeded;
    char path[32];
    char name[32];
    float pace_speed;
    float fall_speed;
    bool operator < (const SkinData &o)const
    {
        return levelNeeded < o.levelNeeded;
    }
};

class LuLevel
{
public:
    LuLevel();
    virtual ~LuLevel()
    {
        skins.clear();
    }
    bool load(const char *szFilename);
    const SkinData* step(int level);
    const SkinData* getSkinData()
    {
        if (curskin >= skins.size())
            return NULL;
        return &skins[curskin];
    }
private:
    ::std::vector< SkinData > skins;
    unsigned int curskin;
};
#endif//LUMINES_LEVEL_H