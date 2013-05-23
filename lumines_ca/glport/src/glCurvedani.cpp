#include <cstdio>
#include <sstream>
#include <windows.h>
#include <gl/gl.h>
#include "glCurvedani.h"
#include "caInterpolaters.h"
#include "caMergers.h"
#include "font.h"

#include "sdl/sdl_image.h"
#include "sdl/sdl_rwops.h"
#include "sdl/sdl_video.h"
//#include "sdl/sdl_ttf.h"

using cAni::StepInterpolater;
using cAni::LinearInterpolater;
using cAni::BsplineInterpolater;
using cAni::KeepMerger;
using cAni::GainMerger;
using cAni::ReduceMerger;
using cAni::SaturateMerger;

#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL.lib")
//#pragma comment(lib, "SDL_ttf.lib")

#define IS_NOT_POWER_OF_2(a) ((a)-1 & (a))
#define IS_POWER_OF_2(a) (!IS_NOT_POWER_OF_2(a))

glTexture::~glTexture()
{
    if (tex)
    {
        glDeleteTextures(1, &tex);
    }
}
void glTexture::loadTexture(const char *filename)
{
    if (!tex)
    {
        glGenTextures(1, &tex);
    }
    if (tex)
    {
        SDL_RWops *sdl_rwops = SDL_RWFromFile(filename, "rb");
        if (!sdl_rwops)
            return;
         
        SDL_Surface *sdl_surface = NULL;
        bool valid_type = true;
        //* these are...
#define SEEK SDL_RWseek(sdl_rwops, 0, SEEK_SET)
        /* something like the below if..else if..else cases
        if (SEEK, IMG_isPNG(sdl_rwops))
        {
            SEEK;
            sdl_surface = IMG_LoadPNG_RW(sdl_rwops);
        }
        else
            valid_type = false;
        */
#define TRY_LOAD_IMG(surface, op, type) \
    (SEEK, IMG_is##type(op)) && (SEEK, 0 != (surface = IMG_Load##type##_RW(op)))

        TRY_LOAD_IMG(sdl_surface, sdl_rwops, PNG) ||
        //(SEEK, Utilities::ogl2dIMG_isTGA(sdl_rwops)) && (SEEK, sdl_surface = Utilities::ogl2dIMG_LoadTGA_RW(sdl_rwops)) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, BMP) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, PCX) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, GIF) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, JPG) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, TIF) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, LBM) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, PNM) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, XPM) ||
        TRY_LOAD_IMG(sdl_surface, sdl_rwops, XCF) ||
        0 == (valid_type = false);

        SDL_RWclose(sdl_rwops);

        if (!valid_type || sdl_surface == NULL || sdl_surface->pixels == NULL)
            return;

        if (sdl_surface->w == 0 || sdl_surface->h == 0 || 
            sdl_surface->pitch != sdl_surface->format->BytesPerPixel * sdl_surface->w ||        
            IS_NOT_POWER_OF_2(sdl_surface->w) || IS_NOT_POWER_OF_2(sdl_surface->h)) // not valid
        {
            assert(0 && "glport only support textures with 2-power-sized pictures");
            SDL_FreeSurface(sdl_surface);
            return;
        }

        int nchannels = sdl_surface->format->BytesPerPixel;

        glBindTexture(GL_TEXTURE_2D, tex);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        switch(nchannels)
        {
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width = sdl_surface->w, height = sdl_surface->h, 
                0/*No border*/, GL_RGB, GL_UNSIGNED_BYTE, sdl_surface->pixels);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width = sdl_surface->w, height = sdl_surface->h, 
                0/*No border*/, GL_RGBA, GL_UNSIGNED_BYTE, sdl_surface->pixels);
            break;
        }

        SDL_FreeSurface(sdl_surface);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST
    }
}

iTexture *glCurvedAniSystem::allocTexture()
{
    return new glTexture;
}

void glCurvedAniSystem::release(iTexture *tex)
{
    delete tex;
}

glCurvedAniSystem::glCurvedAniSystem()
{
    if (!QueryPerformanceFrequency(&m_PerformenceFreq))
    {
        assert(0);
    }

    attach(&this->renderer);
}
float glCurvedAniSystem::getTime()
{
    LARGE_INTEGER counter;
    if (QueryPerformanceCounter(&counter))
    {
        return float((double)counter.QuadPart / (double)m_PerformenceFreq.QuadPart);
    }
    return GetTickCount() * 0.001f;
}

