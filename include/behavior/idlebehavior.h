#ifndef IDLEBEHAVIOR_H_INCLUDED
#define IDLEBEHAVIOR_H_INCLUDED

#include "behavior.h"

namespace solstice {
    class IdleBehavior : public Behavior {
    public:
        IdleBehavior();
        void Update(double dt, MovableObject* obj);
    };

}

#endif // IDLEBEHAVIOR_H_INCLUDED
