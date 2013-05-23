#ifndef CANI_IMAGE_H
#define CANI_IMAGE_H

#include <cassert>
#include "curvedani.h"
#include "caRect.h"

namespace cAni
{

    /// Image 二维面片，矩形
    /// 多个Image可以公用同一个纹理，使用Rect来区分出自己在该纹理中的部分。
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