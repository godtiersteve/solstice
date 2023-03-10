#ifndef WETSPOT_H_INCLUDED
#define WETSPOT_H_INCLUDED

#include "geometry/hitbox.h"

namespace solstice {
    class WetSpot {
    public:
        WetSpot(float change = 10) {
            mH2oChange = change;
        }
        WetSpot(HitBoxf hitbox, float change): mHitBox(hitbox), mH2oChange(change);
        float mH2oChange;
        HitBoxf mHitBox;
    };

}

#endif // WETSPOT_H_INCLUDED
