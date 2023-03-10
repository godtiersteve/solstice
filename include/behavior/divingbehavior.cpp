#include "divingbehavior.h"

namespace solstice {
    DivingBehavior::DivingBehavior(float diveSpeed) {
        mMotion.SetVelocity(Vector2f(0, -fabs(diveSpeed)));
        mName = "Diving";
        mBehaviorType = BEHAVIOR_DIVE;
    }
    void DivingBehavior::Update(double dt, MovableObject* obj) {
        if(obj) {
            if(obj->GetUnderwater() && !obj->CheckState(MOVE_SWIMMING_UNDERWATER)) {
                if(obj->GetVelocity().y > 0) {
                    obj->StopY();
                }
                obj->ApplyImpulseY(-fabs(mMotion.GetVelocity().y));
            }
            float subDepth = obj->GetSubmergeRatio();
            if(subDepth >= 1.25) {
                obj->ChangeState(MOVE_SWIMMING_UNDERWATER);
                mFinished = true;
            }
        }
    }

}
