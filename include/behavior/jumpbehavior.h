#ifndef JUMPBEHAVIOR_H_INCLUDED
#define JUMPBEHAVIOR_H_INCLUDED

#include "behavior.h"

namespace solstice {
    class JumpBehavior : public Behavior {
    protected:
        bool mJumped;
    public:
        JumpBehavior(float jumpstrength = 20) {
            mJumpStrength = jumpstrength;
            mBehaviorType = BEHAVIOR_JUMP;
            mMotion.mType = ENMOTION_MOVES;
            mMotion.mStopsMomentum = false;
            mName = "Jumping";
        }
        float mJumpStrength;
        void Update(double dt, MovableObject* obj);
        void Reset() {Behavior::Reset(); mJumped = false;}
        bool Finished() {return mJumped;}
    };
}

#endif // JUMPBEHAVIOR_H_INCLUDED
