#ifndef __CurvedAni_Locus_H_
#define __CurvedAni_Locus_H_

#include "curvedani.h"

namespace cAni
{
    /// Locus
    /// @note   �켣��һ������ʱ��-״̬�ĺ���
    struct Locus
    {
    public:
        struct StreamData
        {
            unsigned short dsid;
            unsigned short ipid;
            unsigned short sid;
            unsigned short mergeStyle;
        };
        iCurveDataSet *dataset;
        //iCurveInterpolater *interpolator;
        InterpolateStyleId interpolateStyle; // ���ݲ�ֵ��ʽ
        MergeStyleId mergeStyle; // ���ӽ���ںϷ�ʽ
        StateId sid;
    };
} // namespace cAni

#endif // __CurvedAni_Locus_H_
