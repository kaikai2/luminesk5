#include <cassert>

#include "caAnimElement.h"
#include "caLocus.h"

namespace cAni
{
    bool AnimElement::sample(int frameTime, iClipState &rCS, iRenderer &render) const
    {
        if (locus.empty())
            return false;

        for (size_t i = 0; i < locus.size(); i++)
        {
            const Locus &rL = locus[i];
            rCS.setStateMergeStyle(rL.sid, rL.mergeStyle);
            if (iCurveInterpolater *pInterpolator = render.getInterpolator(rL.sid, rL.interpolateStyle))
            {
                if (!rCS.makeSample(rL.sid, *pInterpolator, *rL.dataset, frameTime))
                {
                    return false;
                }
            }
        }
        return true;
    }
} // namespace cAni
