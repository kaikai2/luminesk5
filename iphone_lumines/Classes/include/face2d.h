#ifndef __FACE_2D_H_
#define __FACE_2D_H_

#import "rect.h"
#import "texture.h"
#import "renderstate.h"

namespace ogl2d
{

/// Face2d 二维面片，矩形，自裁减
/// 多个Face2d可以公用同一个纹理，使用Rect来区分出自己在该纹理中的部分。
class Face2d
{
public:
    Face2d();
    virtual ~Face2d();
    virtual bool operator < (const Face2d &u2) const = 0;
    virtual void Draw(int x, int y, const Rect *clipRect = NULL) = 0;
    virtual int getTextureCount() const = 0;
    virtual TexturePtr getTexture(int index = 0) const = 0;
    virtual const Rect &getRect(int index = 0) const = 0;
protected:
    friend class ClipPass;

    unsigned int refCount; // reference count
    // for the heap
    unsigned int heapPos;  //the position of self in the heap
};

// 普通的face
class Face2dRgb : public Face2d
{
public:
    Face2dRgb(TexturePtr &_tex, Rect *_rect = NULL);
    virtual ~Face2dRgb();
    virtual bool operator < (const Face2d &u2) const
    {
        return tex->GetId() < ((Face2dRgb*)(&u2))->tex->GetId();
    }
    virtual int getTextureCount() const
    {
        return 1;
    }
    virtual TexturePtr getTexture(int index = 0) const
    {
        return index, tex;
    }
    virtual const Rect &getRect(int index = 0) const
    {
        return index, rect;
    }
protected:
    friend class ClipPass;
    virtual void Draw(int x, int y, const Rect *clipRect = NULL);

    TexturePtr tex; // ref to texture
    Rect rect; // rect range of the texture (Images share one texture)
};

// 带有alpha混合的face
typedef Face2dRgb Face2dRgba; // they are just the same

// 带有蒙版的alpha混合的face
class Face2dRgbaMask : public Face2dRgb
{
public:
    typedef Face2dRgb Parent;
    Face2dRgbaMask(TexturePtr &_tex, TexturePtr &_texmask, Rect *_rect = NULL, Rect *_rectmask = NULL);
    virtual ~Face2dRgbaMask();
    virtual bool operator < (const Face2d &u2) const
    {
        return tex->GetId() < ((Face2dRgbaMask*)(&u2))->tex->GetId() || 
               tex->GetId() ==   ((Face2dRgbaMask*)(&u2))->tex->GetId() && 
               texmask->GetId() <((Face2dRgbaMask*)(&u2))->texmask->GetId();
    }
    virtual int getTextureCount() const
    {
        return 2;
    }
    virtual TexturePtr getTexture(int index = 0) const
    {
        if (index == 0) return Parent::getTexture(index);
        return texmask;
    }
    virtual const Rect &getRect(int index = 0) const
    {
        if (index == 0) return Parent::getRect(index);
        return rectmask;
    }
protected:
    friend class ClipPass;
    void Draw(int x, int y, const Rect *clipRect = NULL);
    TexturePtr texmask; // ref to texture
    Rect rectmask;
};
};

#endif