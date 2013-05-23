#pragma warning(disable: 4786)
// zhu #import <windows.h>
#import <OpenGLES/ES1/gl.h>
//#import <gl/glaux.h>
#import <algorithm>
#import <cassert>
#import "texture.h"
#import "basemath.h"
#import "pic.h"

namespace ogl2d
{

Texture::Texture(TextureResource &res, const unsigned char *buf, unsigned short _width, unsigned short _height, unsigned short channel):
    resource(res), refCount(0), scalex(1.f/_width), scaley(1.f/_height), width(_width), height(_height)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture::~Texture()
{
    assert(refCount == 0);
    glDeleteTextures(1, &id);
}

void Texture::GetRef()
{
    if (refCount++ == 0)
    {
        // ActiveListNode::Remove(); // remove from unused link
    }
}

void Texture::ReleaseRef()
{
    if (--refCount == 0)
    {
        resource.ReleaseTexture(this); // the texture may be deleted! 
                                       // so never use it after this line
    }
}

void Texture::DrawRect(const Rect &rect) const
{
    float u = (rect.right-rect.left) * .5f, v = (rect.bottom-rect.top) *.5f;
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GetId());

    float u1,v1,u2,v2;
    u1 = rect.left * scalex;
    v1 = rect.top * scaley;
    u2 = rect.right * scalex;
    v2 = rect.bottom * scaley;
    /*glBegin(GL_QUADS);
    glTexCoord2f(u1, v1); glVertex2f(-u, -v);
    glTexCoord2f(u1, v2); glVertex2f(-u,  v);
    glTexCoord2f(u2, v2); glVertex2f( u,  v);
    glTexCoord2f(u2, v1); glVertex2f( u, -v);
    glEnd();*/

	
	GLfloat pfVertices[] = {-u, -v,  -u,  v,   u,  v,   u, -v};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,pfVertices);
	GLfloat pfTexCoord[] = {u1,v1,  u1, v2,   u2, v2,   u2, v1};
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,0,pfTexCoord);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


}

void Texture::DrawRect(const Rect &rect, const Rect &clipper) const
{
    float u = (rect.right-rect.left) * .5f, v = (rect.bottom-rect.top) *.5f;

    float left = max(-u, clipper.left);
    float right = min(u, clipper.right);
    float top = max(-v, clipper.top);
    float bottom = min(v, clipper.bottom);

    float u1,v1,u2,v2;
    if (left >= right || top >= bottom)
        return;
    u1 = (rect.left + left + u) * scalex;
    v1 = (rect.top + top + v)* scaley;
    u2 = (rect.right + right - u)* scalex;
    v2 = (rect.bottom + bottom - v)* scaley;
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GetId());
    /*glBegin(GL_QUADS);
    glTexCoord2f(u1, v1); glVertex2f(left, top);
    glTexCoord2f(u1, v2); glVertex2f(left, bottom);
    glTexCoord2f(u2, v2); glVertex2f(right, bottom);
    glTexCoord2f(u2, v1); glVertex2f(right, top);
    //glEnd();*/
	
	GLfloat pfVertices[] = {left,top, left,bottom, right,bottom,   right,top};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2,GL_FLOAT,0,pfVertices);
	GLfloat pfTexCoord[] = {u1,v1,  u1, v2,   u2, v2,   u2, v1};
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,GL_FLOAT,0,pfTexCoord);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	

}

TextureRgb::TextureRgb(TextureResource &res, const unsigned char *buf, unsigned short _width, unsigned short _height)
    :Texture(res, buf, _width, _height, 3)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 
        0/*No border*/, GL_RGB, GL_UNSIGNED_BYTE, buf);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST
}

TextureRgba::TextureRgba(TextureResource &res, const unsigned char *buf, unsigned short _width, unsigned short _height)
    :Texture(res, buf, _width, _height, 4)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA , width, height, 
        0/*No border*/, GL_RGBA, GL_UNSIGNED_BYTE, buf);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST

}

TextureResource::TextureResource()
{
}

TextureResource::~TextureResource()
{
    Release();
}

/// 释放一个纹理
void TextureResource::ReleaseTexture(Texture *tex)
{
    // 放到不使用的纹理中，等待空闲时间或内存不足时释放
    // unusedTexs.Insert(tex);
    for (std::vector< Pair >::iterator iter = texs.begin(); iter != texs.end(); ++iter)
    {
        if (tex == iter->pTex)
        {
            delete tex;
            iter = texs.erase(iter);
            return;
        }
    }
    assert(0 && "invalid Texture pointer or it is not generated by this TextureResource");
}

/// TextureResource::Release
/// @note   释放所有纹理
void TextureResource::Release()
{
    for (std::vector< Pair >::iterator iter = texs.begin(); iter != texs.end(); ++iter)
    {
        delete iter->pTex;
    }
    texs.clear();
}


/// @note    从图片文件创建新纹理
TexturePtr TextureResource::GetTexture(const char *szfilename)
{
    std::vector< Pair >::const_iterator tex = ::std::find(texs.begin(), texs.end(), szfilename);
    if (tex != texs.end())
    {
        return TexturePtr(tex->pTex);
    }

    Ogl2dPicType type = image::Pic::PictureType(szfilename);
    if (type == OGL2D_PIC_UNKNOWN)
        return TexturePtr();
    image::Pic pic;
    pic.Load(szfilename, type);

    if (pic.width == 0 || pic.height == 0 || IS_NOT_POWER_OF_2(pic.width) || IS_NOT_POWER_OF_2(pic.height)) // not valid
        return TexturePtr();

    return NewTexture(pic.buf, pic.width, pic.height, pic.nchannels, szfilename);
}


/// TextureResource::NewTexture
/// @note   从内存块创建新纹理
/// @return Texture *   纹理对象
/// @param  const unsigned char *buf    纹理数据块，容量为size*size
/// @param  unsigned short size         纹理的边长，必须是2的幂
TexturePtr TextureResource::NewTexture(const unsigned char *buf, unsigned short _width, unsigned short _height, unsigned short channel, const char *szFileName)
{
    Texture *pTex = NULL;
    switch(channel)
    {
   /* case 1:
        pTex = new TextureAlpha(*this, buf, _width, _height);
        break;*/
    case 3:
        pTex = new TextureRgb(*this, buf, _width, _height);
        break;
    case 4:
        pTex = new TextureRgba(*this, buf, _width, _height);
        break;
    default:
        assert(0);
        break;
    }
    assert(pTex != NULL);
    Pair node;
    node.pTex = pTex;
    if (szFileName != NULL)
    {
        node.filename = szFileName;
        node.isFileTexture = true;
    }
    else
        node.isFileTexture = false;
    texs.push_back(node);
    // unusedTexs.Insert(pTex);

    return TexturePtr(pTex);
}

/// 取得纹理的文件名，如果纹理不是来自文件，则返回空串。如果纹理不存在或出错，返回NULL
const char* TextureResource::GetFileName(const TexturePtr &texture) const
{
    std::vector< Pair >::const_iterator tex = std::find(texs.begin(), texs.end(), texture);
    if (tex == texs.end())
        return NULL;
    
    if (!tex->isFileTexture)
        return "";

    return tex->filename.c_str();
}

/// TextureResource::GetTexture
/// @note   获得纹理
/// @return Texture *   纹理对象
/// @param  unsinged int id 纹理id(OpenGL纹理id)
// Texture *TextureResource::GetTexture(unsigned int id)
// {return *std::find_if(texs.begin(), texs.end(), FindTextureById(id));}

} // namespace opd2d