#ifndef _GameAttribute_H_
#define _GameAttribute_H_

#include "IGameAttribute.h"
#include "GameEvent.h"
#include <string>
template<typename T, typename Tag = T>
class GameAttribute : public IGameAttribute
{
public:
    GameAttribute(){}
    GameAttribute(Tag){}
    virtual ~GameAttribute(){}
    virtual int Id() const
    {
        return s_id;
    }
    static const int s_id;
    T m_value;
};

typedef GameAttribute<int> GA_int;
typedef GameAttribute<float> GA_float;
typedef GameAttribute<bool> GA_bool;

struct FileName{};

template<typename T, typename Tag>
const int GameAttribute<T, Tag>::s_id = GameEvent::Generate();

typedef GameAttribute<std::string, FileName> GA_FileName;


#endif//_GameAttribute_H_
