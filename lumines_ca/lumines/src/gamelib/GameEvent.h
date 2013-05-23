#ifndef _GameEvent_H_
#define _GameEvent_H_

#include "IGameEvent.h"

//class GameEvent : public IGameEvent
//{
//public:
//    GameEvent(){}
//    virtual ~GameEvent(){}
//
//    virtual int ID()
//    {
//        return id;
//    }
//private:
//    static int id;
//};

class GameEvent
{
public:
    static int Generate()
    {
        static int s_a = 0;
        return ++s_a;
    }
};

#endif//_GameEvent_SetPosition_H_
