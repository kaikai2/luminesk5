#ifndef LUMINES_SCANNER_H
#define LUMINES_SCANNER_H

class Lumines;

class Scanner
{
public:
    void init(Lumines *const _lumines, /*const ogl2d::Font *_font, */int pixwidth, int pixheight);
    void step();
    void render();
    void scan();
    int getX(){return (int)x/(LUMINES_BLOCK_SIZE+1);}
    void setScanSpeed(float speed);
    int getThroughCount(){return throughCount;}
private:
    float x;
    float scan_speed;
    int last_pos;
    int width, height;
    Lumines *lumines;
    bool bScan;
    int throughCount; //扫到的大方块总数量
    //const ogl2d::Font *font;
};

#endif//LUMINES_SCANNER_H
