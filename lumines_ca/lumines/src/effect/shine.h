#ifndef LUMINES_SHINE_H
#define LUMINES_SHINE_H

#include <vector>

class Shine
{
public:
    void push(int x, int y);
    void step();
    void render();
private:
    struct node
    {
        int x, y;
        float a1, a2;
        float len1, len2;
    };
    ::std::vector< node > nodes;
};

#endif//LUMINES_SHINE_H
