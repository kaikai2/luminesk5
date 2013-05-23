#pragma warning(disable: 4996)	// "name" 被声明为否决的
#pragma warning(disable:4786)
#include <cassert>
#include <vector>
#include <algorithm>
#include <cstdio>

#include "caAnimation.h"
#include "caAnimData.h"
#include "caBspline.h"

using namespace std;

namespace cAni
{

void Animation::init(AnimId animIdCount)
{
    m_play = false;
    m_loop = true;
    m_startTime = 0;
    m_AnimData.clear();
    m_AnimData.assign(animIdCount, NULL);
    m_CurAnimId = AnimId(DefaultAnimId);
}

void Animation::setAnimData(const AnimData *pAnimData, AnimId aniId)
{
    // assert(pAnimData != NULL);
    assert(aniId < m_AnimData.size());
    if (m_AnimData[aniId])
    {
        m_AnimData[aniId]->releaseReference();
    }
    m_AnimData[aniId] = pAnimData;
    if (pAnimData)
    {
        pAnimData->requireReference();
    }
    if (m_CurAnimId == AnimId(DefaultAnimId))
        m_CurAnimId = aniId;
}

const AnimData *Animation::getCurAnimData() const
{
    assert(m_CurAnimId == DefaultAnimId || m_CurAnimId < m_AnimData.size());
    return m_CurAnimId == DefaultAnimId ? NULL : m_AnimData[m_CurAnimId];
}

bool Animation::startAnim(int curtime, AnimId aniId/* = DefaultAnimId*/)
{
    if (aniId != DefaultAnimId)
    {
        assert(aniId < m_AnimData.size());
        m_CurAnimId = aniId;
    }
    if (getCurAnimData() == NULL)
    {
        // could not found a valid animdata using the m_CurAnimId
        m_CurAnimId = AnimId(DefaultAnimId);
        return false; // failed to start animation
    }
    m_play = true;
    m_startTime = curtime;
    return true;
}

bool Animation::checkEnd(int curtime) const
{
    const AnimData *pAnimData = getCurAnimData();
    return pAnimData == NULL || curtime - m_startTime >= pAnimData->length;
}

void Animation::setAnimLoop(bool bLoop)
{
    m_loop = bLoop;
}

void Animation::render(int frameTime, const iClipState *defaultClipState) const
{
    if (m_CurAnimId == DefaultAnimId)
        return;
    assert(m_CurAnimId < m_AnimData.size());
    const AnimData *pAnimData = m_AnimData[m_CurAnimId];
    assert(pAnimData != NULL);
    frameTime -= m_startTime;
    if (m_loop && pAnimData->length)
    {
        frameTime %= pAnimData->length;
    }
    else
    {
        if (frameTime > pAnimData->length)
            frameTime = pAnimData->length;
    }

    pAnimData->render(frameTime, defaultClipState);
}

}; // namespace cAni

