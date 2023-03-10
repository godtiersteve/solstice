#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED

#include "bulletbase.h"

namespace solstice {
    class Explosion : public BulletBase {
    public:
        float mInitialSize, mMaxSize;
        float mForce;
        float mVacuumSize;
    };
}


#endif // EXPLOSION_H_INCLUDED
