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
    LSE_LEFT = 0,    // ����һ��
    LSE_RIGHT,   // ����һ��
    LSE_LEFT_SLIDE,    // ��������
    LSE_RIGHT_SLIDE,   // ��������
    LSE_DOWN,    // ����
    LSE_ROTA,    // ��ת��˳ʱ�룩
    LSE_ROTB,    // ��ת����ʱ�룩
    LSE_UNION,      // ���᷽��
    LSE_VANISH,     // ��������
    LSE_COMBO_SMALL,  // Сcombo
    LSE_COMBO_BIG,  // ��combo
    LSE_MAX_COUNT,
};

class Sound
{
public:
    static Sound* getInstance();
    static void releaseInstance();

    bool init();

    /// ��һ����������
    void loadSkin(const char* path, const char *skinfile);

    /// �ͷ���������
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