glStateMergerComposition::glStateMergerComposition()
{
	memset(mergers, 0, sizeof(mergers));
}

glStateMergerComposition::~glStateMergerComposition()
{
	for (size_t i = 0; i < NumOfGLSI; i++)
	{
		iSystem::GetInstance()->release(mergers[i]);
	}
}
// 更换和设置融合方式
void glStateMergerComposition::setMergerStyle(cAni::StateId sid, cAni::MergeStyleId mid)
{
	if (mergers[sid])
	{
		iSystem::GetInstance()->release(mergers[sid]);
		mergers[sid] = 0;
	}

	std::string typeName;
	switch(sid)
	{
	case glSI_Image:    // size_t
		typeName = "Image";
		break;
	case glSI_Position: // vector2
	case glSI_Scale:    // vector2
	case glSI_AnchorOffPos: // vector2
		typeName = "Float2";
		break;
	case glSI_Angle:    // float
	case glSI_Alpha:    // float
		typeName = "Float";
		break;
	case glSI_Color:    // vector3
		typeName = "Float3";
		break;
	case glSI_Clip: // Rect(short4)
		typeName = "Rect";
		break;
	//case hgeSI_Transform: // Wm4::Matrix3
	//	typeName = "Matrix3";
	//	break;
	default:
		assert(0);
		break;
	}
	std::string mergeStyleName;
	switch(mid)
	{
	case glMI_None:
		mergeStyleName = "Keep";
		break;
	case glMI_Gain:
		mergeStyleName = "Gain";
		break;
	case glMI_Reduce:
		mergeStyleName = "Reduce";
		break;
	case glMI_SaturateColor: // saturate in [0,1]
		mergeStyleName = "Saturate";
		break;
	default:
		assert(0);
		mergeStyleName = "Keep";
		break;
	}

	if (mergeStyleName.length() && typeName.length())
	{
		std::string stateMergerName = mergeStyleName + typeName;
		mergers[sid] = iSystem::GetInstance()->createStateMerger(stateMergerName.c_str());
	}
}

// 容器类元素访问接口
iStateMergerIf *glStateMergerComposition::getMerger(cAni::StateId sid) const
{
	return mergers[sid];
}

//	iStateMergerIf *mergers[NumOfHgeSI];

template<typename key, typename value>
class BindMap
{
public:
    struct Info
    {
        key k;
        value v;
    };
    BindMap(Info *begin, Info *end, const key &ik, const value &iv)
        : invalidKey(ik), invalidValue(iv)
    {
        for (Info *i = begin; i != end; i++)
        {
            a2b[i->k] = i->v;
            b2a[i->v] = i->k;
        }
    }
    const value &find(const key &k) const
    {
        std::map<key, value>::const_iterator i = a2b.find(k);
        if (i == a2b.end())
        {
            return invalidValue;
        }
        return i->second;
    }
    const key &find(const value &v) const
    {
        std::map<value, key>::const_iterator i = b2a.find(v);
        if (i == b2a.end())
        {
            return invalidKey;
        }
        return i->second;
    }
private:
    BindMap &operator = (const BindMap &rhs)
    {
        rhs;
        return *this;
    }
    const key invalidKey;
    const value invalidValue;
    std::map<key, value> a2b;
    std::map<value, key> b2a;
};

// no way to use this
template<typename key>
class BindMap<key, key>
{
private:
    BindMap(){}
    ~BindMap(){}
};

typedef BindMap<cAni::StateId, std::string> BindMapState2Name;
static BindMapState2Name::Info s_StateInfos[] = 
{
    {glSI_Image,           "Image" },    // size_t
    {glSI_Position,        "Position" }, // vector2
    {glSI_Scale,           "Scale" },    // vector2
    {glSI_Angle,           "Angle" },    // float
    {glSI_Alpha,           "Alpha" },    // float
    {glSI_Color,           "Color" },    // vector3
    {glSI_AnchorOffPos,    "AnchorOffPos" }, // vector2
    {glSI_Text,            "Text" },     // string
    {glSI_Clip,             "Clip"},     // rect
};
BindMapState2Name g_state2Name(s_StateInfos, s_StateInfos + sizeof(s_StateInfos) / sizeof(s_StateInfos[0]), cAni::StateId(cAni::InvalidStateId), "");

cAni::StateId glRenderer::getStateId(const char *stateName) const
{
    return g_state2Name.find(stateName);
}

