#ifndef _GameEntity_H_
#define _GameEntity_H_

#include "IGameEntity.h"
#include <map>

class GameEntity : public IGameEntity
{
public:
    GameEntity();
    virtual ~GameEntity();

    virtual void Response(IGameEvent &rEvent);  // ��Ӧ��Ϣ
    
    virtual IGameProperty &Property(int index); // ����

    virtual IGameAttribute &Attribute(int index); // ����

    void AttachProperty(IGameProperty &rProperty);
    void AttachAttribute(IGameAttribute &rAttribute);

private:
    std::map<int, IGameProperty *> m_mapProperties;
    std::map<int, IGameAttribute *> m_mapAttributes;
};

#endif//_GameEntity_H_
