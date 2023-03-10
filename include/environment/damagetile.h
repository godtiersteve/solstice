#ifndef DAMAGETILE_H_INCLUDED
#define DAMAGETILE_H_INCLUDED

#include "interactables/interactable.h"

namespace solstice {
    enum DamageTileType {
        DTILE_ELECTRIC, DTILE_FIRE, DTILE_SUPERHEAT, DTILE_ICE
    };
    class DamageTile : public Interactable {
    public:
        DamageTileType mDamageTileType;
        float mBaseDamage;
        DamageTile(float baseDamage = 1, DamageTileType type = DTILE_ELECTRIC);
        Collision CheckBodyCollision(RigidBody* b);
    };
}


#endif // DAMAGETILE_H_INCLUDED
