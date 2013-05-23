#ifndef ___Rect_H_
#define ___Rect_H_

namespace ogl2d
{
template< class T >
struct _Rect
{
    _Rect();
    _Rect(T _left,T _right, T _top, T _bottom);
    _Rect(const _Rect &t);
    T left, right, top, bottom;
    _Rect & operator &= (const _Rect &clip);
    _Rect operator & (const _Rect &clip);
	_Rect & operator =(const _Rect &clip);
    T GetWidth() const;
    T GetHeight() const;
    bool Visible();
};
template< class T >
inline _Rect<T>::_Rect():left(0),right(0),top(0),bottom(0)
{
}
template< class T >
inline _Rect<T>::_Rect(T _left,T _right, T _top, T _bottom):
    left(_left),right(_right),top(_top),bottom(_bottom)
{
}
template< class T >
inline _Rect<T>::_Rect(const _Rect<T> &t):left(t.left),right(t.right),top(t.top),bottom(t.bottom)
{
}
template< class T >
inline _Rect<T> & _Rect<T>::operator &= (const _Rect<T> &clip)
{
    if (left<clip.left) left = clip.left;
    if (top<clip.top) top = clip.top;
    if (right>clip.right) right = clip.right;
    if (bottom>clip.bottom) bottom = clip.bottom;
    return *this;
}
	template< class T >
inline _Rect<T> & _Rect<T>::operator = (const _Rect<T> &clip)
{
	left = clip.left;
	top = clip.top;
	right = clip.right;
	bottom = clip.bottom;
	return *this;
}
template< class T >
inline _Rect<T> _Rect<T>::operator & (const _Rect<T> &clip)
{
    _Rect<T> t(*this);
    t &= clip;
    return t;
}
template< class T >
inline bool _Rect<T>::Visible()
{
    return left<right && top<bottom;
}
template< class T >
inline T _Rect<T>::GetWidth() const
{
    return right - left;
}
template< class T >
inline T _Rect<T>::GetHeight() const
{
    return bottom - top;
}

typedef _Rect<short> Rect;
typedef _Rect<float> Rectf;

};

#endif