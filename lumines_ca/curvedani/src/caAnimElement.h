#ifndef __CurvedAni_AnimElement_H_
#define __CurvedAni_AnimElement_H_

#include <vector>
#include "caLocus.h"
using std::vector;
namespace cAni
{
    class iClipState;
    class iRenderer;
//#pragma warning(push)
//#pragma warning(disable:4786)


    /// AnimElement
    /// @note smallest single part of an animation
    struct AnimElement
    {
        bool sample(int frameTime, iClipState &cs, iRenderer &render) const;
        vector<Locus> locus;
        int parentIndex; // -1 for none, other for index
        // int m_LocusStartTime[6]; // elements may have the same locus only differs in the start time
        // string m_name;
    protected:
        //int m_time;   // anim length
    };

} // namespace cAni

#endif//__CurvedAni_AnimElement_H_
