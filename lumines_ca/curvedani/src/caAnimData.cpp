#include "caAnimData.h"
#include "caAnimElement.h"
#include "caLocus.h"

#include <cassert>

namespace cAni
{
    void AnimData::release()
    {
        this->length = 0;

        animElements.clear();
        for (vector<iCurveDataSet*>::iterator ds = datasets.begin(); ds != datasets.end(); ++ds)
        {
            iSystem::GetInstance()->release(*ds);
        }
        datasets.clear();
        //     for (vector<iCurveInterpolater*>::iterator ip = interpolaters.begin(); ip != interpolaters.end(); ++ip)
        //     {
        //         iSystem::GetInstance()->release(*ip);
        //     }
        //    interpolaters.clear();
    }

    void AnimData::render(int frameTime, const iClipState *defaultClipState/* = 0*/) const
    {
        iClipState *cs = this->renderer->getClipState();
        const size_t nSize = animElements.size();
        cs->reset(defaultClipState);

        vector<iClipState *> allClipStates(nSize, 0); // need a object buffer
        for(size_t i = 0; i < nSize; i++)
        {
            allClipStates[i] = cs->clone(); // need a object buffer
            animElements[i].sample(frameTime, *allClipStates[i], *this->renderer);
        }
        for (size_t i = 0; i < nSize; i++)
        {
            if (size_t(animElements[i].parentIndex) < nSize)
            {
                this->renderer->render(*allClipStates[i], *allClipStates[animElements[i].parentIndex]);
            }
            else
            {
                this->renderer->render(*allClipStates[i], *cs);
            }
        }
        for(size_t i = 0; i < nSize; i++)
        {
            allClipStates[i]->release();
            // allClipStates[i] = 0; // this is not needed.
        }
    }

} // namespace cAni
