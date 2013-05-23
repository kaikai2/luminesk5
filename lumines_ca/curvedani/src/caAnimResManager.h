#ifndef __CurvedAni_AnimResManager_H_
#define __CurvedAni_AnimResManager_H_

#include "curvedani.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

namespace cAni
{
//#pragma warning(push)
//#pragma warning(disable:4786)


    /// AnimResManager
    /// @note   manage AnimData as resources
    class AnimResManager : public iAnimResManager
    {
    public:
        AnimResManager()
        {
        }
        virtual ~AnimResManager();

        /// getAnimData
        /// @param aniFileName  [in] main file name of ani file, no suffix needed
        virtual const AnimData* getAnimData(const char *aniFileName);
        virtual bool release(const char *aniFileName);
        virtual bool saveToByteStream(const AnimData *pAnimData, iByteStream *bs) const;
        virtual bool saveToXml(const AnimData *pAnimData, const char *filename) const;
    private:
        AnimResManager(const AnimResManager &o)
        {
            o;
        }
        AnimResManager &operator = (const AnimResManager &o)
        {
            o;
            return *this;
        }
        const AnimData* findAnimData(const char *aniFileName) const;
        const AnimData* loadAnimData(const char *aniFileName);

        struct Pair
        {
            Pair(const AnimData *_pAnimData, const char *_anifilename)
                :pAnimData(_pAnimData),anifilename(_anifilename)
            {
            }
            const AnimData* pAnimData;
            string anifilename;
            bool operator == (const char*name) const
            {
                return anifilename == name;
            }
        };
        vector< Pair > m_AnimData;
    };

} // namespace cAni

#endif __CurvedAni_AnimResManager_H_