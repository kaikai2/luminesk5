#import <cstdio>
#import <algorithm>
#import "lulevel.h"

LuLevel::LuLevel():curskin(0){}
//LuLevel::~LuLevel(){}
bool LuLevel::load(const char *szFilename)
{
    char path[256], name[256], linebuf[512];
    FILE *fp = fopen(szFilename, "rt");
    if (fp == NULL)
        return false;
    skins.clear();
    SkinData skindata;

    while(!feof(fp))
    {
        fgets(linebuf, 512, fp);
        if (linebuf[0] == '#')
            continue;
        if (sscanf(linebuf, "%d%s%s%f%f", &skindata.levelNeeded, path, name,
            &skindata.pace_speed, &skindata.fall_speed) != 5)
            continue;
        memset(skindata.path, 0, 32);
        strncpy(skindata.path, path, 32);
        if (skindata.path[31] != 0)
            continue;
        memset(skindata.name, 0, 32);
        strncpy(skindata.name, name, 32);
        if (skindata.name[31] != 0)
            continue;
        skins.push_back(skindata);
    }
    fclose(fp);
    ::std::sort(skins.begin(), skins.end());
    curskin = 0;
    return true;
}
const SkinData* LuLevel::step(int level)
{
    const SkinData* skindata = getSkinData();
    if (skindata == NULL)
        return NULL;
    
    if (level >= skindata->levelNeeded)
    {
        curskin++;
        return skindata;
    }
    return NULL;
}