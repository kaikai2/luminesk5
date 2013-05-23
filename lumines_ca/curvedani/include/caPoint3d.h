#ifndef __POINT_3_H_
#define __POINT_3_H_

namespace cAni
{
    template< class T >
    struct _Point3
    {
        T x, y, z;
        _Point3():x(0), y(0), z(0)
        {
        }
        _Point3(const _Point3 &p):x(p.x), y(p.y), z(p.z)
        {
        }
        _Point3(T _x, T _y, T _z):x(_x), y(_y), z(_z)
        {
        }
        const _Point3& operator *= (T s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }
        const _Point3 operator * (T s) const
        {
            _Point3 o = *this;
            o *= s;
            return o;
        }
        _Point3& operator += (const _Point3& a)
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }
        friend const _Point3 operator + (const _Point3& a, const _Point3& b)
        {
            _Point3 o = a;
            return o += b;
        }
        _Point3& operator -= (const _Point3& a)
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }
        friend const _Point3 operator - (const _Point3& a, const _Point3& b)
        {
            _Point3 o = a;
            return o -= b;
        }
        _Point3 operator - () const
        {
            return _Point3(-x, -y, -z);
        }
        _Point3 operator / (T t) const
        {
            return _Point3(x/t, y/t, z/t);
        }
        operator const T* () const
        {
            return (const T*)this;
        }
        bool operator == (const _Point3 &a) const
        {
            return x == a.x && y == a.y && z == a.z;
        }
        bool operator != (const _Point3 &a) const
        {
            return ! (*this == a);
        }
    };

    typedef _Point3< float > Point3f;
    typedef _Point3< int > Point3i;
    typedef _Point3< short > Point3s;

} // namespace cAni

#endif // __POINT_3_H_