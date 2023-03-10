#include "damagetile.h"

namespace solstice {
    DamageTile::DamageTile(float baseDamage, DamageTileType type) {
        mBaseDamage = baseDamage;
        mDamageTileType = type;
    }
Collision DamageTile::CheckBodyCollision(RigidBody* b) {

}

}
