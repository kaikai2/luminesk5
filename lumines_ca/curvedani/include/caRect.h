#ifndef CURVEDANI_Rect_H
#define CURVEDANI_Rect_H

#include "caPoint2d.h"

namespace cAni
{
    template< class T >
    struct _Rect
    {
        _Rect();
        _Rect(T _left,T _right, T _top, T _bottom);
        _Rect(const _Rect &t);
        _Point2<T> leftTop, rightBottom;
        _Rect & operator &= (const _Rect &clip); // intersect rect
        _Rect operator & (const _Rect &clip) const; // intersect rect
        _Rect & operator += (const _Point2<T> &point); // offset rect by point
        _Rect operator + (const _Point2<T> &point) const; // offset rect by point
        bool operator & (const _Point2<T> &point) const; // check point in rect
        T distance(const _Point2<T> &point) const;
        bool operator == (const _Rect &o) const;
        _Rect operator + (const _Rect &o) const;
        _Rect operator - (const _Rect &o) const;
        template<typename T2>
        _Rect operator * (T2 scale) const
        {
            _Rect t(*this);
            t.leftTop *= scale;
            t.rightBottom *= scale;
            return t;
        }
        T GetWidth() const;
        T GetHeight() const;
        bool Visible() const;
    };
    template< class T >
    inline _Rect<T>::_Rect()
    {
    }
    template< class T >
    inline _Rect<T>::_Rect(T _left,T _right, T _top, T _bottom) :
    leftTop(_left, _top), rightBottom(_right, _bottom)
    {
    }
    template< class T >
    inline _Rect<T>::_Rect(const _Rect<T> &t) :
    leftTop(t.leftTop), rightBottom(t.rightBottom)
    {
    }
    template< class T >
    inline _Rect<T> & _Rect<T>::operator &= (const _Rect<T> &clip)
    {
        if (leftTop.x < clip.leftTop.x) leftTop.x = clip.leftTop.x;
        if (leftTop.y < clip.leftTop.y) leftTop.y = clip.leftTop.y;
        if (rightBottom.x > clip.rightBottom.x) rightBottom.x = clip.rightBottom.x;
        if (rightBottom.y > clip.rightBottom.y) rightBottom.y = clip.rightBottom.y;
        return *this;
    }
    template< class T >
    inline _Rect<T> _Rect<T>::operator & (const _Rect<T> &clip) const
    {
        _Rect<T> t(*this);
        t &= clip;
        return t;
    }
    template< class T >
    inline T _Rect<T>::distance(const _Point2<T> &point) const
    {
        _Point2<T> off;
        if (point.x < this->leftTop.x)
        {
            off.x = this->leftTop.x - point.x;
        }
        else if (point.x > this->rightBottom.x)
        {
            off.x = point.x - this->rightBottom.x;
        }
        if (point.y < this->leftTop.y)
        {
            off.y = this->leftTop.y - point.y;
        }
        else if (point.y > this->rightBottom.y)
        {
            off.y = point.y - this->rightBottom.y;
        }
        return off.Length();
    }
    template< class T >
    inline _Rect<T> & _Rect<T>::operator += (const _Point2<T> &point)
    {
        leftTop += point;
        rightBottom += point;
        return *this;
    }
    template< class T >
    inline _Rect<T> _Rect<T>::operator + (const _Point2<T> &point) const
    {
        _Rect<T> t(*this);
        t += point;
        return t;
    }
    template< class T >
    inline bool _Rect<T>::operator & (const _Point2<T> &point) const
    {
        return point.x >= leftTop.x && point.x < rightBottom.x && point.y >= leftTop.y && point.y < rightBottom.y;
    }

    template< class T >
    inline _Rect<T> _Rect<T>::operator + (const _Rect &o) const
    {
        _Rect<T> t(*this);
        t.leftTop += o.leftTop;
        t.rightBottom += o.rightBottom;
        return t;
    }

    template< class T >
    inline _Rect<T> _Rect<T>::operator - (const _Rect &o) const
    {
        _Rect<T> t(*this);
        t.leftTop -= o.leftTop;
        t.rightBottom -= o.rightBottom;
        return t;
    }

    template< class T >
    inline bool _Rect<T>::Visible() const
    {
        return leftTop.x < rightBottom.x && leftTop.y < rightBottom.y;
    }
    template< class T >
    inline bool _Rect<T>::operator == (const _Rect<T> &o) const
    {
        return leftTop.x  == o.leftTop.x  && rightBottom.x == o.rightBottom.x && leftTop.y == o.leftTop.y &&  rightBottom.y == o. rightBottom.y;
    }
    template< class T >
    inline T _Rect<T>::GetWidth() const
    {
        return rightBottom.x - leftTop.x;
    }
    template< class T >
    inline T _Rect<T>::GetHeight() const
    {
        return rightBottom.y - leftTop.y;
    }

    typedef _Rect<short> Rect;
    typedef _Rect<float> Rectf;

};

#endif//CURVEDANI_Rect_H