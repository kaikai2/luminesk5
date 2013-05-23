
#ifndef GL_CURVEDANI_H
#define GL_CURVEDANI_H

#include <windows.h>
#include "curvedani.h"
#include "caImage.h"
#include "caPoint2d.h"
#include "caPoint3d.h"
#include "caRect.h"
#include "Wm4Matrix3.h"

/// gl版本且用一下stl,但在curvedani内当不该出现stl，以免非c++用户怪罪
#include <vector>
#include <map>
#include <string>

using cAni::StateId;
using cAni::MergeStyleId;
using cAni::InterpolateStyleId;
using cAni::iTexture;
using cAni::iCurveDataSet;
using cAni::iCurveInterpolater;
using cAni::iStateMergerIf;
using cAni::iStateMergerComposition;
using cAni::iClipState;
using cAni::iRenderer;
using cAni::iByteStream;
using cAni::iSystem;
using cAni::Image;
using cAni::Point2f;
using cAni::Point3f;
using cAni::Rect;
typedef unsigned long DWORD;

enum glCurvedAniStateId
{
    glSI_Image,    // size_t
    glSI_Position, // vector2
    glSI_Scale,    // vector2
    glSI_Angle,    // float
    glSI_Alpha,    // float
    glSI_Color,    // vector3
    glSI_AnchorOffPos, // vector2
	glSI_Text,     // string
    glSI_Clip,     // rect

    NumOfGLSI,
};

enum glCurvedAniMergeStyleId
{
	glMI_None,
    glMI_Gain,
    glMI_Reduce,
	glMI_SaturateColor, // saturate in [0,1]

    NumOfHGLMI,
};

enum glCurvedAniInterpolateStyleId
{
    glII_Step,
    glII_Linear,
    glII_Bspline,

    NumOfGlII,
};
/// 实现下列接口
/// 纹理提供者
class glTexture : public iTexture
{
public:
    glTexture() : tex(0)
    {
    }
    virtual ~glTexture();
    virtual size_t getWidth() const
    {
        assert(tex);
        return size_t(width);
    }
    virtual size_t getHeight() const
    {
        assert(tex);
        return size_t(height);
    }
    unsigned int getGlTexture()
    {
        if (!tex)
        {
            loadTexture(this->getFilename());
        }
        return tex;
    }
    void loadTexture(const char *filename);
protected:
    unsigned int tex;
    int width;
    int height;
};

// 数据集
template<class T>
class glCurveDataSet : public iCurveDataSet
{
public:
    virtual const char *getTypeName() const;
    virtual bool loadFromByteStream(iByteStream *bs)
    {
        return iCurveDataSet::loadFromByteStream(bs);
    }
    virtual bool saveToByteStream(iByteStream *bs) const
    {
        return iCurveDataSet::saveToByteStream(bs);
    }
    virtual size_t getDataCount() const
    {
        return data.size();
    }
    virtual void resize(size_t index)
    {
        data.resize(index);
    }
    virtual void setData(size_t index, const void *value, int time)
    {
        assert(index < data.size());
        data[index].data = *(T*)value;
        data[index].time = time;
    }
    virtual void setData(size_t index, const void *value)
    {
        assert(index < data.size());
        data[index].data = *(T*)value;
    }
    virtual void setTime(size_t index, int time)
    {
        assert(index < data.size());
        data[index].time = time;
    }
    virtual int getTime(size_t index) const
    {
        assert(index < data.size());
        return data[index].time;
    }
    virtual const void *getData(size_t index) const
    {
        assert(index < data.size());
        return &data[index].data;
    }
    virtual size_t getDataSize() const
    {
        return sizeof(T);
    }
    virtual void setDataString(size_t index, const char *valueStr);
    virtual const char *getDataString(size_t index) const;
    struct DataNode
    {
        int time;
        T data;
    };
	std::vector<DataNode> data;
};

/// 参数
class glClipState : public iClipState
{
public:
    glClipState() : image(), position(0, 0), scale(1, 1), angle(0), alpha(1), color(1, 1, 1), anchorOffPos(0, 0)
	{
		memset(mergeStyle, 0, sizeof(mergeStyle));
    }

    glClipState(const glClipState &rhs) : image(), position(0, 0), scale(1, 1), angle(0), alpha(1), color(1, 1, 1), anchorOffPos(0, 0)
    {
		reset(&rhs);
    }
    virtual void reset(const iClipState *defaultClipState = 0)
    {
        if (defaultClipState)
        {
            glClipState &rOther = *(glClipState *)defaultClipState;
            defaultClipState->getState(glSI_Image,    image);
            defaultClipState->getState(glSI_Position, position);
            defaultClipState->getState(glSI_Scale,    scale);
            defaultClipState->getState(glSI_Angle,    angle);
            defaultClipState->getState(glSI_Alpha,    alpha);
            defaultClipState->getState(glSI_Color,    color);
            defaultClipState->getState(glSI_AnchorOffPos, anchorOffPos);
            defaultClipState->getState(glSI_Text,     text);
            defaultClipState->getState(glSI_Clip,     clip);
            for (int i = 0; i < NumOfGLSI; i++)
            {
                setStateMergeStyle((StateId)i, rOther._getStateMergeStyle((StateId)i));
            }
        }
        else
        {
            image = Image();
            position = Point2f(0, 0);
            scale = Point2f(1, 1);
            angle = 0;
            alpha = 1;
            color = Point3f(1, 1, 1);
            anchorOffPos = Point2f(0, 0);
			text = "";
            clip = Rect();

            memset(mergeStyle, 0, sizeof(mergeStyle));
        }
    }

