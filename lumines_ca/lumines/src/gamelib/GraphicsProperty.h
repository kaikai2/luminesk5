#ifndef _GraphicsProperty_H_
#define _GraphicsProperty_H_

#include "IGameProperty.h"
//class IGameMesh;// 假设是画这个

class GraphicsProperty : public IGameProperty
{
public:
    GraphicsProperty();
    virtual ~GraphicsProperty();

    virtual int Id() const// 识别id
    {
        return s_id;
    }

    virtual void Response(IGameEvent &rEvent); // 响应消息

private:


    static const int s_id;
};
#endif//_GraphicsProperty_H_