const char *glRenderer::getStateName(cAni::StateId stateId) const
{
    return g_state2Name.find(stateId).c_str();
}
const char *glRenderer::_getStateTypeName(StateId statusId) const
{
    switch(statusId)
    {
	case glSI_Image:    // size_t
        return "Image";
	case glSI_Position: // vector2
        return "Float2";
	case glSI_Scale:    // vector2
        return "Float2";
	case glSI_Angle:    // float
        return "Float";
	case glSI_Alpha:    // float
        return "Float";
	case glSI_Color:    // vector3
        return "Float3";
	case glSI_AnchorOffPos: // vector2
        return "Float2";
	case glSI_Text:     // string
		return "String";
	case glSI_Clip:     // rect
        return "Rect";
    }
    return "";
}
typedef BindMap<cAni::InterpolateStyleId, std::string> BindMapInterpolateStyle2Name;
static BindMapInterpolateStyle2Name::Info s_InterpolateStyleInfos[] = 
{
    {glII_Step,        "Step" },
    {glII_Linear,      "Linear" },
    {glII_Bspline,     "Bspline" },
};
BindMapInterpolateStyle2Name g_InterpolateStyle2Name(s_InterpolateStyleInfos, s_InterpolateStyleInfos + sizeof(s_InterpolateStyleInfos) / sizeof(s_InterpolateStyleInfos[0]), cAni::InterpolateStyleId(cAni::InvalidInterpolateStyleId), "");

/// @fn getInterpolateStyleId
/// @brief return id from name
InterpolateStyleId glRenderer::getInterpolateStyleId(const char *isName) const
{
    return g_InterpolateStyle2Name.find(isName);
}

/// @fn getInterpolateStyleName
/// @brief return name from id
const char *glRenderer::getInterpolateStyleName(InterpolateStyleId isId) const
{
    return g_InterpolateStyle2Name.find(isId).c_str();
}

/// @fn getInterpolator
/// @brief get a working instance
iCurveInterpolater *glRenderer::getInterpolator(StateId stateId, InterpolateStyleId isId) const
{
    if (mapInterpolaters[std::make_pair(stateId, isId)] == 0)
    {
        std::stringstream ss;
        ss << this->getInterpolateStyleName(isId) << this->_getStateTypeName(stateId);
        mapInterpolaters[std::make_pair(stateId, isId)] = iSystem::GetInstance()->createCurveInterpolater(ss.str().c_str());
    }
    return mapInterpolaters[std::make_pair(stateId, isId)];
}



glRenderer::glRenderer()
{
	this->fontmap = new glport::FontMap(wglGetCurrentDC());
	fontmap->GetFont("Tahoma.ttf");
}

