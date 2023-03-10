#ifndef MOVEMENTCOMPONENT_H_INCLUDED
#define MOVEMENTCOMPONENT_H_INCLUDED

#include "physics/movableobject.h"

namespace solstice {
    class MovementComponent {
    protected:
        Vector2 mDirection;
    public:
        Vector2f mVelocity, mAcceleration;
        void MoveObject(double dt, MovableObject* obj);
    };
}

#endif // MOVEMENTCOMPONENT_H_INCLUDED
