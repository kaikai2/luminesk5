/*
�����ں���
Ҳ�����Լ������µ��ں���

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
    // �ں���
	/// �����ں���
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

    /// �ӷ��ں���
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
    /// �����ں���
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

    /// �����ں���
    template<class T, template<typename>class Parser>
    class SaturateMerger : public iStateMerger
    {
    public:
        virtual const char *getMergerName() const
        {
            return "Saturate";
        }
        /// @fn parseParam
        /// @brief ��ýű��ڲ���
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