#ifndef DIVINGBEHAVIOR_H_INCLUDED
#define DIVINGBEHAVIOR_H_INCLUDED

#include "behavior.h"

namespace solstice {
    class DivingBehavior : public Behavior {
        public:
            DivingBehavior(float diveSpeed = 1);
            void Update(double dt, MovableObject* obj);
    };

}

#endif // DIVINGBEHAVIOR_H_INCLUDED
