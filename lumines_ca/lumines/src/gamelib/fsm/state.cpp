#include "state.h"
#include "event.h"
#include "fsm.h"

void State::OnStart(State &rState) const
{
    if (On_Start)
    {
        On_Start(rState);
    }
}

void State::OnEnd(State &rState) const
{
    if (On_End)
    {
        On_End(rState);
    }
}

void State::OnEvent(Event &rEvent) const
{
    if (On_Event)
    {
        On_Event(rEvent);
    }
}
