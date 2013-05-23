#pragma warning(disable:4786)
#import <algorithm>
// zhu #import <windows.h>
#import <OpenGLES/ES1/gl.h>

#import "clip.h"


namespace ogl2d
{

void Clip::render(const Rect *cliprect) const
{
    if (!m_visible)
        return;

    glPushMatrix();
    glTranslatef(m_p.x, m_p.y, (float)m_layer);
    glRotatef(m_rot, 0,0,1);
    glScalef(m_zoom.x, m_zoom.y,0);
    glColor4f(m_color.x, m_color.y, m_color.z, m_alpha);
    pFace->Draw(0,0);
    glPopMatrix();
}

/// 从ClipPass中删除一个Clip
void ClipPass::PopClip(Clip *clip)
{
    std::vector< Clip * >::iterator iter = std::find(clips.begin(), clips.end(), clip);
    if (iter != clips.end())
    {
        clips.erase(iter);
    }
}

/// 添加一个Clip
void ClipPass::PushClip(Clip *clip)
{
    clips.push_back(clip);
    sorted = 0;
}

void ClipPass::Sort()
{
    if (sorted)
        return;
    std::sort(clips.begin(), clips.end());
    sorted = 1;
}

void ClipPass::Render(unsigned int &lastTextureId)
{
    Sort();
    std::vector< Clip* >::iterator iter = clips.begin();

    Clip* clip;
   // lastTextureId = -1;
    while (iter != clips.end())
    {
        clip = *iter++;
        clip->render();
    }
}



};