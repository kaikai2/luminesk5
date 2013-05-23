#ifndef _Logic_FSM_Event_H_
#define _Logic_FSM_Event_H_

#include <string>
using std::string;

class Event;

class Event
{
public:
    Event(const string &_name, int _id)
        : name(_name), id(_id)
    {
    }

    int ID() const
    {
        return id;
    }
    const string &Name() const
    {
        return name;
    }
private:
    string name;
    int id;
};

#endif//_Logic_FSM_Event_H_
