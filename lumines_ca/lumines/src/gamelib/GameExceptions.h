#ifndef _GameExceptions_H_
#define _GameExceptions_H_

#include <exception>

namespace gameexception
{
    class bad_index : public std::exception
    {
    };
    class bad_pointer : public std::exception
    {
    };
}
#endif//_GameExceptions_H_
