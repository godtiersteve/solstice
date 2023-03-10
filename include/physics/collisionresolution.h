#ifndef COLLISIONRESOLUTION_H_INCLUDED
#define COLLISIONRESOLUTION_H_INCLUDED
#include "geometry/collision.h"
#include "rigidbody.h"
#include "environment/backgroundtile.h"
namespace solstice {
    void ResolveCollision(RigidBody* b1, RigidBody* b2, Collision& c);
    void ResolveCollision(RigidBody* b1, BackgroundTile2& ground, Collision& c);
}


#endif // COLLISIONRESOLUTION_H_INCLUDED
