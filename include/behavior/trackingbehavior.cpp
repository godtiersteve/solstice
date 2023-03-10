#include "trackingbehavior.h"

namespace solstice {
TrackingBehavior::TrackingBehavior() {
    Init(0, 0, 0, 0, 0);
}

TrackingBehavior::TrackingBehavior(float speed, float targetRange, float minRange, float jumpStrength, float jumpDelay) {
    Init(speed, targetRange, minRange, jumpStrength, jumpDelay);
}
void TrackingBehavior::Init(float speed, float targetRange, float minRange, float jumpStrength, float jumpDelay) {
    mMotion.mVelocity.x = speed;
    mJumpDelay = jumpDelay;
    mTargetRange = targetRange;
    mJumpStrength = jumpStrength;
    mMinTargetRange = minRange;
    mPriority = BEHAVIOR_PRIORITY_LOW;
    mBehaviorType = BEHAVIOR_TRACKING;
}
void TrackingBehavior::Update(double dt, MovableObject* obj) {
    Vector2f pos = obj->GetPosition();
    Vector2f distance = pos - mDestination;
    float dist = pos.Distance(mDestination);
    float maxRange = mTargetRange * mTargetRange;
    float minRange = mMinTargetRange * mMinTargetRange;
    if(!obj->GetGrounded() && obj->CheckState(MOVE_GROUNDED)) {
        minRange = 0;
    }
    if(dist <= maxRange && dist > minRange) {
        RigidBody* body = (RigidBody*)(obj);
        if(distance.x < 0) {
            body->ApplyForceX(mMotion.mVelocity.x);
        }
        else if(distance.x > 0) {
            body->ApplyForceX(-mMotion.mVelocity.x);
        }
        if(TryJump(distance, body)) {
            mTimer.mElapsedTime+= dt;
            if(mJumpDelay <= mTimer.mElapsedTime && body->GetGrounded()) {
                body->ApplyImpulseY(mJumpStrength);
                mTimer.mElapsedTime = 0;
            }
        }
        else {
            mTimer.mElapsedTime = 0;
        }
        mFinished = false;
    }
    else if(dist < minRange) {
        obj->StopX();
        mFinished = true;
    }
    else if(dist >= maxRange) {
        mFinished = true;
    }

}
bool TrackingBehavior::TryJump(Vector2f distance, RigidBody* body) {
    if(distance.y < -100 && body->GetGrounded()) {
        return true;
    }
    else if(body->GetCollisionLocation(COL_LEFT) && distance.x > 0 || body->GetCollisionLocation(COL_RIGHT) && distance.x < 0 ) {
        return true;
    }
    return false;
}

}
