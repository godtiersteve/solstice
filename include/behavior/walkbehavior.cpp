#include "walkbehavior.h"
namespace solstice {
WalkBehavior::WalkBehavior() {
    Init(0, -1, false, 0, 0);
    mName = "Walking";
}
void WalkBehavior::Init(Vector2f vel, float distance, bool turns, float hopTimer, float hopStrength) {
    mMotion.mVelocity = vel;
    mDistanceToTravel = distance;
    mTurnsAtEnd = turns;
    mHopTimer = hopTimer;
    mHopStrength = hopStrength;
    mDistanceTraveled = 0;
    mBehaviorType = BEHAVIOR_WALK;
    mPriority = BEHAVIOR_PRIORITY_LOW;;
}
void WalkBehavior::Update(double dt, MovableObject* obj) {
    if(mDistanceToTravel >= 1) {
        Vector2f vel = obj->GetVelocity2();
        mDistanceTraveled+= obj->GetVelocity2().x * 0.1;
        if((!mMotion.mFlipped && (mDistanceTraveled >= mDistanceToTravel)) || (mMotion.mFlipped && mDistanceTraveled <= 0)) {
            if(mTurnsAtEnd) {
                mMotion.Flip();
            }
            else {
                mDistanceTraveled = mDistanceToTravel;
            }
        }
    }
    obj->ApplyForce(mMotion.GetVelocity());
    if(mHopStrength >= 1.0 && obj->CheckState(MOVE_GROUNDED)) {
        if(static_cast<RigidBody*>(obj)->GetGrounded()) {
            mTimer.mElapsedTime+= dt;
            if(mTimer.mElapsedTime >= mHopTimer) {
                obj->ApplyImpulseY(mHopStrength);
                mTimer.mElapsedTime = 0;
            }
        }
        else {
            mTimer.mElapsedTime = 0;
        }
    }
    else {
        obj->ApplyForceY(mMotion.GetVelocity().y);
    }
}
void WalkBehavior::Reverse() {
    mMotion.Flip();
    if(mMotion.GetVelocity().x > 0) {
        mDistanceTraveled = 0;
    }
    else if(mMotion.GetVelocity().y < 0) {
        mDistanceTraveled = mDistanceToTravel;
    }
}

}
