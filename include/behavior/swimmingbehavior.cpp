#include "swimmingbehavior.h"

namespace solstice {
    SwimmingBehavior::SwimmingBehavior(float speed) {
        mBehaviorType = BEHAVIOR_SWIM;
        SetSwimSpeed(speed);
        mSwimAngle = 0;
        mName = "Swimming";
    }

    void SwimmingBehavior::SetSwimSpeed(float f)
    {
        mSwimSpeed = f;
        mMotion.SetVelocity(mSwimSpeed);
        mMotion.SetDirection(Vector2i(1, 0));
    }

    void SwimmingBehavior::Update(double dt, MovableObject* obj) {
        if(mMotion.GetDirection().Magnitude2()) {
            mSwimAngle = atan2(mMotion.GetDirection().y, mMotion.GetDirection().x);
            Vector2f vel;
            vel.x = mSwimSpeed * cos(mSwimAngle);
            vel.y = mSwimSpeed * sin(mSwimAngle);
            if(SameSign(obj->GetVelocity2().x, vel.x) && fabs(obj->GetVelocity2().x) > fabs(vel.x)) {
                vel.x = 0;
            }
            if(SameSign(obj->GetVelocity2().y, vel.y) && fabs(obj->GetVelocity2().y > fabs(vel.y))) {
                vel.y = 0;
            }
            obj->ApplyImpulse(vel);
            //obj->ApplyForce(vel * 1000.0f);
        }
    }

}
