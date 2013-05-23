#ifndef _Logic_FSM_State_H_
#define _Logic_FSM_State_H_

#include <cassert>
#include <string>
#include <map>
using std::string;
using std::map;

class Event;
class Fsm;

class State
{
	State & operator = (const State &rhs)
    {
        assert(0);
        return *this;
    }
    const string name;
    int id;
public:
    typedef void (*fun_ptr_State)(State &);
    typedef void (*fun_ptr_Event)(Event &);
    State(const string &_name
        , int _id
        , fun_ptr_State _On_Start = 0
        , fun_ptr_State _On_End = 0
        , fun_ptr_Event _On_Event = 0)
        : name(_name)
        , id(_id)
        , On_Start(_On_Start)
        , On_End(_On_End)
        , On_Event(_On_Event)
    {
    }

    fun_ptr_State On_Start;
    fun_ptr_State On_End;
    fun_ptr_Event On_Event;

    //void SetStartHandle(fun_ptr_State _On_Start)
    //{
    //    On_Start = _On_Start;
    //}
    //void SetEndHandle(fun_ptr_State _On_End)
    //{
    //    On_End = _On_End;
    //}
    //void SetEventHandle(fun_ptr_Event _On_Event)
    //{
    //    On_Event = _On_Event;
    //}
    int ID() const
    {
        return id;
    }
	const string &Name() const
	{
		return name;
	}
private:
    friend class Fsm;
    void OnStart(State &rState) const;
    void OnEnd(State &rState) const;
    void OnEvent(Event &rEvent) const;
};

#endif//_Logic_FSM_State_H_
