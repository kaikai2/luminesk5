#ifndef _IGameEvent_H_
#define _IGameEvent_H_

class IGameEvent
{
public:
    IGameEvent(){}
    virtual ~IGameEvent(){}

    virtual int ID() = 0;
};

#endif//_IGameEvent_H_
