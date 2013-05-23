#ifndef __CurvedAni_Locus_H_
#define __CurvedAni_Locus_H_

#include "curvedani.h"

namespace cAni
{
    /// Locus
    /// @note   轨迹，一个描述时间-状态的函数
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
        InterpolateStyleId interpolateStyle; // 数据差值方式
        MergeStyleId mergeStyle; // 父子结点融合方式
        StateId sid;
    };
} // namespace cAni

#endif // __CurvedAni_Locus_H_
