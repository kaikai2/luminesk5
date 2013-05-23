#ifndef COMBO_TEXT_H
#define COMBO_TEXT_H

#include <vector>
#include <string>
#include <glCurvedAni.h>

class ComboText
{
public:
    ComboText(cAni::iAnimResManager &rARM/*const ogl2d::Font *_font = NULL*/);
    virtual ~ComboText();
    void push(int x, int y, const char* s, int ani=0);
    void step();
    void render();
private:
    struct node
    {
        int start;
        int x, y;
        float color[4];
        ::std::string s;
        int ani;
    };
    ::std::vector< node > texts;
    static bool inited;
    static DWORD curframe;
    //static const ogl2d::Font * font;

};
#endif//COMBO_TEXT_H