glRenderer::~glRenderer()
{
	delete this->fontmap;
	this->fontmap = 0;
}
void glRenderer::render(const iClipState &rClipState, const iClipState &rParentClipState)
{
	glStateMergerComposition hsc;

	glClipState result;
	result.reset(&rClipState);
	for (int i = 0; i < NumOfGLSI; i++)
	{
		result.merge((cAni::StateId)i, hsc, (const glClipState &)rClipState, (const glClipState &)rParentClipState);
	}

    float alpha = result.alpha;
    const Point3f &color = result.color;
    const Rect &clip = result.clip;

    if (clip.GetWidth() && clip.GetHeight())
    {
        glColorMask(0,0,0,0);								// Set Color Mask
        glEnable(GL_STENCIL_TEST);							// Enable Stencil Buffer For "marking" The Floor
        glStencilFunc(GL_ALWAYS, 1, 1);						// Always Passes, 1 Bit Plane, 1 As Mask
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);			// We Set The Stencil Buffer To 1 Where We Draw Any Polygon
        // Keep If Test Fails, Keep If Test Passes But Buffer Test Fails
        // Replace If Test Passes
        glDisable(GL_DEPTH_TEST);							// Disable Depth Testing
        //DrawFloor();										// Draw The Floor (Draws To The Stencil Buffer)
        glBegin(GL_QUADS);
        glVertex3f(result.position.x + clip.leftTop.x, result.position.y + clip.leftTop.y, 0);
        glVertex3f(result.position.x + clip.rightBottom.x, result.position.y + clip.leftTop.y, 0);
        glVertex3f(result.position.x + clip.rightBottom.x, result.position.y + clip.rightBottom.y, 0);
        glVertex3f(result.position.x + clip.leftTop.x, result.position.y + clip.rightBottom.y, 0);
        glEnd();
        // We Only Want To Mark It In The Stencil Buffer
        //glEnable(GL_DEPTH_TEST);							// Enable Depth Testing
        glColorMask(1,1,1,1);								// Set Color Mask to TRUE, TRUE, TRUE, TRUE
        glStencilFunc(GL_EQUAL, 1, 1);						// We Draw Only Where The Stencil Is 1
        // (I.E. Where The Floor Was Drawn)
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);				// Don't Change The Stencil Buffer
    }
	glColor4f(color.x, color.y, color.z, alpha);

    const Image &image = result.image;
	if (image.valid())
	{
		Wm4::Matrix3f m33;
	    glClipState::updateMatrix(m33, (const glClipState&)rClipState, (const glClipState&)rParentClipState);
		m33 = m33.Transpose();
		glTexture &glTex = *(glTexture*)&image.getTexture();
		unsigned int tex = glTex.getGlTexture();
		glBindTexture(GL_TEXTURE_2D, tex);
	    const Rect &texRect = image.getRect();
		const float w_2 = texRect.GetWidth() * 0.5f;
		const float h_2 = texRect.GetHeight() * 0.5f;
		Wm4::Vector3f wmPosition0 = m33 * Wm4::Vector3f(-w_2, -h_2, 1.f);
		Wm4::Vector3f wmPosition1 = m33 * Wm4::Vector3f(w_2, -h_2, 1.f);
		Wm4::Vector3f wmPosition2 = m33 * Wm4::Vector3f(w_2, h_2, 1.f);
		Wm4::Vector3f wmPosition3 = m33 * Wm4::Vector3f(-w_2, h_2, 1.f);

        float left = (float)texRect.leftTop.x / glTex.getWidth();
		float right = (float)texRect.rightBottom.x / glTex.getWidth();
		float top = (float)texRect.leftTop.y / glTex.getHeight();
		float bottom = (float)texRect.rightBottom.y / glTex.getHeight();

        glEnable(GL_TEXTURE_2D);    
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		
		glTexCoord2f(left, top);
		glVertex3f(wmPosition0.X(), wmPosition0.Y(), 0.5f);// x-,y-
		glTexCoord2f(right, top);
		glVertex3f(wmPosition1.X(), wmPosition1.Y(), 0.5f);// x+,y-
		glTexCoord2f(right, bottom);
		glVertex3f(wmPosition2.X(), wmPosition2.Y(), 0.5f);// x+,y+
		glTexCoord2f(left, bottom);
		glVertex3f(wmPosition3.X(), wmPosition3.Y(), 0.5f);// x-,y+
		glEnd();
	}
    const std::string &text = result.text;
	if (text.length())
	{
		glport::Font *pFont = fontmap->GetFont("Impact");
		if (pFont)
		{
			glPushMatrix();
			glTranslatef(result.position.x, result.position.y, 0);
			glScalef(result.scale.x, -result.scale.y, 1); // reverse y direciton
            glTranslatef(-result.anchorOffPos.x, -result.anchorOffPos.y, 0);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
			glLineWidth(0.3f);
			pFont->textOut(text.c_str());
			glPopMatrix();
		}
	}

    glDisable(GL_STENCIL_TEST);
}

// 对image的读写进行特化
bool glCurveDataSet<Image>::loadFromByteStream(iByteStream *bs)
{
    assert(sizeof(Image) == getDataSize());

    unsigned short size;
    if (!bs->read(size))
        return false;
    resize(size);
    
    int len;
    char name[MAX_PATH];
    Rect rect;

    for (size_t i = 0; i < size; i++)
    {
        int time;
        if (!bs->read(time) || !bs->read(len) || len >= MAX_PATH || !bs->read(name, len) || !bs->read(rect))
        {
            resize(0);
            return false;
        }

        name[len] = 0;
        Image image(iSystem::GetInstance()->getTexture(name), rect);
        setData(i, &image, time);
    }
    return true;
}

void glByteStream::loadByteStream(const char *name)
{
    if (data)
    {
        delete [] data;
        data = 0;
        size = 0;
    }
    FILE *fp = fopen(name, "rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        data = new char[size];
        fread(data, size, 1, fp);
        fclose(fp);
    }
    cpos = 0;
}
bool glCurveDataSet<Image>::saveToByteStream(iByteStream *bs) const
{
    assert(sizeof(Image) == getDataSize());

    unsigned short size = (unsigned short)getDataCount();
    if (!bs->write(size))
        return false;


    for (size_t i = 0; i < size; i++)
    {
        int time = getTime(i);
        const Image &image = *(const Image *)getData(i);
        const char *filename = image.getTexture().getFilename();
        int len = (int)strlen(filename);
        if (len >= MAX_PATH || !bs->write(time) || !bs->write(len) || !bs->write(filename, len) || !bs->write(image.getRect()))
            return false;
    }
    return true;
}


