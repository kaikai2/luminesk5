#ifndef __POINT_2_H_
#define __POINT_2_H_

#import <cmath>

namespace ogl2d
{
template< typename T >
struct _Point2
{
    T x, y;
    _Point2():x(0), y(0)
    {
    }
    _Point2(const _Point2 &p):x(p.x), y(p.y)
    {
    }
    _Point2(T _x, T _y):x(_x), y(_y)
    {
    }
    _Point2& operator *= (T s)
    {
        x *= s;
        y *= s;
        return *this;
    }
    _Point2 operator * (T s) const
    {
        _Point2 o = *this;
        o *= s;
        return o;
    }
    _Point2& operator += (const _Point2& a)
    {
        x += a.x;
        y += a.y;
        return *this;
    }
    _Point2 operator + (const _Point2& b) const
    {
        _Point2 o = *this;
        return o += b;
    }
    _Point2& operator -= (const _Point2& a)
    {
        x -= a.x;
        y -= a.y;
        return *this;
    }
    _Point2 operator - (const _Point2& b) const
    {
        _Point2 o = *this;
        return o -= b;
    }
    _Point2 operator - () const
    {
        return _Point2(-x, -y);
    }
    _Point2 operator / (T t) const
    {
        return _Point2(x/t, y/t);
    }
    T DotProduct() const
    {
        return x * x + y * y;
    }
    T operator * (const _Point2 &a) const
    {
        return x * a.x + y * a.y;
    }
/*
    friend const _Point2 CrossProduct(const _Point2 &a, const _Point2 &b)
    {
        return _Point2(a.x * b.y - b.x * a.y, a.x * b.y - b.x * a.y);
    }
*/
    template< typename T2 >
    operator _Point2<T2> () const
    {
        _Point2<T2> t;
        t.x = static_cast<T2>(x);
        t.y = static_cast<T2>(y);
        return t;
    }
    T Length() const
    {
        return x + y;
    }
    _Point2& Normalize()
    {
        return *this = *this / Length();
    }
    _Point2& Normalize(float len)
    {
        return *this = *this * (len / Length());
    }
};

template<>
inline float _Point2<float>::Length() const
{
    return sqrtf(DotProduct());
}
template<>
inline double _Point2<double>::Length() const
{
    return sqrt(DotProduct());
}
typedef _Point2< float > Point2f;
typedef _Point2< int > Point2i;

} // namespace ogl2d

#endif // __POINT_2_H_