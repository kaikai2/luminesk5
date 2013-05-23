// zhu #import <windows.h>
#import <OpenGLES/ES1/gl.h>

#import "face2d.h"

namespace ogl2d
{

Face2d::Face2d()
{
}

Face2d::~Face2d()
{
}

Face2dRgb::Face2dRgb(TexturePtr &_tex, Rect *_rect):tex(_tex)
{
    if (_rect)
    {
        rect = *_rect;
    }
    else
    {
        rect.left = 0;
        rect.right = _tex->GetWidth();
        rect.top = 0;
        rect.bottom = _tex->GetHeight();
    }
    //tex->GetRef();
}

Face2dRgb::~Face2dRgb()
{
    //tex->ReleaseRef();
}

void Face2dRgb::Draw(int x, int y, const Rect *clipRect)
{
    Rect clip(rect);
    if (!clip.Visible())
    {
        return;
    }

    render::SetRenderState(false, true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    tex->DrawRect(clip);
}

Face2dRgbaMask::Face2dRgbaMask(TexturePtr &_tex, TexturePtr &_texmask, Rect *_rect, Rect *_rectmask)
:   Face2dRgb(_tex, _rect), texmask(_texmask)
{
    if (_rectmask)
    {
        rectmask = *_rectmask;
    }
    else
    {
        rectmask.left = 0;
        rectmask.right = _texmask->GetWidth();
        rectmask.top = 0;
        rectmask.bottom = _texmask->GetHeight();
    }
    //texmask->GetRef();
}

Face2dRgbaMask::~Face2dRgbaMask()
{
    //texmask->ReleaseRef();
}

void Face2dRgbaMask::Draw(int x, int y, const Rect *clipRect)
{
    Rect clip(rect);
    Rect clipmask(rectmask);
    if (!clip.Visible())
    {
        return;
    }
    render::SetRenderState(false, true, GL_DST_COLOR, GL_ZERO);
    texmask->DrawRect(rectmask);
    render::SetRenderState(false, true, GL_ONE, GL_ONE);
    tex->DrawRect(clip);
}

};
