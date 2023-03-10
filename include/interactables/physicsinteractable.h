#ifndef PHYSICSINTERACTABLE_H_INCLUDED
#define PHYSICSINTERACTABLE_H_INCLUDED

#include "interactables/interactable.h"
#include "physics/physicsrect.h"

namespace solstice {
    class PhysicsInteractable : public Interactable, public PhysicsBox2f {
    };
}

#endif // PHYSICSINTERACTABLE_H_INCLUDED
