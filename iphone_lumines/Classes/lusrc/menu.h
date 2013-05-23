#ifndef LUMINES_MENU_H
#define LUMINES_MENU_H

//#import <windows.h>
#define KEYDOWN 1
#define KEYUP 0


struct MenuItem
{
    int id;
    char *name;
    int parentid;
};

typedef void (*MenuCallBack)(int id);

class Menu
{
public:
    Menu(const MenuItem *_list, MenuCallBack fn);
    //virtual ~Menu();
    void setCallBack(MenuCallBack fn)
    {
        callbackfn = fn;
    }
    bool msgHandle(char key,bool msg);
    void step();
    void render();
    void changeMenu(const MenuItem *_list)
    {
        list = _list;
        select = 0;
    }
    void setHandle(bool _bHandle)
    {
        bHandle = _bHandle;
    }
    void setVisible(bool _visible)
    {
        visible = _visible;
    }
private:

    void selectNext();
    void selectPrep();
    void turnLeft();
    void turnRight();
    void run();

    const MenuItem *list;
    MenuCallBack callbackfn;
    int select;
    bool bHandle;
    bool visible;

    int x, y;
};

#endif//LUMINES_MENU_H