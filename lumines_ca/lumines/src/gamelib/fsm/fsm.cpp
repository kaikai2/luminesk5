#include <cassert>
#include "fsm.h"
#include "state.h"
#include "./event.h"

using std::pair;

Fsm::~Fsm()
{
    if (pCurState)
    {
        pCurState->OnEnd(*pCurState);
        pCurState = 0;
    }
	for (size_t i = 0; i < vecStates.size(); i++)
    {
        delete vecStates[i];
    }
    vecStates.clear();
    name2stateIndex.clear();
}

State &Fsm::GetState(const string &name)
{
    map<string, size_t>::iterator it = name2stateIndex.find(name);
    if (it == name2stateIndex.end())
    {
        State *pState = new State(name, 0);
        name2stateIndex.insert(it, pair<string, size_t>(name, vecStates.size()));
		vecStates.push_back(pState);
        return *pState;
    }
	if (it->second < vecStates.size())
	{
		return *vecStates[it->second];
	}
	assert(0);
	static State s_null("<<Null>>", -1);
	return s_null;
}

void Fsm::Input(Event &rEvent)
{
    if (!pCurState)
    {
		return;
	}

    pCurState->OnEvent(rEvent);

	map<string, size_t>::iterator it = name2stateIndex.find(GetTransitionMap().Get(pCurState->Name(), rEvent.ID()));
	if (it != name2stateIndex.end())
	{
		assert(it->second < vecStates.size());
		if (it->second < vecStates.size())
		{
			if (State *pNext = vecStates[it->second])
			{
				pCurState->OnEnd(*pNext);
				pNext->OnStart(*pCurState);
				pCurState = pNext;
			}
		}
	}
}

void Fsm::Start(const string &name)
{
    State &s = GetState(name);

    if (pCurState)
    {
        pCurState->OnEnd(s);
    }

    s.OnStart(s);

    pCurState = &s;
}
