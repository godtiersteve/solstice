#ifndef EFFECTCHAIN_H_INCLUDED
#define EFFECTCHAIN_H_INCLUDED

#include "imageeffect.h"
#include <vector>
#include <algorithm>

#include "animation/animator.h"

namespace solstice {
class EffectChain : public Animator<ImageEffect> {
    protected:
        ImageEffect mActiveEffect;
    public:
        EffectChain();
        EffectChain(const EffectChain& eff);
        EffectChain& operator=(const EffectChain& eff);

        void AddEffect(ImageEffect eff);
        void AddEffect(ImageEffect eff, QuadData* quad);

        ImageEffect& GetEffect(size_t i)            {return mAnimation[i];}
        void Reverse();

        void Start();
        void Stop();
        void Pause(short dur = -1);
        void Reset();
        void Update(float dur = 1.0f);
        void Update(QuadData q);
};

}

#endif // EFFECTCHAIN_H_INCLUDED
