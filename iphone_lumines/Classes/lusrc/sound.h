#ifndef LUMINES_SOUND_H
#define LUMINES_SOUND_H

#pragma warning(disable:4786)
#import <vector>


#import "soundlist_parser.h"

enum LuminesSoundEffectId
{
    LSE_BG1 = 0,
    LSE_BG2,
    LSE_BG3,
    LSE_BG4,
    LSE_BG5,
    LSE_BG6,
    LSE_BG_COUNT,

    LSE_NO_EFFECT = -1,
    LSE_LEFT = 0,    // 左移一格
    LSE_RIGHT,   // 右移一格
    LSE_LEFT_SLIDE,    // 快速左移
    LSE_RIGHT_SLIDE,   // 快速右移
    LSE_DOWN,    // 下落
    LSE_ROTA,    // 正转（顺时针）
    LSE_ROTB,    // 反转（逆时针）
    LSE_UNION,      // 凝结方块
    LSE_VANISH,     // 方块销毁
    LSE_COMBO_SMALL,  // 小combo
    LSE_COMBO_BIG,  // 大combo
    LSE_MAX_COUNT,
};

class Sound
{
public:
    static Sound* getInstance();
    static void releaseInstance();

    bool init();

    /// 读一套音乐配置
    void loadSkin(const char* path, const char *skinfile);

    /// 释放音乐配置
    void freeSkin();

    void step();

    void playbg();
    void stopbg();
    void playeffect(LuminesSoundEffectId id);
    void setEnable(bool enable);
private:
    void playnextbg();
    Sound();
    virtual ~Sound();
    static Sound* s_instance;

    ::std::vector< LuminesSoundEffectId > soundqueue;
    unsigned int last;
    float last_off;
    float delay;

    bool bPlaybg;
    LuminesSoundEffectId curbg,curbg2;
};

#endif//LUMINES_SOUND_H