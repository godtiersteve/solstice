#include "conveyerbelt.h"

namespace solstice {

void ConveyerBelt::OnActivate(Interactable* obj) {}

void ConveyerBelt::OnDeactivate(Interactable* obj) {}

ConveyerBelt::ConveyerBelt(float speed, bool active) {
    mActivated = active;
    mSpeed = speed;
    mInteractableType = INTERACT_CONVEYER_BELT;
}

ConveyerBelt::ConveyerBelt(Rectanglef hitbox, float speed, bool active) {
    Init(hitbox, speed, active);
    mInteractableType = INTERACT_CONVEYER_BELT;
}

void ConveyerBelt::Init(Rectanglef hitbox, float speed, bool active) {
    mActivated = active;
    mSpeed = speed;
    mHitBox = hitbox;
}

void ConveyerBelt::OnNotify(const Interactable& entity, InteractableEvent event) {}

Collision ConveyerBelt::CheckBodyCollision(RigidBody* body) {
    Collision c = solstice::CheckCollision(&body->GetHitBox(), &mHitBox);
    if(c) {
        Vector2f vel = solstice::CalcVelocity(body, 0.0, 0.0, c.normal);
        body->ResolveCollision(c, 1.0, vel);
        if(c.normal.y > 0.0 && mActivated) {
            body->ApplyImpulseX(mSpeed);
        }
    }
    return c;
}

}
