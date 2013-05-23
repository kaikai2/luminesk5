#import <cstdio>
#import <cstring>
#import <cassert>
#import <algorithm>
#import "sound.h"



Sound* Sound::s_instance = NULL;

Sound::Sound()
{
    
}

Sound* Sound::getInstance()
{
    if (s_instance == NULL)
    {
        s_instance = new Sound;
    }
    return s_instance;
}

void Sound::releaseInstance()
{
    if (s_instance != NULL)
    {
        delete s_instance;
        s_instance = NULL;
    }
}
void Sound::setEnable(bool enable)
{
 
}
bool Sound::init()
{
    
    return true;
}

Sound::~Sound()
{
    freeSkin();

}

void Sound::freeSkin()
{
  
}

void Sound::loadSkin(const char* path, const char *skinfile)
{
   
}

void Sound::playbg()
{
   
}

void Sound::stopbg()
{
   
}

void Sound::playeffect(LuminesSoundEffectId id)
{
 
}

void Sound::playnextbg()
{
    
}
void Sound::step()
{
   
}