#ifndef __CurvedAni_AnimData_H_
#define __CurvedAni_AnimData_H_

#include <cassert>
#include <vector>
#include "caAnimElement.h"

using std::vector;

namespace cAni
{
    class iByteStream;
    class iClipState;
    class iCurveDataSet;
    //#pragma warning(push)
    //#pragma warning(disable:4786)


    /// AnimData 
    /// @note set of AnimElements and the total info of the animation
    class AnimData
    {
        AnimData() : renderer(0), nRefCount(0)
        {
        }
        /// dtor, faces need to be released
        virtual ~AnimData() // use locus_anim_free() function to free the animdata
        {
            release();
        }

        bool loadFromByteStream(iByteStream *bs);
        bool loadFromXml(const char *filename);
        void release();

        mutable int nRefCount;
    public:
        friend class Animation;
        friend class AnimResManager;

        /// @fn render
        /// @param  time    [in] frame time
        /// @param  cliprect [in] clipper rectangle (not implemented yet)
        void render(int frameTime, const iClipState *defaultClipState) const;
        bool saveToByteStream(iByteStream *bs) const;
        bool saveToXml(const char *filename) const;

        void releaseReference() const
        {
            if (nRefCount > 0)
            {
                nRefCount--;
            }
            else
            {
                assert(nRefCount > 0);
            }
        }
        void requireReference() const
        {
            nRefCount++;
        }

        vector<AnimElement> animElements;
        vector<iCurveDataSet*> datasets;
        //vector<iCurveInterpolater*> interpolaters;
        iRenderer *renderer;
        int length;
    };

} // namespace cAni

#endif//__CurvedAni_AnimData_H_
