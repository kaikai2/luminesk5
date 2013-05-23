#ifndef __CLIP_H_
#define __CLIP_H_

#import <vector>

#import "face2d.h"
#import "point2d.h"
#import "point3d.h"

namespace ogl2d
{


/// 一个Clip包含了一个平面(Face2d)的位置(x,y)、大小(zx,zy)、旋转(angle)信息
/// 多个Clip公用一个平面(Face2d)的数据，好比是一个单图的精灵。
class Clip
{
public:
    Clip():m_p(), m_zoom(1.f, 1.f), m_rot(0.f), m_color(1.f,1.f,1.f), m_alpha(1.f), 
         m_layer(0), pFace(NULL), m_visible(true)
    {
    }
    Point2f m_p;    // Position
    Point2f m_zoom; // Zoom size
    float m_rot;    // Rotate Degree
    Point3f m_color;
    float m_alpha;
    bool m_visible;
    unsigned int m_layer; // 层号，数字越小越优先显示
    Face2d *pFace;
    friend bool operator < (const Clip &n1, const Clip &n2)
    {
        // 对于普通图片渲染，不必关心层数。让opengl的z-buffer去做检测
        return *n1.pFace < *n2.pFace;
    }
    void render(const Rect *cliprect = NULL) const;
};
/*
class ClipAlphaBlend : public Clip
{
public:
    friend int operator < (const ClipAlphaBlend &n1, const ClipAlphaBlend &n2)
    {
        // 但对于透明图片，必须从远处画到近处
        if (n1.m_layer < n2.m_layer)
            return true;
        if (n1.m_layer == n2.m_layer)
            return *n1.pFace < *n2.pFace;
        return false;
    }
};
*/

/// ClipPass是Clip的集合，用于统一对Clip进行渲染
class ClipPass
{
public:
    /// 从ClipPass中删除一个Clip
    void PopClip(Clip *clip);

    /// 添加一个Clip
    void PushClip(Clip *clip);

    /// 根据纹理号排列Clip
    void Sort();

    /// 渲染所有的Clip
    void Render(unsigned int &lastTextureId);
private:
    std::vector< Clip * > clips;
    int sorted;
};

};

#endif