#ifndef __RENDER_STATE_H_
#define __RENDER_STATE_H_

//// zhu #import <windows.h>
#import <OpenGLES/ES1/gl.h>

namespace ogl2d
{

namespace render
{

struct RenderState
{
    bool bDepthTest;
    bool bBlend;

    GLenum  blend_sfactor, blend_dfactor;
};

void SetRenderState(RenderState &rs);
void SetRenderState(bool bDepthTest, bool bBlend = false, GLenum sf = GL_ONE, GLenum df = GL_ZERO);

}; // ogl2d

}; // ogl2d
#endif // __RENDER_STATE_H_