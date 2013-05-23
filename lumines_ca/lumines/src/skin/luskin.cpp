#include <cstdio>
#include <sstream>
#include "luskin.h"
#include "../effect/sound.h"

using namespace std;

iAnimResManager *SkinPack::resManager = 0;
void SkinPack::loadFromFile(const char *name, const char* szFilename)
{
	static char *s_name[NumAnimType] =
	{
		"AT_BLOCKA",
		"AT_BLOCKB",
		"AT_SPECIAL",
		"AT_SPECIALCONNECTED",
		"AT_ACTIVATED",
		"AT_SPECIALACTIVATED",

		"AT_UNIONA",
		"AT_UNIONB",
	};
	for (int i = 0; i < NumAnimType; i++)
	{
		if (strcmp(name, s_name[i]) == 0)
		{
			loadFromFile((AnimType)i, szFilename);
			break;
		}
	}
}

void SkinPack::loadFromFile(AnimType at, const char* szFilename)
{
    if (resManager == 0)
    {
        resManager = iSystem::GetInstance()->createAnimResManager();
    }
    anim[at] = resManager->getAnimData(szFilename);
    if (anim[at] == NULL)
    {
        assert(0);
        MessageBox(0, "Failed to load anim data:", szFilename, MB_OK);
        exit(1);
    }
    valid = true;
}
void SkinPack::release()
{
    if (!valid)
        return;
    for (int i = 0; i < NumAnimType; i++)
        anim[i] = 0;
    valid = false;
}
//LuSkin::LuSkin(){}
LuSkin::~LuSkin()
{
    release();
}
void LuSkin::release()
{
    current.release();
    next.release();
}

void LuSkin::load(const char* path, const char* szFilename)
{
    char name[256], buf[256], linebuf[512];
    stringstream s;
    s << path << '/' << szFilename;
    FILE *fp = fopen(s.str().c_str(), "rt");
    if (fp == NULL)
        return;

    next.release();

    //bool error = false;
    //int PACE = 60;

    while(!feof(fp))
    {
        fgets(linebuf, 512, fp);
        if (sscanf(linebuf, "%s%s", name, buf) != 2)
            continue;
        strupr(name);
        if (name[0] == '#')
            continue;
        if (buf[0] == '*')
            continue;

        if (strncmp(name, "BLOCK.", 6) == 0)
        {
            sprintf(linebuf, "%s/%s", path, buf);
			next.loadFromFile(name + 6, linebuf);
        }
        else if (strncmp(name, "SOUND", 5) == 0)
        {
            Sound *pSound = Sound::getInstance();
            pSound->loadSkin(path, buf);
        }
    }
    fclose(fp);
}
void LuSkin::setSkinChangeStep()
{
    if (current.valid && next.valid)
    {
        step += 0.01f;
    }
    if (!current.valid || step >= 0.99)
    {
        current = next;
        step = 0;
        next.release();
    }
}
