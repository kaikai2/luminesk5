/** @file curvedani.h
    @brief 接口定义文件
*/

#ifndef CANI_CURVED_ANI
#define CANI_CURVED_ANI

#include <cassert>
#include "caRect.h"

class TiXmlElement;

/// @namespace cAni
/// @brief namespace of CurvedAni
namespace cAni
{
    /// @defgroup groupInterface 需要实现的接口
    /// @defgroup groupInterface2 不要实现的接口

    /// implement the following interfaces in your renderer
    
    /// @interface iTexture
    /// @ingroup groupInterface
    /// @brief interface to identify a texture resource<br>
    /// 需要实现本接口
    class iTexture
    {
    public:
        iTexture();
        virtual ~iTexture();

        /// @fn getWidth
        /// @brief 获取纹理文件名，在实现本接口时需要使用
        /// @return
        virtual size_t getWidth() const = 0;

        /// @fn getHeight
        /// @brief 获取纹理文件名，在实现本接口时需要使用
        virtual size_t getHeight() const = 0;

        /// @fn getFilename
        /// @brief 获取纹理文件名，在实现本接口时需要使用
        const char *getFilename() const;
    private:
        friend class iSystem;
        void *internalData;
    };

    /// @interface iByteStream
    /// @ingroup groupInterface
    class iByteStream
    {
    public:
        template<typename T> bool write(const T& var)
        {
            return write(&var, sizeof(T));
        }
        template<typename T> bool writeArray(const T var[], size_t n)
        {
            return write(var, sizeof(T) * n);
        }
        template<typename T> bool read(T& var)
        {
            return read(&var, sizeof(T));
        }
        template<typename T> bool readArray(T var[], size_t n)
        {
            return read(var, sizeof(T) * n);
        }

        virtual bool write(const void *buf, size_t len) = 0;
        virtual bool read(void *buf, size_t len) = 0;
        /// seek to head
        void rewind()
        {
            seek(0);
        }
        /// seek to position
        virtual void seek(size_t pos) = 0;
        /// seek with an offset
        virtual void seekOffset(int offset) = 0;
        virtual size_t length() const = 0;
        virtual size_t tell() const = 0;
    };
    enum StateIds
    {
        InvalidStateId = size_t(-1),
    };
    typedef size_t StateId;

    enum InterpolateStyleIds
    {
        InvalidInterpolateStyleId = size_t(-1),
    };
    typedef size_t InterpolateStyleId;

    enum MergeStyleIds
    {
        InvalidMergeStyleId = size_t(-1),
    };
    typedef size_t MergeStyleId;

    /// @interface iCurveDataSet
    /// @ingroup groupInterface
    /// @brief 数据集
    class iCurveDataSet
    {
    public:
        iCurveDataSet();
        virtual ~iCurveDataSet();
        /// @fn getDataSetName
        /// @brief unique data set name
        const char *getDataSetName() const;
        /// @fn setDataSetName
        /// @brief unique data set name
        void setDataSetName(const char *name);

        /// @fn getTypeName
        /// @brief unique type name, 
        virtual const char *getTypeName() const = 0;

        virtual size_t getDataCount() const = 0;
        virtual void resize(size_t index) = 0;
        virtual size_t getDataSize() const = 0;

        virtual void setTime(size_t index, int time) = 0;
        virtual void setData(size_t index, const void *value, int time) = 0;
        virtual void setData(size_t index, const void *value) = 0;
        virtual void setDataString(size_t index, const char *valueStr) = 0;

        virtual int getTime(size_t index) const = 0;
        virtual const void *getData(size_t index) const = 0;
        virtual const char *getDataString(size_t index) const = 0;

        virtual bool loadFromByteStream(iByteStream *bs);
        virtual bool saveToByteStream(iByteStream *bs) const;
        bool loadFromXml(const TiXmlElement *elem);
        bool saveToXml(TiXmlElement *elem) const;
    private:
        void *internalData;
    };
    /// @interface iCurveInterpolater
    /// @ingroup groupInterface
    /// 插值器
    class iCurveInterpolater
    {
    public:
		virtual ~iCurveInterpolater(){}
        /// @fn getInterpolateName
        /// @brief unique interpolate name
        virtual const char *getInterpolateName() const = 0;

        virtual bool getSample(StateId sid, const iCurveDataSet &cds, int time, void *result) const = 0;
    };

	class iStateMergerIf
	{
	public:
		virtual ~iStateMergerIf(){}
        virtual void merge(StateId sid, const void *child, const void *parent, void *result) const = 0;
	};
    /// @interface iStateMerger
    /// @ingroup groupInterface
    /// 融合器
	class iStateMerger : public iStateMergerIf
    {
    public:
		virtual ~iStateMerger(){}
        /// @fn getMergerName
        /// @brief unique merger name
        virtual const char *getMergerName() const = 0;

        /// @fn parseParam
        /// @brief 获得脚本内参数
        virtual void parseParam(const char *){}
    };
	/// 组合模式，容器提供类的接口，需要根据类型提供
	class iStateMergerComposition : public iStateMergerIf
	{
	public:
		virtual ~iStateMergerComposition(){}
		virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
		{
			if (iStateMergerIf *sm = getMerger(sid))
			{
				sm->merge(sid, child, parent, result);
			}
		}
		// 更换和设置融合方式
		virtual void setMergerStyle(StateId sid, MergeStyleId mid) = 0;
	protected:
		// 容器类元素访问接口
		virtual iStateMergerIf *getMerger(StateId sid) const = 0;
	};

