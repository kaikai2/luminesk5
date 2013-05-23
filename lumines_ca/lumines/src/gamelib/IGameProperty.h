#ifndef _IGameProperty_H_
#define _IGameProperty_H_

class IGameEvent;

class IGameProperty
{
public:
    IGameProperty(){}
    virtual ~IGameProperty(){}

    virtual int Id() const = 0;// ʶ��id

    virtual void Response(IGameEvent &rEvent) = 0; // ��Ӧ��Ϣ
};

#endif//_IGameProperty_H_
