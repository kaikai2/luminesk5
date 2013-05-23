/*
常用融合器
也可以自己创建新的融合器

*/

#ifndef CANI_MERGERS_H
#define CANI_MERGERS_H

#include "curvedani.h"
#include "caImage.h"
#include "caBspline.h"

#pragma warning(push)
#pragma warning(disable:4554)
namespace cAni
{
    // 融合器
	/// 保持融合器
	template<class T>
	class KeepMerger : public iStateMerger
	{
	public:
		virtual const char *getMergerName() const
		{
			return "Keep";
		}
		virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
		{
			sid, parent;

			*(T*)result = *(const T*)child;
		}
	};

    /// 加法融合器
    template<class T>
    class GainMerger : public iStateMerger
    {
    public:
        virtual const char *getMergerName() const
        {
            return "Gain";
        }
        virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
        {
            sid;

            *(T*)result = *(const T*)child + *(const T*)parent;
        }
    };
    /// 减法融合器
    template<class T>
    class ReduceMerger : public iStateMerger
    {
    public:
        virtual const char *getMergerName() const
        {
            return "Reduce";
        }
        virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
        {
            sid;

            *(T*)result = *(const T*)child - *(const T*)parent;
        }
    };

    /// 饱和融合器
    template<class T, template<typename>class Parser>
    class SaturateMerger : public iStateMerger
    {
    public:
        virtual const char *getMergerName() const
        {
            return "Saturate";
        }
        /// @fn parseParam
        /// @brief 获得脚本内参数
        virtual void parseParam(const char *t)
        {
            Parser<T> parser;
            minValue = parser.parseToken(t, ",");
            maxValue = parser.parseNext();
        }
        virtual void merge(StateId sid, const void *child, const void *parent, void *result) const
        {
            sid;

            T &rRes = *(T*)result;
            rRes = *(const T*)child + *(const T*)parent;
            if (rRes > maxValue)
            {
                rRes = maxValue;
            }
            else if (rRes < minValue)
            {
                rRes = minValue;
            }
        }
    private:
        T minValue, maxValue;
    };
};
#pragma warning(pop)

#endif//CANI_MERGERS_H