#ifndef _InputProperty_H_
#define _InputProperty_H_

#include "IGameProperty.h"

class InputProperty : public IGameProperty
{
public:
    InputProperty();
    virtual ~InputProperty();

    virtual int Id() const// ʶ��id
    {
        return s_id;
    }

    virtual void Response(IGameEvent &rEvent); // ��Ӧ��Ϣ

private:
    static const int s_id;
};
#endif//_InputProperty_H_
