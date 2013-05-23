#ifndef _InputProperty_H_
#define _InputProperty_H_

#include "IGameProperty.h"

class InputProperty : public IGameProperty
{
public:
    InputProperty();
    virtual ~InputProperty();

    virtual int Id() const// 识别id
    {
        return s_id;
    }

    virtual void Response(IGameEvent &rEvent); // 响应消息

private:
    static const int s_id;
};
#endif//_InputProperty_H_
