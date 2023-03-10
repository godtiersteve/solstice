#ifndef PARTICLEANIM_H_INCLUDED
#define PARTICLEANIM_H_INCLUDED

#include "animation/animator.h"
#include "utility/sol_math.h"

namespace solstice {
struct ParticleAnimFrame {
    int row, col;
    int mDuration;
    int mCurFrame;
    ParticleAnimFrame(int c = 0, int r = 0, int dur = 5) {
        row = r;
        col = c;
        mDuration = dur;
        mCurFrame = 0;
    }
    void Update() {
        mCurFrame++;
        if(mCurFrame > mDuration) {
            mCurFrame = mDuration;
        }
    }
    bool Finished() {return mCurFrame >= mDuration;}
};
class ParticleAnimation : public Animator<ParticleAnimFrame> {
public:
    ParticleAnimation() {Start(); mLoops = -1;}
    void Update(float dt);
};

}

#endif // PARTICLEANIM_H_INCLUDED
