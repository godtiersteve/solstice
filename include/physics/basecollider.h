#ifndef BASECOLLIDER_H_INCLUDED
#define BASECOLLIDER_H_INCLUDED

#include "geometry/rectangle.h"
#include "graphics/vector2.h"
#include "geometry/collision.h"
#include "geometry/hitdetection.h"

namespace solstice {
class BaseCollider {
protected:
    Rectanglef mHitBox;
    Vector2f mLastPos;
    float mElasticity, mInvMass;

public:

};

}


#endif // BASECOLLIDER_H_INCLUDED
