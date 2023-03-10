#ifndef FLEEINGBEHAVIOR_H_INCLUDED
#define FLEEINGBEHAVIOR_H_INCLUDED

#include "behavior.h"
#include "trackingbehavior.h"

namespace solstice {
class FleeingBehavior : public TrackingBehavior {
public:
    FleeingBehavior();
    FleeingBehavior(float speed, float targetRange, float minRange, float jumpStrength = 0, float jumpDelay = 0);
    void Update(double dt, MovableObject* obj);
};
}


#endif // FLEEINGBEHAVIOR_H_INCLUDED
