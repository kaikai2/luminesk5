#ifndef _GameEvent_SetPosition_H_
#define _GameEvent_SetPosition_H_

#include "IGameEvent.h"

class GameEventSetPosition : public IGameEvent
{
public:
    GameEventSetPosition(float x, float y) : m_x(x), m_y(y) {}
    virtual ~GameEventSetPosition(){}

    virtual int ID()
    {
        return s_id;
    }
    static int Id()
    {
        return s_id;
    }

    float m_x, m_y;
private:
    static int s_id;
};

#endif//_GameEvent_SetPosition_H_
