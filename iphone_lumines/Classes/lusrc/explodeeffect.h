#ifndef EXPLODE_EFFECT_H
#define EXPLODE_EFFECT_H

#import <vector>
#import <ogl2d.h>

/// ��������ʱ�ı�ըЧ��
class ExplodeEffect
{
public:
    ExplodeEffect();
    virtual ~ExplodeEffect();
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

#endif//EXPLODE_EFFECT_H