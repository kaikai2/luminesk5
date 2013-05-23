#import <cstdio>
#import "luskin.h"
#import "sound.h"

extern TextureResource *g_pTexres;
void SkinPack::loadFromFile(const char* szFilename)
{
    release();
    block_tex = g_pTexres->GetTexture(szFilename);
    if (block_tex == NULL)
    {
        assert(0);
        //MessageBox(0, "Failed to load texture:", szFilename, MB_OK);
		NSLog(@"Failed to load texture");
        exit(1);
    }
    valid = true;
}
void SkinPack::release()
{
    if (!valid)
        return;
    block_tex = TexturePtr(); // clear the texture reference
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

char   *strupr(char   *str) 
{ 
	char   *ptr   =   str; 
	
	while   (*ptr   !=   '\0 ')   { 
		if   (islower(*ptr))   
			*ptr   =   toupper(*ptr); 
		ptr++; 
	} 
	
	return   str; 
}

void LuSkin::load(const char* path, const char* szFilename)
{
    char name[256], buf[256], linebuf[512];
    sprintf(linebuf, "%s/%s", path, szFilename);
    FILE *fp = fopen(linebuf, "rt");
    if (fp == NULL)
        return;
    bool error = false;

    int PACE = 60;
    
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

        if (strncmp(name, "BLOCK", 5) == 0)
        {
            sprintf(linebuf, "%s/%s", path, buf);
            next.loadFromFile(linebuf);
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
