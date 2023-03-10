#ifndef TRACKINGBEHAVIOR_H_INCLUDED
#define TRACKINGBEHAVIOR_H_INCLUDED

#include "behavior.h"

namespace solstice {
class TrackingBehavior : public Behavior {
protected:
    bool TryJump(Vector2f distance, RigidBody* body);
public:
    float mTargetRange, mMinTargetRange, mJumpStrength, mJumpDelay;
    TrackingBehavior();
    TrackingBehavior(float speed, float targetRange, float minRange, float jumpStrength = 0, float jumpDelay = 0);
    void Init(float speed, float targetRange, float minRange, float jumpStrength = 0, float jumpDelay = 0);
    void Update(double dt, MovableObject* obj);
};
}


#endif // TRACKINGBEHAVIOR_H_INCLUDED