    /// @interface iClipState
    /// @ingroup groupInterface
    /// 参数
    class iClipState
    {
    public:
        //virtual StateId StringToStateId(const char *name) = 0;
        //virtual void *getStateAddr(StateId sid) = 0;
        //virtual void setState(StateId sid, const void *value) = 0;
        virtual const void *getState(StateId sid) const = 0;

        template<typename T>
        void getState(StateId sid, T &value) const
        {
            value = *(T*)getState(sid);
        }

        // merge style: 父子结点融合方式.
        virtual void setStateMergeStyle(StateId sid, MergeStyleId mergeStyle) = 0;

        // 按原型初始化
        virtual void reset(const iClipState *prototype = 0) = 0;

        virtual iClipState *clone() const = 0;
        virtual void release() = 0;

        virtual bool makeSample(StateId sid, iCurveInterpolater &rInterpolater, iCurveDataSet &rDataSet, int time) = 0;

    protected:
        void *_getState(StateId sid);
    };
    /// @interface iRenderer
    /// @ingroup groupInterface
    class iRenderer
    {
    public:
        /// @fn getName
        /// @brief get renderer's unique name
        virtual const char *getName() const = 0;

        /// @fn getStateId
        /// @brief return id from name
        virtual StateId getStateId(const char *stateName) const = 0;

        /// @fn getStateName
        /// @brief return name from id
        virtual const char *getStateName(StateId stateId) const = 0;

        /// @fn getInterpolateStyleId
        /// @brief return id from name
        virtual InterpolateStyleId getInterpolateStyleId(const char *isName) const = 0;

        /// @fn getInterpolateStyleName
        /// @brief return name from id
        virtual const char *getInterpolateStyleName(InterpolateStyleId isId) const = 0;

        /// @fn getClipState
        /// @brief get a working instance
        virtual iClipState *getClipState() = 0;

        /// @fn getInterpolator
        /// @brief get a working instance
        virtual iCurveInterpolater *getInterpolator(StateId stateId, InterpolateStyleId isId) const = 0;

        /// @fn render
        /// @brief render the giving clip
        virtual void render(const iClipState &rClipState, const iClipState &rParentClipState) = 0;
    };

    /// @class class
    class AnimData;
    /// @interface iAnimation
    /// @ingroup groupInterface2
    class iAnimation
    {
    public:
        typedef size_t AnimId;
        enum
        {
            DefaultAnimId = ~0,
        };
        virtual void setAnimData(const AnimData *pAnimData, AnimId aniId) = 0;
        virtual bool startAnim(int curtime, AnimId aniId = DefaultAnimId) = 0;
        virtual bool isPlaying() const = 0;
        virtual bool isLoop() const = 0;
        virtual bool checkEnd(int curtime) const = 0;
        virtual void setAnimLoop(bool bLoop) = 0;
        virtual AnimId getCurAnim() const = 0;
        // void setAnimTimer(int time);

        /// render to default renderer
        /// @param time     frame time
        /// @param defaultClipState
        virtual void render(int frameTime, const iClipState *defaultClipState = 0) const = 0;
    };
    /// @interface iAnimResManager
    /// @ingroup groupInterface2
    class iAnimResManager
    {
    public:
        virtual const AnimData* getAnimData(const char *aniFileName) = 0;
        virtual bool release(const char *aniFileName) = 0;
        virtual bool saveToByteStream(const AnimData *pAnimData, iByteStream *bs) const = 0;
        virtual bool saveToXml(const AnimData *pAnimData, const char *filename) const = 0;
    };
    /// @interface iSystem
    /// @ingroup groupInterface
    class iSystem
    {
    public:
        iSystem()
        {
            assert(s_Instance == 0);
            s_Instance = this;
        }
        virtual ~iSystem()
        {
            assert(s_Instance == this);
            s_Instance = 0;
        }
        iTexture *getTexture(const char *name);
        virtual iByteStream *loadStream(const char *name) = 0;
        virtual iCurveDataSet *createCurveDataSet(const char *name) = 0;
        virtual iCurveInterpolater *createCurveInterpolater(const char *name) = 0;
        virtual iStateMergerIf *createStateMerger(const char *name) = 0;
        iAnimation *createAnimation(size_t animCount = 1);
        iAnimResManager *createAnimResManager();
        
        virtual void release(iByteStream *bs) = 0;
        virtual void release(iCurveDataSet *cds) = 0;
        virtual void release(iCurveInterpolater *ci) = 0;
        virtual void release(iStateMergerIf *sm) = 0;
        void release(iAnimation *a);
        void release(iAnimResManager *arm);

        /// @fn getRenderer
        /// @brief get the specific renderer
        /// @param name identification of the renderer
        iRenderer *getRenderer(const char *name) const;
        virtual float getTime() = 0;

        /// @fn getPortName
        /// @brief return the name of the port
        virtual const char *getPortName() const = 0;
        static iSystem* GetInstance()
        {
            return s_Instance;
        }

        /// @fn attach
        /// @brief attach a renderer, and then it could be get through getRenderer
        void attach(iRenderer *r);
        /// @fn dettach
        /// @brief dettach a renderer
        void dettach(iRenderer *r);
    protected:
        void release();
        virtual void release(iTexture *bs) = 0;
        virtual iTexture *allocTexture() = 0;
        static iSystem *s_Instance;
    };
}

#endif // CANI_CURVED_ANI