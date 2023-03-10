#ifndef SWIMMINGBEHAVIOR_H_INCLUDED
#define SWIMMINGBEHAVIOR_H_INCLUDED

#include "behavior.h"

namespace solstice {
    class SwimmingBehavior : public Behavior {
    protected:
        float mSwimSpeed, mSwimAngle;
    public:
        SwimmingBehavior(float speed = 10);
        void SetSwimSpeed(float f);
        float GetSwimSpeed() {return mSwimSpeed;}
        float GetSwimAngle() {return mSwimAngle;}

        void Update(double dt, MovableObject* obj);
    };
}
#endif // SWIMMINGBEHAVIOR_H_INCLUDED
