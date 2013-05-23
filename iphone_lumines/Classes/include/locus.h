#ifndef __LOCUS_H_
#define __LOCUS_H_

#import <cassert>
#import <vector>
#import <algorithm>

#define LOCUS_MAX_NODE_COUNT 100

namespace ogl2d
{

/// LocusType
/// @note   轨迹类型
enum LocusType
{
    LT_UNKNOWN = -1,
    LT_LINEAR = 0,  /// 线性插值轨迹
    LT_B_SPLINE,    /// b样条插值轨迹
    LT_STEP,        /// 跳变（不插值）轨迹
};
class bitstream;
/// LocusBase
/// @note   轨迹公共基类
class LocusBase
{
public:
    int m_time;
    virtual LocusType getLocusType() const = 0;
    virtual void loadData(const char *data) = 0;
    virtual void loadData(bitstream *bs) = 0;
    virtual unsigned int dataSize() = 0;
protected:
    LocusBase():m_time(0)
    {
    }
};

/// Locus
/// @note   轨迹，一个描述时间-状态的函数
template< class Point >
class Locus : public LocusBase
{
public:
    Locus();
    struct Node
    {
        Node():m_time(0), m_p()
        {
        }
        Node(const Node &n):m_time(n.m_time), m_p(n.m_p)
        {
        }
        Node(int time, const Point &p):m_time(time), m_p(p)
        {
        }
        int m_time;
        Point m_p;
        bool operator < (int time) const
        {
            return m_time < time;
        }
        bool operator < (const Node &o) const
        {
            return m_time < o.m_time;
        }
    };
    virtual LocusType getLocusType() const {return LT_UNKNOWN;}
    void loadData(const char *data);
    
    void loadData(bitstream *bs);

    /* addData
    @note    add a Single data Point `p' at `time'. the data need to be sorted after this process.
    */
    void addData(int time, const Point &p);

    /* getInterval
    @note   find the time in datum, calculate the interval by LocusType
    */
    virtual int getInterval(int time, Point &result) = 0;
    
    /* getData
    @note   get the index-th data
    */
    bool getData(unsigned int index, int &time, Point &result);

    /* setData
    @note   set the index-th data, may be sorted after the process
    */
    bool setData(unsigned int index, int time, const Point &p);

    unsigned int dataSize(){return (unsigned int)m_data.size();}
    ::std::vector< Node > m_data;
    bool m_bNeedSort;
};

/// LocusLinear 
/// @note 线性插值轨迹
template< class Point >
class LocusLinear : public Locus< Point > 
{
public:
    LocusLinear();
    int getInterval(int time, Point &result);
    LocusType getLocusType() const {return LT_LINEAR;}
	struct Node
    {
        Node():m_time(0), m_p()
        {
        }
        Node(const Node &n):m_time(n.m_time), m_p(n.m_p)
        {
        }
        Node(int time, const Point &p):m_time(time), m_p(p)
        {
        }
        int m_time;
        Point m_p;
        bool operator < (int time) const
        {
            return m_time < time;
        }
        bool operator < (const Node &o) const
        {
            return m_time < o.m_time;
        }
    };
	
};

/// LocusLinear 
/// @note b样条插值轨迹
template< class Point >
class LocusBspline : public Locus< Point > 
{
public:
    LocusBspline();
    int getInterval(int time, Point &result);
    LocusType getLocusType() const {return LT_B_SPLINE;}
	struct Node
    {
        Node():m_time(0), m_p()
        {
        }
        Node(const Node &n):m_time(n.m_time), m_p(n.m_p)
        {
        }
        Node(int time, const Point &p):m_time(time), m_p(p)
        {
        }
        int m_time;
        Point m_p;
        bool operator < (int time) const
        {
            return m_time < time;
        }
        bool operator < (const Node &o) const
        {
            return m_time < o.m_time;
        }
    };
};

/// LocusLinear 
/// @note 跳变插值轨迹
template< class Point >
class LocusStep : public Locus< Point > 
{
public:
    LocusStep();
    int getInterval(int time, Point &result);
    LocusType getLocusType() const {return LT_STEP;}
	struct Node
    {
        Node():m_time(0), m_p()
        {
        }
        Node(const Node &n):m_time(n.m_time), m_p(n.m_p)
        {
        }
        Node(int time, const Point &p):m_time(time), m_p(p)
        {
        }
        int m_time;
        Point m_p;
        bool operator < (int time) const
        {
            return m_time < time;
        }
        bool operator < (const Node &o) const
        {
            return m_time < o.m_time;
        }
    };
};

} // namespace ogl2d

#ifndef USE_EXPORT_KEYWORD
   #import "locus.cpp"
#endif 

#endif // __LOCUS_H_