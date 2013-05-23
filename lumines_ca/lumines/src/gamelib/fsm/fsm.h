#ifndef _Logic_FSM_FSM_H_
#define _Logic_FSM_FSM_H_

#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;

class Event;
class State;

class TransitionMap
{
public:
	void AddRule(const string &rFrom, int id, const string &rTo)
	{
		theMap[rFrom][id] = rTo;
	}
	const string &Get(const string &rFrom, int id) const
	{
		map<string, map<int, string> >::const_iterator it = theMap.find(rFrom);
		if (it != theMap.end())
		{
			map<int, string>::const_iterator it2 = it->second.find(id);
			if (it2 != it->second.end())
			{
				return it2->second;
			}
		}
		static string s;
		return s;
	}
private:
	map<string, map<int, string> > theMap;
};

class Fsm
{
public:
    Fsm(const TransitionMap &rtm) : rTransitionMap(rtm), pCurState(0)
    {
    }
    ~Fsm();

    State &GetState(const string &name);

    void Input(Event &rEvent);

    void Start(const string &name);

	const TransitionMap &GetTransitionMap() const
	{
		return rTransitionMap;
	}
private:
    map<string, size_t> name2stateIndex;
	vector<State*> vecStates;

	const TransitionMap &rTransitionMap;
    State *pCurState;
};

#endif//_Logic_FSM_FSM_H_
