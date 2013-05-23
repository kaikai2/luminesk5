#include "GameEntity.h"
#include "GameExceptions.h"
#include "IGameProperty.h"
#include "IGameAttribute.h"
#include "EventOnAttach.h"
#include <cassert>

GameEntity::GameEntity()
{
}

GameEntity::~GameEntity()
{
    for (std::map<int, IGameProperty *>::iterator it = m_mapProperties.begin();
        it != m_mapProperties.end(); ++it)
    {
        delete it->second;
    }
    m_mapProperties.clear();

    for (std::map<int, IGameAttribute *>::iterator it = m_mapAttributes.begin();
        it != m_mapAttributes.end(); ++it)
    {
        delete it->second;
    }
    m_mapAttributes.clear();
}

void GameEntity::Response(IGameEvent &rEvent)  // 响应消息
{
    for (std::map<int, IGameProperty *>::iterator it = m_mapProperties.begin();
        it != m_mapProperties.end(); ++it)
    {
        assert(it->second);
        if (it->second)
        {
            it->second->Response(rEvent);
        }
    }
}

IGameProperty &GameEntity::Property(int index) // 性质
{
    std::map<int, IGameProperty *>::iterator it = m_mapProperties.find(index);
    if (it == m_mapProperties.end())
    {
        throw gameexception::bad_index();
    }
    if (it->second == 0)
    {
        throw gameexception::bad_pointer();
    }
    return *it->second;
}

IGameAttribute &GameEntity::Attribute(int index) // 属性
{
    std::map<int, IGameAttribute *>::iterator it = m_mapAttributes.find(index);
    if (it == m_mapAttributes.end())
    {
        throw gameexception::bad_index();
    }
    if (it->second == 0)
    {
        throw gameexception::bad_pointer();
    }
    return *it->second;
}

void GameEntity::AttachProperty(IGameProperty &rProperty)
{
    try
    {
        Property(rProperty.Id());
        assert(0);
        return;
    }
    catch(gameexception::bad_index &)
    {
        // 这样才对么
        m_mapProperties[rProperty.Id()] = &rProperty;

        GameEventOnAttach event(*this);
        rProperty.Response(event);
    }
    catch(gameexception::bad_pointer &)
    {
        assert(0);
    }
}

void GameEntity::AttachAttribute(IGameAttribute &rAttribute)
{
    try
    {
        Attribute(rAttribute.Id());
        assert(0);
        return;
    }
    catch(gameexception::bad_index &)
    {
        // 这样才对么
        m_mapAttributes[rAttribute.Id()] = &rAttribute;
    }
    catch(gameexception::bad_pointer &)
    {
        assert(0);
    }
}