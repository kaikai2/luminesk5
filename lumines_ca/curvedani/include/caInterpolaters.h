/*
常用插值器
可以自己创建新的插值器，比如sin曲线函数插值器,curvedataset内保存曲线参数，建议仍然使用时间-状态组合
此时，可表示随着时间的变化，sin曲线的参数也在变化。如模拟阻尼振动。

*/

#ifndef CANI_INTERPOLATERS_H
#define CANI_INTERPOLATERS_H

#include "curvedani.h"
#include "caImage.h"
#include "caBspline.h"

#pragma warning(push)
#pragma warning(disable:4554)
namespace cAni
{
    // 插值器
    /// 跃变型插值器
    template<class T>
    class StepInterpolater : public iCurveInterpolater
    {
    public:
        virtual const char *getInterpolateName() const
        {
            return "Step";
        }
        virtual bool getSample(StateId sid, const iCurveDataSet &cds, int time, void *result) const
        {
            return getSample(sid, cds, time, *(T*)result);
        }
        bool getSample(StateId sid, const iCurveDataSet &cds, int time, T &result) const
        {
            sid;

            size_t lo = 0, hi = cds.getDataCount();
            if (hi <= 0)
                return false;

            if (time <= cds.getTime(0))
            {
                result = *(T*)cds.getData(0);
                return true;
            }
            if (time >= cds.getTime(hi - 1))
            {
                result = *(T*)cds.getData(hi - 1);
                return true;
            }
            while(lo + 1 < hi)
            {
                size_t id = lo + hi >> 1;
                if (cds.getTime(id) <= time)
                    lo = id;
                else
                    hi = id;
            }
            assert(lo + 1 == hi);
            result = *(T*)cds.getData(lo);
            return true;
        }
    };
    /// 线性插值器
    template<class T>
    class LinearInterpolater : public iCurveInterpolater
    {
    public:
        virtual const char *getInterpolateName() const
        {
            return "Linear";
        }
        virtual bool getSample(StateId sid, const iCurveDataSet &cds, int time, void *result) const
        {
            return getSample(sid, cds, time, *(T*)result);
        }
        bool getSample(StateId sid, const iCurveDataSet &cds, int time, T &result) const
        {
            sid;

            size_t lo = 0, hi = cds.getDataCount();
            if (hi <= 0)
                return false;

            if (time <= cds.getTime(0))
            {
                result = *(T*)cds.getData(0);
                return true;
            }
            if (time >= cds.getTime(hi - 1))
            {
                result = *(T*)cds.getData(hi - 1);
                return true;
            }
            assert(hi >= 2);
            while(lo + 1 < hi)
            {
                size_t id = lo + hi >> 1;
                if (cds.getTime(id) <= time)
                    lo = id;
                else
                    hi = id;
            }
            assert(lo + 1 == hi && lo + 1 != cds.getDataCount());
            const T &t1 = *(const T*)cds.getData(lo);
            const T &t2 = *(const T*)cds.getData(lo + 1);
            float d = float(cds.getTime(lo + 1) - cds.getTime(lo));
            d = (time - cds.getTime(lo)) / d;
            result = t1 + (t2 - t1) * d;
            return true;
        }
    };
    /// B样条曲线差值器
    template<class T>
    class BsplineInterpolater : public iCurveInterpolater
    {
    public:
        virtual const char *getInterpolateName() const
        {
            return "Bspline";
        }
        virtual bool getSample(StateId sid, const iCurveDataSet &cds, int time, void *result) const
        {
            return getSample(sid, cds, time, *(T*)result);
        }
        bool getSample(StateId sid, const iCurveDataSet &cds, int time, T &result) const
        {
            sid;

            size_t lo = 0, hi = cds.getDataCount();
            if (hi <= 3)
                return false;

            int time1 = cds.getTime(1);
            int time2 = cds.getTime(hi - 2);
            if (time <= time1)
            {
                lo = 1;
                time = time1;
            }
            else if (time >= time2)
            {
                lo = hi - 3;
                time = time2;
            }
            else
            {
                while(lo + 1 < hi)
                {
                    size_t id = lo + hi >> 1;
                    if (cds.getTime(id) <= time)
                        lo = id;
                    else
                        hi = id;
                }
                if (lo >= hi)
                    return false;
            }

            float dd = (float)cds.getTime(lo);
            const float (&d4)[4] = spline::bspline_d((time - dd) / (cds.getTime(lo + 1) - dd));

            const T &a = *(const T*)cds.getData(lo - 1);
            const T &b = *(const T*)cds.getData(lo);
            const T &c = *(const T*)cds.getData(lo + 1);
            const T &d = *(const T*)cds.getData(lo + 2);

            result = T(a * d4[0] + b * d4[1] + c * d4[2] + d * d4[3]);
            return true;
        }
    };
};
#pragma warning(pop)

#endif//CANI_INTERPOLATERS_H