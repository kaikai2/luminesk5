#ifndef CANI_IMAGE_H
#define CANI_IMAGE_H

#include <cassert>
#include "curvedani.h"
#include "caRect.h"

namespace cAni
{

    /// Image ��ά��Ƭ������
    /// ���Image���Թ���ͬһ������ʹ��Rect�����ֳ��Լ��ڸ������еĲ��֡�
    class Image
    {
    public:
        Image::Image() : tex(0)
        {
        }
        Image::Image(iTexture *_tex) : tex(_tex)
        {
        }
        Image::Image(iTexture *_tex, const Rect &r) : tex(_tex), rect(r)
        {
        }
        bool valid() const
        {
            return tex != 0;
        }
        bool operator < (const Image &u2) const
        {
            return tex < u2.tex;
        }
        iTexture& getTexture() const
        {
            return *tex;
        }
        const Rect &getRect() const
        {
            return rect;
        }
        Rect &getRect()
        {
            return rect;
        }
    protected:
        iTexture *tex;
        Rect rect;
    };

}

#endif//CANI_