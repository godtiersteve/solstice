#include "fleeingbehavior.h"

namespace solstice {

FleeingBehavior::FleeingBehavior() {
    TrackingBehavior();
    mPriority = BEHAVIOR_PRIORITY_LOW;
    mName = "Fleeing";
}

FleeingBehavior::FleeingBehavior(float speed, float targetRange, float minRange, float jumpStrength, float jumpDelay):
TrackingBehavior(speed, targetRange, minRange, jumpStrength, jumpDelay)
{
    mBehaviorType = BEHAVIOR_FLEEING;
    mPriority = BEHAVIOR_PRIORITY_LOW;
    mName = "Fleeing";
}

void FleeingBehavior::Update(double dt, MovableObject* obj) {
    Vector2f pos = obj->GetPosition();
    Vector2f distance = pos - mDestination;
    float dist = pos.Distance(mDestination);
    float maxRange = mTargetRange * mTargetRange;
    float minRange = mMinTargetRange * mMinTargetRange;
    if(dist <= maxRange || mMinTargetRange < 0) {
        RigidBody* body = (RigidBody*)(obj);
        if(distance.x < 0) {
            body->ApplyForceX(-mMotion.mVelocity.x);
        }
        else if(distance.x > 0) {
            body->ApplyForceX(mMotion.mVelocity.x);
        }
        if(TryJump(-distance, body)) {
            mTimer.mElapsedTime+= dt;
            if(mJumpDelay <= mTimer.mElapsedTime && body->GetGrounded()) {
                body->ApplyImpulseY(mJumpStrength);
                mTimer.mElapsedTime = 0;
            }
        }
        else {
            mTimer.mElapsedTime = 0;
        }
    }
    else {
        obj->StopX();
    }
    if(dist < minRange) {
        obj->StopX();
    }
}

}
