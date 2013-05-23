#ifndef LUMINES_SCANNER_H
#define LUMINES_SCANNER_H

class Lumines;

class Scanner
{
public:
    enum ScanDirection
    {
        LeftToRight,
        TopToBottom,
    };
    Scanner(ScanDirection scanDirection):
      cDirection(scanDirection)
    {
        assert(scanDirection == LeftToRight || scanDirection == TopToBottom);
    }
    void init(Lumines *const _lumines, const ogl2d::Font *_font, int pixwidth, int pixheight);
    void step();
    void render();
    void scan();
    int getPos() const {return (int)scan_pos/(LUMINES_BLOCK_SIZE+1);}
    void setScanSpeed(float speed);
    int getThroughCount() const {return throughCount;}
    int getScanLength() const
    {
        return cDirection == LeftToRight ? width : height;
    }
private:
    float scan_pos;
    float scan_speed;
    int last_pos;
    int width, height;
    Lumines *lumines;
    bool bScan;
    int throughCount; //扫到的大方块总数量
    const ogl2d::Font *font;
    const ScanDirection cDirection;
};

class Boundary
{
public:
    void step()
    {
    }
    void render()
    {
    }
    int pos;
};
#endif//LUMINES_SCANNER_H