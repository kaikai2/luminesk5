#ifndef __CLIP_H_
#define __CLIP_H_

#import <vector>

#import "face2d.h"
#import "point2d.h"
#import "point3d.h"

namespace ogl2d
{


/// һ��Clip������һ��ƽ��(Face2d)��λ��(x,y)����С(zx,zy)����ת(angle)��Ϣ
/// ���Clip����һ��ƽ��(Face2d)�����ݣ��ñ���һ����ͼ�ľ��顣
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
    unsigned int m_layer; // ��ţ�����ԽСԽ������ʾ
    Face2d *pFace;
    friend bool operator < (const Clip &n1, const Clip &n2)
    {
        // ������ͨͼƬ��Ⱦ�����ع��Ĳ�������opengl��z-bufferȥ�����
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
        // ������͸��ͼƬ�������Զ����������
        if (n1.m_layer < n2.m_layer)
            return true;
        if (n1.m_layer == n2.m_layer)
            return *n1.pFace < *n2.pFace;
        return false;
    }
};
*/

/// ClipPass��Clip�ļ��ϣ�����ͳһ��Clip������Ⱦ
class ClipPass
{
public:
    /// ��ClipPass��ɾ��һ��Clip
    void PopClip(Clip *clip);

    /// ���һ��Clip
    void PushClip(Clip *clip);

    /// �������������Clip
    void Sort();

    /// ��Ⱦ���е�Clip
    void Render(unsigned int &lastTextureId);
private:
    std::vector< Clip * > clips;
    int sorted;
};

};

#endif