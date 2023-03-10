#include "idlebehavior.h"

namespace solstice {
    IdleBehavior::IdleBehavior() {
        mBehaviorType = BEHAVIOR_IDLE;
        mName = "Idling";
    }
    void IdleBehavior::Update(double dt, MovableObject* obj) {
        if(obj) {
            Vector2f vel = obj->GetVelocity2();
            Vector2f motionVel = mMotion.GetVelocity();
            Vector2f impulse;
            if(vel.x > 0) {
                impulse.x = std::max(-(double)vel.x, -fabs(motionVel.x));
            }
            else if(vel.x < 0) {
                impulse.x = std::min(-(double)vel.x, fabs(motionVel.x));
            }
            if(obj->CheckState(MOVE_SWIMMING_UNDERWATER)) {
                if(vel.y > 0) {
                    impulse.y = std::max(-(double)vel.y, -fabs(motionVel.y));
                }
                else if(vel.y < 0) {
                    impulse.y = std::min(-(double)vel.y, fabs(motionVel.y));
                }
            }
            obj->ApplyImpulse(impulse);
            if(vel.x == 0) {
                mTimer.Update(dt);
            }
        }
    }
}