const char *glCurveDataSet<Image>::getTypeName() const
{
    return "Image";
}
void glCurveDataSet<Image>::setDataString(size_t index, const char *valueStr)
{
    char *buf = _strdup(valueStr);
    char *pos = strchr(buf, ';');
    assert(pos && *pos == ';');
    *pos = 0;
    Image image(iSystem::GetInstance()->getTexture(buf));
    int temp[4];
    if (sscanf(pos + 1, "%d,%d,%d,%d", temp, temp + 1, temp + 2, temp + 3) != 4)
    {
        assert(0);
    }
    Rect &rect = image.getRect();
    rect.leftTop = cAni::Point2s((short)temp[0], (short)temp[1]);
    rect.rightBottom = cAni::Point2s((short)temp[2], (short)temp[3]);
    free(buf);
    this->setData(index, &image);
}
const char *glCurveDataSet<Image>::getDataString(size_t index) const
{
    static char buf[MAX_PATH + 10 * 4 + 5];
    assert(index < data.size());
    const Image &image = *(const Image *)getData(index);
    const char *texFilename = image.getTexture().getFilename();
    if (!texFilename)
        return 0;
    const Rect &rect = image.getRect();
    sprintf(buf, "%s;%d,%d,%d,%d", texFilename, rect.leftTop.x, rect.leftTop.y, rect.rightBottom.x, rect.rightBottom.y);
    return buf;
}

const char *glCurveDataSet<float>::getTypeName() const
{
    return "Float";
}
void glCurveDataSet<float>::setDataString(size_t index, const char *valueStr)
{
    float f;
    sscanf(valueStr, "%f", &f);
    this->setData(index, &f);
}
const char *glCurveDataSet<float>::getDataString(size_t index) const
{
    static char buf[10];
    float f = *(const float *)getData(index);
    sprintf(buf, "%f", f);
    return buf;
}

const char *glCurveDataSet<Point2f>::getTypeName() const
{
    return "Float2";
}
void glCurveDataSet<Point2f>::setDataString(size_t index, const char *valueStr)
{
    Point2f v;
    sscanf(valueStr, "%f,%f", &v.x, &v.y);
    this->setData(index, &v);
}
const char *glCurveDataSet<Point2f>::getDataString(size_t index) const
{
    static char buf[10];
    const Point2f &v = *(const Point2f *)getData(index);
    sprintf(buf, "%f,%f", v.x, v.y);
    return buf;
}

const char *glCurveDataSet<Point3f>::getTypeName() const
{
    return "Float3";
}
void glCurveDataSet<Point3f>::setDataString(size_t index, const char *valueStr)
{
    Point3f v;
    sscanf(valueStr, "%f,%f,%f", &v.x, &v.y, &v.z);
    this->setData(index, &v);
}
const char *glCurveDataSet<Point3f>::getDataString(size_t index) const
{
    static char buf[10];
    const Point3f &v = *(const Point3f *)getData(index);
    sprintf(buf, "%f,%f,%f", v.x, v.y, v.z);
    return buf;
}

const char *glCurveDataSet<Rect>::getTypeName() const
{
    return "Rect";
}
void glCurveDataSet<Rect>::setDataString(size_t index, const char *valueStr)
{
    Rect v;
    sscanf(valueStr, "%d,%d,%d,%d", &v.leftTop.x, &v.leftTop.y, &v.rightBottom.x, &v.rightBottom.y);
    this->setData(index, &v);
}
const char *glCurveDataSet<Rect>::getDataString(size_t index) const
{
    static char buf[10];
    const Rect &v = *(const Rect *)getData(index);
	sprintf(buf, "%d,%d,%d,%d", v.leftTop.x, v.leftTop.y, v.rightBottom.x, v.rightBottom.y);
    return buf;
}

