#ifndef _GraphicsProperty_H_
#define _GraphicsProperty_H_

#include "IGameProperty.h"
//class IGameMesh;// �����ǻ����

class GraphicsProperty : public IGameProperty
{
public:
    GraphicsProperty();
    virtual ~GraphicsProperty();

    virtual int Id() const// ʶ��id
    {
        return s_id;
    }

    virtual void Response(IGameEvent &rEvent); // ��Ӧ��Ϣ

private:


    static const int s_id;
};
#endif//_GraphicsProperty_H_
