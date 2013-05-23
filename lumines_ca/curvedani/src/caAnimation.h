#ifndef __ANIM_H_
#define __ANIM_H_

#include <cassert>
#include <vector>
using std::vector;

#include "curvedani.h"

/*
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
*/
namespace cAni
{
#pragma warning(push)
#pragma warning(disable:4786)

class AnimData;

/// Animation
/// @note playing instance of the AnimData
class Animation : public iAnimation
{
public:
    typedef size_t AnimId;
    enum
    {
        DefaultAnimId = ~0,
    };
    Animation(AnimId animIdCount = 1)
    {
        init(animIdCount);
    }

    void setAnimData(const AnimData *pAnimData, AnimId aniId);
    bool startAnim(int curtime, AnimId aniId = DefaultAnimId);
    bool isPlaying() const
    {
        return m_play;
    }
    bool isLoop() const
    {
        return m_loop;
    }
    bool checkEnd(int curtime) const;
    void setAnimLoop(bool bLoop);
    AnimId getCurAnim() const{return m_CurAnimId;}
    // void setAnimTimer(int time);

    void render(int frameTime, const iClipState *defaultClipState = 0) const;
protected:
    void init(AnimId animIdCount);
    const AnimData *getCurAnimData() const;

    AnimId m_CurAnimId;
    vector<const AnimData *> m_AnimData;

    int m_startTime;    // start time in game timing

    // int m_curTime;  // current time in animation timing
    bool m_play;
    bool m_loop;  // is loop ?
};

}; // namespace cAni

#endif __ANIM_H_