    virtual const void *getState(StateId sid) const
    {
        switch(sid)
        {
        case glSI_Image:    // size_t
            return &image;
        case glSI_Position: // vector2
            return &position;
        case glSI_Scale:    // vector2
            return &scale;
        case glSI_Angle:    // float
            return &angle;
        case glSI_Alpha:    // float
            return &alpha;
        case glSI_Color:    // vector3
            return &color;
        case glSI_AnchorOffPos: // vector2
            return &anchorOffPos;
        case glSI_Text: // string
            return &text;
        case glSI_Clip:// rect
            return &clip;
        default:
            assert(0);
            break;
        }
        return 0;
    }
    // merge style: 父子结点融合方式.
private:
    MergeStyleId _getStateMergeStyle(StateId sid) const
    {
        if (sid >= 0 && sid < NumOfGLSI)
        {
            return mergeStyle[sid];
        }
        return 0;
    }
public:
    virtual void setStateMergeStyle(StateId sid, MergeStyleId mergeStyle)
    {
        if (sid >= 0 && sid < NumOfGLSI)
        {
            this->mergeStyle[sid] = mergeStyle;
        }
    }

    virtual iClipState *clone() const
    {
        return new glClipState(*this);
    }
    virtual void release()
    {
        delete this;
    }

    virtual bool makeSample(StateId sid, iCurveInterpolater &rInterpolater, iCurveDataSet &rDataSet, int time)
    {
        void *pData = _getState(sid);
        return pData != 0 && rInterpolater.getSample(sid, rDataSet, time, pData);
    }
    
	void merge(StateId sid, iStateMergerComposition &rStateMergerComposition, const glClipState &rChild, const glClipState &rParent)
	{
        static bool bNeedMerge[] = 
        {
            false, //glSI_Image,    // size_t
            false, //glSI_Position, // vector2
            false, //glSI_Scale,    // vector2
            false, //glSI_Angle,    // float
            true, //glSI_Alpha,    // float
            true, //glSI_Color,    // vector3
            false, //glSI_AnchorOffPos, // vector2
            false, //glSI_Text,     // string
            false, //glSI_Clip,     // rect
            //NumOfGLSI,
        };
        if (sid < NumOfGLSI && bNeedMerge[sid])
        {
            rStateMergerComposition.setMergerStyle(sid, rChild._getStateMergeStyle(sid));
            rStateMergerComposition.merge(sid, rChild.getState(sid), rParent.getState(sid), this->_getState(sid));
        }
	}

    void getMatrix(Wm4::Matrix3f &mResult, MergeStyleId msAnchorOffPos, MergeStyleId msAngle, MergeStyleId msScale, MergeStyleId msPosition) const
    {
        mResult.MakeIdentity();

        if (msAnchorOffPos != 0)
        {
            Wm4::Matrix3f m33(
                1, 0, 0,
                0, 1, 0,
                -anchorOffPos.x, -anchorOffPos.y, 1);
            mResult = mResult * m33;
        }
        if (msAngle != 0)
        {
            Wm4::Matrix3f m33_rot;
            m33_rot.FromAxisAngle(Wm4::Vector3f(0, 0, 1), angle);
            mResult = mResult * m33_rot;
        }
        if (msScale != 0)
        {
            Wm4::Matrix3f mScale;
            mScale.MakeDiagonal(scale.x, scale.y, 1);
            mResult = mResult * mScale;
        }
        if (msPosition != 0)
        {
            //if (msAnchorOffPos != 0)
            //{
            //    Wm4::Matrix3f mPos(
            //        1, 0, 0,
            //        0, 1, 0,
            //        position.x + anchorOffPos.x, position.y + anchorOffPos.y, 1);
            //    mResult = mResult * mPos;
            //}
            //else
            {
                Wm4::Matrix3f mPos(
                    1, 0, 0,
                    0, 1, 0,
                    position.x, position.y, 1);
                mResult = mResult * mPos;
            }
        }
        else
        {
            //if (msAnchorOffPos != 0)
            //{
            //    Wm4::Matrix3f m33(
            //        1, 0, 0,
            //        0, 1, 0,
            //        anchorOffPos.x, anchorOffPos.y, 1);
            //    mResult = mResult * m33;
            //}
        }
    }
	static void updateMatrix(Wm4::Matrix3f &mResult, const glClipState &rChild, const glClipState &rParent)
	{
        Wm4::Matrix3f mChild;
        rChild.getMatrix(mChild, 1, 1, 1, 1);

        Wm4::Matrix3f mParent;
        rParent.getMatrix(mParent,
            rChild._getStateMergeStyle(glSI_AnchorOffPos),
            rChild._getStateMergeStyle(glSI_Angle),
            rChild._getStateMergeStyle(glSI_Scale),
            rChild._getStateMergeStyle(glSI_Position));

        mResult = mChild * mParent;
	}
	Image image;
    Point2f position;
    Point2f scale;
    float angle;
    float alpha;
    Point3f color;
    Point2f anchorOffPos;
    std::string text;
    Rect clip;
    MergeStyleId mergeStyle[NumOfGLSI];
};

