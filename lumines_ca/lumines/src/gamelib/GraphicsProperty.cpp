#include "GraphicsProperty.h"
#include "IGameEvent.h"
#include "IGameEntity.h"
#include "EventSetPosition.h"
#include "EventOnAttach.h"
#include "GameEvent.h"
#include "GameAttribute.h"
#include <cassert>

const int GraphicsProperty::s_id = GameEvent::Generate();

GraphicsProperty::GraphicsProperty()
{
}

GraphicsProperty::~GraphicsProperty()
{
}

void GraphicsProperty::Response(IGameEvent &rEvent)
{
    if (rEvent.ID() == GameEventSetPosition::Id())
    {
        GameEventSetPosition &rEventSP = (GameEventSetPosition &)rEvent;

    }
    else if (rEvent.ID() == GameEventOnAttach::Id())
    {
        GameEventOnAttach &rEventOA = (GameEventOnAttach &)rEvent;
        try
        {
            GA_FileName &rFileName = (GA_FileName &)rEventOA.m_rEntity.Attribute(GA_FileName::s_id);
        }
        catch(gameexception::bad_index &)
        {
            assert(0 && "Attribute 'GA_FileName' not attached ?");
        }
        catch(...)
        {
            assert(0);
        }
    }
}
