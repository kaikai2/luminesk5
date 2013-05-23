#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
#include "sound.h"

static struct LuminesSound
{
    HSTREAM bg[LSE_BG_COUNT];
    HFX fx[LSE_BG_COUNT];
    QWORD len[LSE_BG_COUNT];
    float lensec[LSE_BG_COUNT];

    HSAMPLE eff[LSE_MAX_COUNT];
} sndpack;

Sound* Sound::s_instance = NULL;

Sound::Sound():curbg(LSE_BG1),curbg2(LSE_BG1),bPlaybg(false)
{
    ZeroMemory(&sndpack, sizeof(LuminesSound));
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
    if (enable)BASS_Start();
    else BASS_Pause();
}
bool Sound::init()
{
    /* check that BASS 2.1 was loaded */
    if (BASS_GetVersion()!=MAKELONG(2,1)) {
        MessageBox(0, "BASS version 2.1 was not loaded\n", "Lumines", MB_OK);
        return false;
    }    
    /* 10ms update period */
    BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,10);

    /* setup output - get latency */
    if (!BASS_Init(1,44100,BASS_DEVICE_LATENCY,0,NULL))
    {
        MessageBox(0, "Can't initialize device", "Lumines", MB_OK);
        return false;
    }
    last = timeGetTime()-1;
    last_off = 0;
    delay = 60000 / (60*4);

    return true;
}

Sound::~Sound()
{
    freeSkin();
    BASS_Free();
}

void Sound::freeSkin()
{
    int i;
    for (i=0;i<LSE_BG_COUNT;i++)
    {
        BASS_ChannelRemoveFX(sndpack.bg[i], sndpack.fx[i]);
        sndpack.fx[i]=0;
        BASS_StreamFree(sndpack.bg[i]);
        sndpack.bg[i] = 0;
    }
    for (i=0;i<LSE_MAX_COUNT;i++)
    {
        BASS_SampleFree(sndpack.eff[i]);
        sndpack.eff[i] = 0;
    }
}