class glStateMergerComposition : public iStateMergerComposition
{
public:
	glStateMergerComposition();
	virtual ~glStateMergerComposition();

	// 更换和设置融合方式
	virtual void setMergerStyle(cAni::StateId sid, cAni::MergeStyleId mid);
protected:
	// 容器类元素访问接口
	virtual iStateMergerIf *getMerger(cAni::StateId sid) const;
private:
	iStateMergerIf *mergers[NumOfGLSI];
};
namespace glport
{
	class FontMap;
}

class glRenderer : public iRenderer
{
public:
    glRenderer();
    virtual ~glRenderer();

    /// @fn getName
    /// @brief get renderer's unique name
    const char *getName() const
    {
        return "gl";
    }

    /// @fn getStateId
    /// @brief return id from name
    StateId getStateId(const char *stateName) const;
    /// @fn getStateName
    /// @brief return name from id
    const char *getStateName(StateId statusId) const;

    /// @fn getInterpolateStyleId
    /// @brief return id from name
    virtual InterpolateStyleId getInterpolateStyleId(const char *isName) const;

    /// @fn getInterpolateStyleName
    /// @brief return name from id
    virtual const char *getInterpolateStyleName(InterpolateStyleId isId) const;

    /// @fn getClipState
    /// @brief get a working instance
    virtual iClipState *getClipState()
    {
        return &cs;
    }
    /// @fn getInterpolator
    /// @brief get a working instance
    virtual iCurveInterpolater *getInterpolator(StateId stateId, InterpolateStyleId isId) const;

    virtual void render(const iClipState &rClipState, const iClipState &rParentClipState);
protected:
    const char *_getStateTypeName(StateId statusId) const;
    glClipState cs;
	glport::FontMap *fontmap;

    mutable std::map<std::pair<StateId, InterpolateStyleId>, iCurveInterpolater *> mapInterpolaters;
};
class glByteStream : public iByteStream
{
public:
    glByteStream() : data(0), size(0)
    {
    }
    virtual ~glByteStream()
    {
        if (data)
        {
//            gl->Resource_Free(data);
            size = 0;
        }
//        gl->Release();
    }
    virtual bool write(const void *buf, size_t len)
    {
        buf, len;
        assert(0 && "not allowed to write");
        return false;
    }
    virtual bool read(void *buf, size_t len)
    {
        if (len > size - cpos)
            return false;
        if (len > 0)
        {
            memcpy(buf, data + cpos, len);
            cpos += (DWORD)len;
        }
        return true;
    }
    /// seek to position
    virtual void seek(size_t pos)
    {
        cpos = min(DWORD(pos), size);
    }
    /// seek with an offset
    virtual void seekOffset(int offset)
    {
        seek(cpos + offset);
    }
    virtual size_t length() const
    {
        return size;
    }
    virtual size_t tell() const
    {
        return cpos;
    }
    void loadByteStream(const char *name);

protected:
    char *data;
    DWORD size;
    DWORD cpos;
};
class glCurvedAniSystem : public iSystem
{
public:
    glCurvedAniSystem();
    virtual ~glCurvedAniSystem()
    {
        dettach(&this->renderer);

        // MUST call this here
        iSystem::release();
        // gl->Release();
    }
    virtual iByteStream *loadStream(const char *name)
    {
        glByteStream *ByteStream = new glByteStream;
        ByteStream->loadByteStream(name);
        return ByteStream;
    }
    virtual iCurveDataSet *createCurveDataSet(const char *name);
    virtual iCurveInterpolater *createCurveInterpolater(const char *name);
    virtual iStateMergerIf *createStateMerger(const char *name);
    virtual void release(iByteStream *bs);
    virtual void release(iCurveDataSet *cds);
    virtual void release(iCurveInterpolater *ci);
    virtual void release(iStateMergerIf *sm);
    virtual iRenderer *getRenderer()
    {
        return &renderer;
    }
    virtual float getTime();

    /// @fn getPortName
    /// @brief return the name of the port
    virtual const char *getPortName() const
    {
        return "GLport";
    }
    static iSystem* GetInstance()
    {
        return s_Instance;
    }
protected:
    virtual iTexture *allocTexture();
    virtual void release(cAni::iTexture *tex);
    glRenderer renderer;

    LARGE_INTEGER m_PerformenceFreq;
};

#endif
