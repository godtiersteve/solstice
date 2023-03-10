#include "jumpbehavior.h"
namespace solstice {
void JumpBehavior::Update(double dt, MovableObject* obj) {
    if((obj && obj->GetGrounded() || (obj->GetSubmergeRatio() >= 0.5 && !obj->CheckState(MOVE_SWIMMING_UNDERWATER))&& !mJumped)) {
        obj->ApplyImpulseY(mJumpStrength);
        mJumped = true;
    }
}
}
