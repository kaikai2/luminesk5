#ifndef LUMINES_ROBOT_H
#define LUMINES_ROBOT_H

#import <vector>
#import "block.h"

struct LuRobotResultPair
{
    int x, r;
};
struct Set
{
    Set(){memset(this,0,sizeof(Set));}
    Set(int _x, int _r, int _s, int _h):x(_x),r(_r),score(_s),height(_h){}
    unsigned char x, r;
    unsigned char height;
    int score;
    bool operator <(const Set &o) const
    {
        if (score > o.score)
            return true;
        if (score < o.score)
            return false;
        if (height > o.height)
            return true;
        return false;
    }
};
#define MAX_LEVEL 1
struct Sets
{
    Sets(){memset(this,0,sizeof(Sets));}
    Sets(int _x, int _r, int _s, int _h):level(0)
    {
        x[0] = _x;
        r[0] = _r;
        score = _s;
        height = _h;
    }
    Sets(const Sets &o, int _x, int _r, int _s, int _h)
    {
        level = o.level+1;
        assert(level >= 0 && level < MAX_LEVEL);
        memcpy(x, o.x, sizeof(x));
        memcpy(r, o.r, sizeof(r));
        x[level] = _x;
        r[level] = _r;
        score = o.score + _s;
        height = max(o.height, _h);
    }
    bool operator < (const Sets &o) const
    {
        if (score > o.score)
            return true;
        if (score < o.score)
            return false;
        if (height > o.height)
            return true;
        return false;
    }
    int level;
    unsigned char x[MAX_LEVEL], r[MAX_LEVEL];
    unsigned char height;
    int score;
};

class LuminesRobot
{
public:
//    LuminesRobot();
//    virtual ~LuminesRobot();

    /// think
    /// @note   reset the map data and make a new calculation
    ///         it is no need to call think() when getResult() returns true
    ///         and no map has not changed by scanner.
    /// @param  btype* map    :the data of the map
    /// @param  int width           :width of map
    /// @param  int height          :height of map
    /// @param  const BlockSet coming[3]   :the three comming blocks
    /// @see    getResult
    void think(btype *map, int width, int height, const BlockSet coming[3]);

    /// getResult
    /// @note   get the result of think()
    /// @param  int x  :recommended solution position
    /// @param  int r  :recommended solution rotation
    /// @return bool   :whether got a result
    bool getResult(int &x, int &r); // x is pos in map from left, y is 0 to 3 times rotate clock-wise

    //-----------------------------------------------------
private:
    /// getlink
    /// @note   count the link surround the block 'a' at (x,y)
    /// @return int     :link count
    /// @param  const btype* map    :the data of the map
    /// @param  int width           :width of map
    /// @param  int height          :height of map
    /// @param  int x,y             :postion of block 'a'
    /// @param  bttype a            :type of block 'a'
    int getlink(const btype *map, int width, int height, int x, int y, btype a);
    
    
    void thinkNextBlockI(
        Sets &baseSet,
        ::std::vector< int > &tops,
        ::std::vector< Sets > &sets,
        int level,
        btype *map, int width, int height, const BlockSet coming[3]);
    void thinkNextBlock(btype *map, int width, int height, const BlockSet coming[3]);
    
    ::std::vector< LuRobotResultPair > m_res;
};
#endif//LUMINES_ROBOT_H