void Sound::loadSkin(const char* path, const char *skinfile)
{
    char name[256], buf[256], linebuf[512];
    freeSkin();
    sprintf(linebuf, "%s/%s", path, skinfile);
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

#define CASE_STREAM(name, filename, id)\
    if (strcmp(name, #id) == 0){\
        sprintf(linebuf, "%s/%s", path, filename);\
        sndpack.bg[id] = BASS_StreamCreateFile(FALSE, linebuf, 0, 0, 0);\
        if (sndpack.bg[id] == 0) error = true;\
        else{ \
            sndpack.len[id] = BASS_StreamGetLength(sndpack.bg[id]);\
            sndpack.lensec[id] = BASS_ChannelBytes2Seconds(sndpack.bg[id],sndpack.len[id]);\
        }\
        /*sndpack.fx[id]=BASS_ChannelSetFX(sndpack.bg[id], BASS_FX_REVERB,0);*/\
    }else

        CASE_STREAM(name, buf, LSE_BG1)     // 背景
        CASE_STREAM(name, buf, LSE_BG2)     // 背景
        CASE_STREAM(name, buf, LSE_BG3)     // 背景
        CASE_STREAM(name, buf, LSE_BG4)     // 背景
        CASE_STREAM(name, buf, LSE_BG5)     // 背景
        CASE_STREAM(name, buf, LSE_BG6)     // 背景

#define CASE(name, filename, id)\
    if (strcmp(name, #id) == 0){\
        sprintf(linebuf, "%s/%s", path, filename);\
        sndpack.eff[id] = BASS_SampleLoad(FALSE, linebuf, 0, 0, 5, 0);\
        if (sndpack.eff[id] == 0)\
            error = true;\
    }else

        CASE(name, buf, LSE_LEFT)           // 左移一格
        CASE(name, buf, LSE_RIGHT)          // 右移一格
        CASE(name, buf, LSE_LEFT_SLIDE)     // 快速左移
        CASE(name, buf, LSE_RIGHT_SLIDE)    // 快速右移
        CASE(name, buf, LSE_DOWN)           // 下落
        CASE(name, buf, LSE_ROTA)           // 正转（顺时针）
        CASE(name, buf, LSE_ROTB)           // 反转（逆时针）
        CASE(name, buf, LSE_UNION)          // 凝结方块
        CASE(name, buf, LSE_VANISH)         // 方块销毁
        CASE(name, buf, LSE_COMBO_SMALL)    // 小combo
        CASE(name, buf, LSE_COMBO_BIG)      // 大combo

#define CASE_SETTING(name, id, value)\
        if (strcmp(name, #id) == 0)\
        {   \
            id = atoi(value);   \
        } else

        CASE_SETTING(name, PACE, buf)   // PACE
        {
            assert(0);
        }
        if (error)
        {
            error = false;
            DWORD e = BASS_ErrorGetCode();
            e = e;
        }
    }
    fclose(fp);

    if (PACE != 0)
    {
        while (PACE<220)
        {
            PACE *= 2;
        }
        delay = 60000.f/PACE;
    }
    last_off = 0.f;
}

void Sound::playbg()
{
    bPlaybg = true;
    /*if (sndpack.bg[LSE_BG1])
    {
        BASS_ChannelPlay(sndpack.bg[LSE_BG1], FALSE);
    }*/
    playnextbg();
}

void Sound::stopbg()
{
    bPlaybg = false;
    if (sndpack.bg[curbg])
    {
        BASS_ChannelStop(sndpack.bg[curbg]);
    }
}

void Sound::playeffect(LuminesSoundEffectId id)
{
    if (id <= LSE_NO_EFFECT || id >= LSE_MAX_COUNT)
        return;
    /*
    if (sndpack.eff[id])
    {
        HCHANNEL channel = BASS_SampleGetChannel(sndpack.eff[id], false);
        BASS_ChannelPlay(channel, FALSE);
    }
    */
    soundqueue.push_back(id);
}
void CALLBACK MySyncProc(HSYNC handle, DWORD channel, DWORD data, DWORD user)
{
    Sound* pSound = (Sound*)user;
    pSound->playnextbg();
    // OutputDebugString("MySyncProc called\n");
}
void Sound::playnextbg()
{
    curbg = curbg2;
    curbg2 = LuminesSoundEffectId(rand()%6);
    for(int i=0;i<6;i++)
    {
        if (sndpack.bg[int(curbg2)])
            break;
        curbg2 = LuminesSoundEffectId((curbg2+1)%6);
    }            
    DWORD snd = sndpack.bg[int(curbg)];
    BASS_ChannelPlay(snd, TRUE);
    QWORD t2 = BASS_ChannelSeconds2Bytes(snd, 0.067f);
    DWORD t = (DWORD)(sndpack.len[int(curbg)] - t2);
    BASS_ChannelSetSync(snd,BASS_SYNC_POS|BASS_SYNC_ONETIME, t, &MySyncProc, (DWORD)this);
    //BASS_ChannelSetSync(snd,BASS_SYNC_END|BASS_SYNC_ONETIME, 0, &MySyncProc, (DWORD)this);
//    BASS_ChannelPreBuf(sndpack.bg[int(curbg2)]);
}
void Sound::step()
{
    DWORD now = timeGetTime();
    if (last - now < delay)
        return;
    last = int(last + delay + last_off);
    last_off += delay - int(delay);
    last_off -= int(last_off);

    ::std::sort(soundqueue.begin(), soundqueue.end());
    ::std::vector< LuminesSoundEffectId >::iterator sound = soundqueue.begin();
    LuminesSoundEffectId last = LSE_NO_EFFECT;
    while(sound != soundqueue.end())
    {
        if (sndpack.eff[*sound] && last != *sound)
        {
            last = *sound;
            HCHANNEL channel = BASS_SampleGetChannel(sndpack.eff[*sound], false);
            BASS_ChannelPlay(channel, FALSE);
        }
        ++sound;
    }
    soundqueue.clear();
}

std::vector<DWORD> Sound::getCurActive() const
{
    std::vector<DWORD> vecResult;
    vecResult.push_back(sndpack.bg[curbg]);
    for (int i = 0; i < LSE_MAX_COUNT; i++)
    {
        HCHANNEL channel = BASS_SampleGetChannel(sndpack.eff[LSE_MAX_COUNT], false);
        vecResult.push_back(channel);
    }
    return vecResult;
}
