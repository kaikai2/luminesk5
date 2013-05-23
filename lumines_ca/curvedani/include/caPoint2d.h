#ifndef __POINT_2_H_
#define __POINT_2_H_

#include <cmath>

namespace cAni
{
    template< typename T >
    struct _Point2
    {
        typedef T ValueType;
        typedef _Point2<T> SelfType;
        ValueType x, y;
        _Point2():x(0), y(0)
        {
        }
        _Point2(const SelfType &p):x(p.x), y(p.y)
        {
        }
        _Point2(const ValueType &_x, const ValueType &_y):x(_x), y(_y)
        {
        }
        template<typename T2>
        SelfType& operator *= (const T2 &s)
        {
            x = ValueType(x * s);
            y = ValueType(y * s);
            return *this;
        }
        template<typename T2>
        SelfType operator * (const T2 &s) const
        {
            SelfType o = *this;
            o *= s;
            return o;
        }
        SelfType& operator += (const SelfType& a)
        {
            x = x + a.x;
            y = y + a.y;
            return *this;
        }
        SelfType operator + (const SelfType& b) const
        {
            SelfType o = *this;
            return o += b;
        }
        SelfType& operator -= (const SelfType& a)
        {
            x = x - a.x;
            y = y - a.y;
            return *this;
        }
        SelfType operator - (const SelfType& b) const
        {
            SelfType o = *this;
            return o -= b;
        }
        SelfType operator - () const
        {
            return SelfType(-x, -y);
        }
        SelfType operator / (const ValueType &t) const
        {
            return SelfType(x/t, y/t);
        }
        SelfType& operator /= (const ValueType &t)
        {
            x /= t;
            y /= t;
            return *this;
        }
        ValueType DotProduct() const
        {
            return x * x + y * y;
        }
        ValueType operator * (const SelfType &a) const
        {
            return x * a.x + y * a.y;
        }
        friend const SelfType CrossProduct(const SelfType &a, const SelfType &b)
        {
            return SelfType(a.x * b.y - b.x * a.y, b.x * a.y - a.x * b.y);
        }
        ValueType operator ^ (const SelfType &a) const
        {
            return x * a.y - y * a.x;
        }
        template< typename T2 >
        operator _Point2<T2> () const
        {
            _Point2<T2> t;
            t.x = static_cast<T2>(x);
            t.y = static_cast<T2>(y);
            return t;
        }
        ValueType Length() const
        {
            return x + y;
        }
        SelfType& Normalize()
        {
            ValueType len = Length();
            if (len == 0)
                return *this = SelfType();
            else
                return *this = *this / len;
        }
        SelfType& Normalize(ValueType len)
        {
            ValueType _len = Length();
            if (_len == 0)
                len = 0;
            else
                len /= _len;
            return *this = *this * len;
        }
        bool operator == (const SelfType &a) const
        {
            return x == a.x && y == a.y;
        }
        bool operator != (const SelfType &a) const
        {
            return ! (*this == a);
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
    template<>
    inline int _Point2<int>::Length() const
    {
        return (int)sqrtf((float)DotProduct());
    }
    template<>
    inline short _Point2<short>::Length() const
    {
        return (short)sqrtf((float)DotProduct());
    }
    typedef _Point2< float > Point2f;
    typedef _Point2< int > Point2i;
    typedef _Point2< short > Point2s;

} // namespace cAni

#endif // __POINT_2_H_