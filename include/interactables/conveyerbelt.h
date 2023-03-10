#ifndef CONVEYERBELT_H_INCLUDED
#define CONVEYERBELT_H_INCLUDED

#include "activeinteractable.h"
#include "geometry/collision.h"
#include "physics/rigidbody.h"
#include "physics/physicsfuncs.h"

namespace solstice {
    class ConveyerBelt : public ActiveInteractable {
    protected:
        virtual void OnActivate(Interactable* obj);
        virtual void OnDeactivate(Interactable* obj);
    public:
        float mSpeed;
        ConveyerBelt(float speed = 3.0, bool active = true);
        ConveyerBelt(Rectanglef hitbox, float speed, bool active);
        void Init(Rectanglef hitbox, float speed, bool active);
        virtual void OnNotify(const Interactable& entity, InteractableEvent event);
        Collision CheckBodyCollision(RigidBody* body);
    };

}

#endif // CONVEYERBELT_H_INCLUDED
