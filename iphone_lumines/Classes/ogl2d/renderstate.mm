#import "renderstate.h"
namespace ogl2d
{

namespace render
{

static RenderState s_CurRenderState;

void SetRenderState(RenderState &rs)
{
    if (rs.bDepthTest != s_CurRenderState.bDepthTest)
    {
        if (rs.bDepthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        s_CurRenderState.bDepthTest = rs.bDepthTest;
    }
    if (rs.bBlend != s_CurRenderState.bBlend)
    {
        if (rs.bBlend)
        {
            glEnable(GL_BLEND);
            if (rs.blend_sfactor != s_CurRenderState.blend_sfactor ||
                rs.blend_dfactor != s_CurRenderState.blend_dfactor)
            {
                glBlendFunc(rs.blend_sfactor, rs.blend_dfactor);
                s_CurRenderState.blend_sfactor = rs.blend_sfactor;
                s_CurRenderState.blend_dfactor = rs.blend_dfactor;
            }
        }
        else
            glDisable(GL_BLEND);
        s_CurRenderState.bBlend = rs.bBlend;
    }
    else
    {
        if (rs.bBlend)
        {
            if (rs.blend_sfactor != s_CurRenderState.blend_sfactor ||
                rs.blend_dfactor != s_CurRenderState.blend_dfactor)
            {
                glBlendFunc(rs.blend_sfactor, rs.blend_dfactor);
                s_CurRenderState.blend_sfactor = rs.blend_sfactor;
                s_CurRenderState.blend_dfactor = rs.blend_dfactor;
            }
        }
    
    }
}

void SetRenderState(bool bDepthTest, bool bBlend, GLenum sf, GLenum df)
{
    if (bDepthTest != s_CurRenderState.bDepthTest)
    {
        if (bDepthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        s_CurRenderState.bDepthTest = bDepthTest;
    }
    if (bBlend != s_CurRenderState.bBlend)
    {
        if (bBlend)
        {
            glEnable(GL_BLEND);
            if (sf != s_CurRenderState.blend_sfactor ||
                df != s_CurRenderState.blend_dfactor)
            {
                glBlendFunc(sf, df);
                s_CurRenderState.blend_sfactor = sf;
                s_CurRenderState.blend_dfactor = df;
            }
        }
        else
            glDisable(GL_BLEND);
        s_CurRenderState.bBlend = bBlend;
    }
    else
    {
        if (bBlend)
        {
            if (sf != s_CurRenderState.blend_sfactor ||
                df != s_CurRenderState.blend_dfactor)
            {
                glBlendFunc(sf, df);
                s_CurRenderState.blend_sfactor = sf;
                s_CurRenderState.blend_dfactor = df;
            }
        }
    }
}

}; // ogl2d

}; // ogl2d