#ifndef PHYSICSFUNCS_H_INCLUDED
#define PHYSICSFUNCS_H_INCLUDED

#include "rigidbody.h"
#include "graphics/vector2.h"

namespace solstice {
    Vector2f CalcVelocity(RigidBody* body, float friction, float elasticity, Vector2f normal);
}

#endif // PHYSICSFUNCS_H_INCLUDED
