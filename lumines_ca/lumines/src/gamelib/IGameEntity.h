#ifndef _IGameEntity_H_
#define _IGameEntity_H_

class IGameProperty;
class IGameAttribute;
class IGameEvent;
#include "GameExceptions.h"
class IGameEntity
{
public:
    IGameEntity(){}
    virtual ~IGameEntity(){}

    virtual void Response(IGameEvent &rEvent) = 0;  // ��Ӧ��Ϣ

    virtual IGameProperty &Property(int index) throw (gameexception::bad_index, gameexception::bad_pointer)= 0; // ����

    virtual IGameAttribute &Attribute(int index)throw (gameexception::bad_index, gameexception::bad_pointer) = 0; // ����
};

#endif//_IGameEntity_H_
