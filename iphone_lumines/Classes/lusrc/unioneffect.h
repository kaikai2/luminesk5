#ifndef UNION_EFFECT_H
#define UNION_EFFECT_H

#import <vector>
#import <string>
#import <ogl2d.h>

/// 方块联合时的特效
class UnionEffect
{
public:
    UnionEffect();
    virtual ~UnionEffect();
    void push(int x, int y, int type);
    void step();
    void render();
private:
    struct node
    {
        int start;
        int x, y;
        float color[4];
        int type;
    };
    ::std::vector< node > effects;
    static bool inited;
    static DWORD curframe;
};

#endif//UNION_EFFECT_H