#ifndef _GameEvent_OnAttach_H_
#define _GameEvent_OnAttach_H_

#include "IGameEvent.h"

class IGameEntity;

class GameEventOnAttach : public IGameEvent
{
public:
    GameEventOnAttach(IGameEntity &rEntity) : m_rEntity(rEntity){}
    virtual ~GameEventOnAttach(){}

    virtual int ID()
    {
        return s_id;
    }
    static int Id()
    {
        return s_id;
    }

    IGameEntity &m_rEntity;
private:
    static int s_id;
};

#endif//_GameEvent_OnAttach_H_
