#ifndef COMBO_TEXT_H
#define COMBO_TEXT_H

#import <vector>
#import <string>
#import <ogl2d.h>

class ComboText
{
public:
    ComboText(const ogl2d::Font *_font = NULL);
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
    static const ogl2d::Font * font;

};
#endif//COMBO_TEXT_H