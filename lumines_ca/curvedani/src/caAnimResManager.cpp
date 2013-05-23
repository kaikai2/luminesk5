#include "caAnimResManager.h"
#include "caAnimData.h"

#include <algorithm>
using std::find;

#pragma warning(disable:4996)

namespace cAni
{

    AnimResManager::~AnimResManager()
    {
        vector<Pair>::iterator animdata = m_AnimData.begin();
        while(animdata != m_AnimData.end())
        {
            delete animdata->pAnimData;
            ++animdata;
        }
        m_AnimData.clear();
    }
    bool AnimResManager::saveToByteStream(const AnimData *pAnimData, iByteStream *bs) const
    {
        return pAnimData->saveToByteStream(bs);
    }
    bool AnimResManager::saveToXml(const AnimData *pAnimData, const char *filename) const
    {
        return pAnimData->saveToXml(filename);
    }
    bool AnimResManager::release(const char *aniFileName)
    {
        const AnimData* pAnimData = findAnimData(aniFileName);
        if (pAnimData == 0)
        {
            return false;
        }

        if (pAnimData->nRefCount > 0)
        {
            return false;
        }

        vector< Pair >::iterator node = find(m_AnimData.begin(), m_AnimData.end(), aniFileName);
        if (node != m_AnimData.end())
        {
            m_AnimData.erase(node);
            delete pAnimData;
        }
        else
        {
            assert(0);
            return false;
        }
        return true;
    }

    const AnimData* AnimResManager::findAnimData(const char *aniFileName) const
    {
        assert(aniFileName != NULL && *aniFileName != 0);
        vector< Pair >::const_iterator node = find(m_AnimData.begin(), m_AnimData.end(), aniFileName);
        if (node != m_AnimData.end())
        {
            return node->pAnimData;
        }
        return 0;
    }

    const AnimData* AnimResManager::loadAnimData(const char *aniFileName)
    {
        assert(aniFileName != NULL && *aniFileName != 0);
        vector< Pair >::iterator node = find(m_AnimData.begin(), m_AnimData.end(), aniFileName);
        if (node != m_AnimData.end())
        {
            return node->pAnimData;
        }

        char *s2 = strdup(aniFileName);
        if (!s2)
            return 0;

        vector<const char *> pathTokens;
        for (const char *pathToken = strtok(s2, "/\\"); pathToken; pathToken = strtok(0, "/\\"))
        {
            pathTokens.push_back(pathToken);
        }
        const char *cdot = strrchr(pathTokens.back(), '.');
        bool bXmlFile = false;
        if (cdot && 0 == strcmp(cdot, ".xml"))
            bXmlFile = true;
        free(s2);

        if (bXmlFile)
        {
            AnimData *pAnimData = new AnimData;
            if (pAnimData)
            {
                if (!pAnimData->loadFromXml(aniFileName))
                {
                    delete pAnimData;
                    pAnimData = 0;
                }
            }
            return pAnimData;
        }
        else
        {
            iByteStream *byteStream = iSystem::GetInstance()->loadStream(aniFileName);
            if (!byteStream)
                return 0;

            AnimData *pAnimData = new AnimData;
            if (pAnimData)
            {
                if (!pAnimData->loadFromByteStream(byteStream))
                {
                    delete pAnimData;
                    pAnimData = 0;
                }
            }

            iSystem::GetInstance()->release(byteStream);
            return pAnimData;
        }
    }

    const AnimData* AnimResManager::getAnimData(const char *aniFileName)
    {
        const AnimData *pAnimData = findAnimData(aniFileName);
        if (!pAnimData)
        {
            pAnimData = loadAnimData(aniFileName);
            if (pAnimData)
                m_AnimData.push_back(Pair(pAnimData, aniFileName));
        }
        return pAnimData;
    }

} // namespace cAni
