#ifndef LUMINES_GAME_H
#define LUMINES_GAME_H

enum PauseReason
{
    PR_NO_PAUSE = 0,
    PR_GAME_OVER = 2,
    PR_PAUSE = 4,
};
enum GameMode
{
    GM_SINGLE_CHALLENGE = 0,
    GM_MULTI_VS_CPU = 1,
    GM_MULTI_VS_2P = 2,
};

class iGameUI
{
public:
    virtual void pause(DWORD reason, bool enable);
    virtual void start(GameMode eGameMode);
};


#endif//LUMINES_GAME_H
