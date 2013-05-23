#ifndef _TEXTURE_H
#define _TEXTURE_H

//// zhu #import <windows.h>
#import <OpenGLES/ES1/gl.h>
#import <vector>
#import <string>
#import "activelist.h"
#import "rect.h"

namespace ogl2d
{

class TextureResource;
class TexturePtr;
class Texture : public AbstractBase::ActiveListNode
{
public:
    GLuint GetId() const;
    void TexCoord(short u, short v) const;
    unsigned short GetWidth() const;
    unsigned short GetHeight() const;
    virtual unsigned short GetChannel()const = 0 ;

    /// DrawRect
    /// @note   ѡ��rect��Χ�ڵ�����һ�����Σ��������Ķ���
    /// @param  const Rect &rect ���򣬵�λ��������
    void DrawRect(const Rect &rect) const;

    /// DrawRect
    /// @note   ѡ��rect��Χ�ڵ�����һ�����Σ��������Ķ��룬���ü�clipper֮��Ĳ���
    /// @param  const Rect &rect ���򣬵�λ��������
    /// @param  cosnt Rect &clipper �ü�����
    void DrawRect(const Rect &rect, const Rect &clipper) const;

protected:
    friend class TexturePtr;
    void GetRef();
    void ReleaseRef();

    friend class TextureResource;
    TextureResource &resource;

    Texture(TextureResource &res, const unsigned char *buf, unsigned short _width, unsigned short _height, unsigned short channel);
    virtual ~Texture();
    GLuint id; // gl texture id
    unsigned int refCount; // reference count
    const float scalex; // 1/width
    const float scaley; // 1/height
    const unsigned short width; // texture size, buf will be size*size
    const unsigned short height; // texture size, buf will be size*size
};

class TextureRgb : public Texture
{
public:
    unsigned short GetChannel()const{return 3;}

private:
    friend class TextureResource;
    TextureRgb(TextureResource &res, const unsigned char *buf, unsigned short _width, unsigned short _height);
};
class TextureRgba : public Texture
{
public:
    unsigned short GetChannel()const{return 4;}

private:
    friend class TextureResource;
    TextureRgba(TextureResource &res, const unsigned char *buf, unsigned short _width, unsigned short _height);
};

class TexturePtr
{
public:
    TexturePtr():ptr(NULL)          {}
    TexturePtr(const TexturePtr &o):ptr(o.ptr){if (ptr) ptr->GetRef();}
    ~TexturePtr(){if (ptr) ptr->ReleaseRef();}
    TexturePtr &operator =(const TexturePtr &o)
    {
        if (ptr) ptr->ReleaseRef();
        ptr = o.ptr;
        if (ptr) ptr->GetRef();
        return *this;
    }
    operator Texture *()             {return ptr;}
    operator const Texture *() const {return ptr;}
    Texture* operator ->()           {return ptr;}
    const Texture* operator ->()const{return ptr;}
private:
    friend class TextureResource;
    TexturePtr(Texture *_ptr):ptr(_ptr){if (ptr) ptr->GetRef();}

    Texture *ptr;
};

class TextureResource
{
public:
    TextureResource();
    virtual ~TextureResource();

    /// �ͷ���������
    void Release();

    /// ��ͼƬ�ļ�����������
    TexturePtr GetTexture(const char *szfilename);

    /// ���ڴ滺��鴴��������
    TexturePtr NewTexture(const unsigned char *buf, unsigned short width, unsigned short height, unsigned short channel);

    /// ȡ��������ļ�������������������ļ����򷵻ؿմ�������������ڻ��������NULL
    const char* GetFileName(const TexturePtr &texture) const;

    /// ͨ��id�������
    // TexturePtr GetTexture(unsigned int id);
    /// ------------------------------
private:
    TexturePtr NewTexture(const unsigned char *buf, unsigned short width, unsigned short height, unsigned short channel, const char *szFileName);
    struct FindTextureById
    {
        FindTextureById(GLuint _id):id(_id){}
        bool operator () (Texture *a)
        {
            return a->GetId() == id;
        }
        unsigned int id;
    };

    friend class Texture;
    void ReleaseTexture(Texture *tex);
    struct Pair
    {
        Texture* pTex;
        bool isFileTexture; // create from file ?
        ::std::string filename;
        bool operator == (const char* name)const
        {
            return isFileTexture && filename == name;
        }
        bool operator == (const TexturePtr &tex)const
        {
            return (const Texture*)tex == pTex;
        }
    };
    std::vector< Pair > texs;
    // AbstractBase::ActiveList unusedTexs;
};


inline GLuint Texture::GetId() const
{
    return id;
}

inline unsigned short Texture::GetWidth() const
{
    return width;
}

inline unsigned short Texture::GetHeight() const
{
    return height;
}

inline void Texture::TexCoord(short u, short v) const
{
 //   glTexCoord2f(u*scalex, v*scaley);
}

inline TexturePtr TextureResource::NewTexture(const unsigned char *buf, unsigned short width, unsigned short height, unsigned short channel)
{
    return NewTexture(buf, width, height, channel, NULL);
}

} // namespace opd2d

#endif //#ifndef _TEXTURE_H