iCurveDataSet *glCurvedAniSystem::createCurveDataSet(const char *name)
{
    if (0 == strcmp("Image", name))
    {
        return new glCurveDataSet<Image>;
    }
    else if (0 == strcmp("Float", name))
    {
        return new glCurveDataSet<float>;
    }
    else if (0 == strcmp("Float2", name))
    {
        return new glCurveDataSet<Point2f>;
    }
    else if (0 == strcmp("Float3", name))
    {
        return new glCurveDataSet<Point3f>;
    }
    else if (0 == strcmp("Rect", name))
    {
        return new glCurveDataSet<Rect>;
    }
    /*
    else if (0 == strcmp("CDS_gl_Int", name))
    {
    return new glCurveDataSet<int>;
    }
    */
    assert(0 && "unsupported curve data set.");
    return 0;
}

template<template <typename> class T >
inline iCurveInterpolater *createInterpolator_all(const char *name)
{
	if (0 == strcmp("Float", name))
    {
        return new T<float>;
    }
    else if (0 == strcmp("Float2", name))
    {
        return new T<Point2f>;
    }
    else if (0 == strcmp("Float3", name))
    {
        return new T<Point3f>;
    }
	return 0;
}

template<template <typename> class T >
inline iCurveInterpolater *createInterpolator(const char *name)
{
	return createInterpolator_all<T>(name);
}

template<>
inline iCurveInterpolater *createInterpolator<StepInterpolater>(const char *name)
{
    if (0 == strcmp("Image", name)) // Image只支持StepInterpolater
    {
        return new StepInterpolater<Image>;
    }
	return createInterpolator_all<StepInterpolater>(name);
}

iCurveInterpolater *glCurvedAniSystem::createCurveInterpolater(const char *name)
{
	if (0 == strncmp("Step", name, 4))
	{
		return createInterpolator<StepInterpolater>(name + 4);
	}
	else if (0 == strncmp("Linear", name, 6))
	{
		return createInterpolator<LinearInterpolater>(name + 6);
	}
	else if (0 == strncmp("Bspline", name, 7))
	{
		return createInterpolator<BsplineInterpolater>(name + 7);
	}
    assert(0 && "unsupported curve interpolater.");
	return 0;
}

template<template <typename> class T >
class MergerFactory
{
public:
    static iStateMergerIf *createMerger(const char *name)
    {
        if (0 == strcmp("Float", name))
        {
            return new T<float>;
        }
        else if (0 == strcmp("Float2", name))
        {
            return new T<Point2f>;
        }
        else if (0 == strcmp("Float3", name))
        {
            return new T<Point3f>;
        }
		else if (0 == strcmp("Matrix3", name))
		{
			return new T<Wm4::Matrix3f>;
		}
        /// and you could add any data types you want here
        //else if if (0 == strncmp("MyDataType", name, strlen("MyDataType")))
        //{
        //  return new T<MyDataType>;
        //}
        return 0;
    }
};

template<typename T>
class ParserNull
{
public:
    T parseToken(const char *str, const char *delim)
    {
		str, delim;
        return T();
    }
    T parseNext()
    {
        return T();
    }
    const char *next;
};

static iStateMergerIf *createSaturateMerger(const char *name)
{
    if (0 == strcmp("Float", name))
    {
        return new SaturateMerger<float, ParserNull>;
    }
    /// and you could add any data types you want here
    //else if if (0 == strncmp("MyDataType", name, strlen("MyDataType")))
    //{
    //  return new T<MyDataType>;
    //}
    return 0;
}


iStateMergerIf *glCurvedAniSystem::createStateMerger(const char *name)
{
	if (0 == strncmp("Keep", name, 4))
	{
		return MergerFactory<KeepMerger>::createMerger(name + 4);
	}
    else if (0 == strncmp("Gain", name, 4))
    {
        return MergerFactory<GainMerger>::createMerger(name + 4);
    }
    else if (0 == strncmp("Reduce", name, 6))
    {
        return MergerFactory<ReduceMerger>::createMerger(name + 6);
    }
    else if (0 == strncmp("Saturate", name, 8))
    {
        return createSaturateMerger(name + 8);
    }

    /// and you could add any interpolaters you want here
    //else if if (0 == strncmp("Modulate", name, 3))
    //{
    //  return createMerger<ModulateMerger>(name + 3);
    //}
    assert(0 && "unsupported state merger.");
    return 0;
}
void glCurvedAniSystem::release(iByteStream *bs)
{
    delete bs;
}
void glCurvedAniSystem::release(iCurveDataSet *cds)
{
    delete cds;
}
void glCurvedAniSystem::release(iCurveInterpolater *ci)
{
    delete ci;
}
void glCurvedAniSystem::release(iStateMergerIf *sm)
{
    delete sm;
}
