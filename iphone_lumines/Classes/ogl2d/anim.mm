#pragma warning(disable:4786)
#import <cassert>
#import <algorithm>
#import <cstdio>

#import "anim.h"
#import "bspline.h"

using namespace std;

namespace ogl2d
{

AnimElement::AnimElement(LocusFace2dPtr face, Locus2fPtr pos, Locus2fPtr zoom, Locus1fPtr rotate,
           Locus3fPtr color, Locus1fPtr alpha):
    m_pFace2d(face), m_pPos(pos), m_pZoom(zoom), m_pRot(rotate), m_pColor(color), m_pAlpha(alpha)
{
    assert(face != NULL);
    memset(m_LocusStartTime, 0, sizeof(m_LocusStartTime));
}

void AnimElement::render(int time, const Rect *cliprect) const
{
    Clip clip;
    assert(m_pFace2d);

    if (!m_pFace2d->getInterval(time - m_LocusStartTime[Face], clip.pFace))
        return;
    
    if (m_pPos)
    {
        if (m_pPos->getInterval(time - m_LocusStartTime[Pos], clip.m_p))
        {
            clip.m_visible = true;
            //clip.m_p += m_Pos;
        }
        else
        {
            clip.m_visible = false;
        }
    }
    if (m_pZoom)
    {
        m_pZoom->getInterval(time - m_LocusStartTime[Zoom], clip.m_zoom);
    }
    if (m_pRot)
    {
        m_pRot->getInterval(time - m_LocusStartTime[Rot], clip.m_rot);
    }
    if (m_pColor)
    {
        m_pColor->getInterval(time - m_LocusStartTime[Color], clip.m_color);
    }
    if (m_pAlpha)
    {
        m_pAlpha->getInterval(time - m_LocusStartTime[Alpha], clip.m_alpha);
    }
    clip.render(cliprect);
}
void Animation::init(AnimId animIdCount)
{
    m_play = false;
    m_loop = true;
    m_startTime = 0;
    m_Pos *= 0;
    m_AnimDatas.clear();
    m_AnimDatas.assign(animIdCount, NULL);
    m_CurAnimId = DefaultAnimId;
}
void Animation::setAnimData(const AnimData *pAnimData, AnimId aniId)
{
    assert(pAnimData != NULL);
    assert(aniId < m_AnimDatas.size());
    m_AnimDatas[aniId] = pAnimData;
    if (m_CurAnimId == DefaultAnimId)
        m_CurAnimId = aniId;
}

bool Animation::startAnim(int curtime, AnimId aniId/* = DefaultAnimId*/)
{
    if (aniId != DefaultAnimId)
    {
        assert(aniId < m_AnimDatas.size());
        m_CurAnimId = aniId;
    }
    if (getCurAnimData() == NULL)
    {
        // could not found a valid animdata using the m_CurAnimId
        m_CurAnimId = DefaultAnimId;
        return false; // failed to start animation
    }
    m_play = true;
    //m_curTime = 0;
    m_startTime = curtime;
    return true;
}

bool Animation::checkEnd(int curtime) const
{
    const AnimData *pAnimData = getCurAnimData();
    return pAnimData == NULL || curtime - m_startTime >= pAnimData->m_length;
}

void Animation::setAnimLoop(bool bLoop)
{
    m_loop = bLoop;
}
/*
void Animation::setAnimTimer(int time)
{
    m_curTime = time;
}
*/

extern TextureResource s_AnimTextureRes;

AnimData::~AnimData()
{
    vector< Face2d* >::iterator face = m_Faces.begin();
    while(face != m_Faces.end())
    {
        delete *face;
        ++face;
    }
    m_Faces.clear();
    vector< AnimElement >::iterator element = m_AnimElements.begin();
    while(element != m_AnimElements.end())
    {
        delete element->m_pFace2d;
        delete element->m_pPos;
        delete element->m_pZoom;
        delete element->m_pRot;
        delete element->m_pColor;
        delete element->m_pAlpha;
        ++element;
    }
    m_AnimElements.clear();
}

void AnimData::render(int time, const Rect *cliprect) const
{
    for(vector< AnimElement >::const_iterator anim = m_AnimElements.begin();
        anim != m_AnimElements.end(); ++anim)
    {
        //anim->SetClipInfo(m_curTime);
        anim->render(time, cliprect);
    }
}
void Animation::render(int time, const Rect *cliprect) const
{
    assert(m_CurAnimId < m_AnimDatas.size());
    const AnimData *pAnimData = m_AnimDatas[m_CurAnimId];
    assert(pAnimData != NULL);
    time -= m_startTime;
    if (m_loop)
        time %= pAnimData->m_length;
    else
    {
        if (time > pAnimData->m_length)
            time = pAnimData->m_length;
    }

    glPushMatrix();
    glTranslatef(m_Pos.x, m_Pos.y, 0);
    pAnimData->render(time, cliprect);
    glPopMatrix();
}

AnimResManager::AnimResManager(TextureResource *pTexRes):m_pTexRes(pTexRes)
{
    assert(pTexRes != NULL);
}

AnimResManager::~AnimResManager()
{
    vector< Pair >::iterator animdata = m_AnimDatas.begin();
    while(animdata != m_AnimDatas.end())
    {
        locus_anim_free(animdata->pAnimData);
        ++animdata;
    }
    m_AnimDatas.clear();
}

const AnimData* AnimResManager::getAnimData(const char *aniFileName)
{
    assert(aniFileName != NULL && *aniFileName != 0);
    vector< Pair >::iterator node = find(m_AnimDatas.begin(), m_AnimDatas.end(), aniFileName);
    if (node != m_AnimDatas.end())
    {
        return node->pAnimData;
    }
    string filename = aniFileName;
    FILE *fp = fopen((filename + ".txt").c_str(), "rt");
    if (fp == NULL)
        return NULL;
    AnimData *pAnimData = NULL;
    bool ret = locus_anim_read_text(fp, m_pTexRes, &pAnimData);
    fclose(fp);
    if (!ret)
        return NULL;
    assert(pAnimData != NULL);
    m_AnimDatas.push_back(Pair(pAnimData, aniFileName));
    return pAnimData;
}

}; // namespace ogl2d

