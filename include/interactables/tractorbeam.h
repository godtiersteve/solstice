#ifndef TRACTORBEAM_H_INCLUDED
#define TRACTORBEAM_H_INCLUDED

#include "interactables/activeinteractable.h"
#include "interactables/conveyerbelt.h"

namespace solstice {
    class TractorBeam : public ActiveInteractable {
    protected:
        virtual void OnActivate(Interactable* obj);
        virtual void OnDeactivate(Interactable* obj);
    public:
        Vector2f mDirection;
        float mSpeed;
        TractorBeam(float speed = 3.0, Vector2f direction = Vector2f(1.0, 0.0));
        TractorBeam(Rectanglef hitbox, float speed, Vector2f direction, bool activated = true);
        void Init(Rectanglef hitbox, float speed, Vector2f direction, bool activated);
        void SetSpeed(float speed, Vector2f direction);
        virtual void OnNotify(const Interactable& entity, InteractableEvent event);
        Collision CheckBodyCollision(RigidBody* body);
        Collision CheckMovableCollision(MovableObject* o);
    };
}


#endif // TRACTORBEAM_H_INCLUDED
