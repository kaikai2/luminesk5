#include "InputProperty.h"
#include "IGameEvent.h"
#include "IGameEntity.h"
//#include "EventSetPosition.h"
//#include "EventOnAttach.h"
#include "GameEvent.h"
//#include "GameAttribute.h"
#include <cassert>

const int InputProperty::s_id = GameEvent::Generate();

InputProperty::InputProperty()
{
}

InputProperty::~InputProperty()
{
}

void InputProperty::Response(IGameEvent &rEvent)
{
    //if (rEvent.ID() == GameEventSetPosition::Id())
    //{
    //    GameEventSetPosition &rEventSP = (GameEventSetPosition &)rEvent;
    //    if (m_pSprite)
    //    {
    //        m_pSprite->SetPosition(rEventSP.m_x, rEventSP.m_y);
    //    }
    //}
    //else if (rEvent.ID() == GameEventOnAttach::Id())
    //{
    //    GameEventOnAttach &rEventOA = (GameEventOnAttach &)rEvent;
    //    SafeDelete(m_pSprite);
    //    try
    //    {
    //        GA_FileName &rFileName = (GA_FileName &)rEventOA.m_rEntity.Attribute(GA_FileName::s_id);

    //        extern LPDIRECT3DDEVICE9 g_Device;
    //        m_pSprite = new Sprite(g_Device); /// sprite factory needed
    //        m_pSprite->InitSprite(rFileName.m_value.c_str());
    //    }
    //    catch(gameexception::bad_index &)
    //    {
    //        assert(0 && "Attribute 'GA_FileName' not attached ?");
    //    }
    //    catch(...)
    //    {
    //        assert(0);
    //    }
    //}
}
