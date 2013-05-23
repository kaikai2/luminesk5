#ifndef _IGameAttribute_H_
#define _IGameAttribute_H_

class IGameAttribute
{
public:
    IGameAttribute(){}
    virtual ~IGameAttribute(){}

    virtual int Id() const = 0;
};

#endif//_IGameAttribute_H_
