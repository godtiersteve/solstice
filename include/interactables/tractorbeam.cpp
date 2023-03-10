#include "tractorbeam.h"

namespace solstice {

void TractorBeam::OnActivate(Interactable* obj) {}
void TractorBeam::OnDeactivate(Interactable* obj) {}
void TractorBeam::OnNotify(const Interactable& entity, InteractableEvent event) {}
TractorBeam::TractorBeam(float speed, Vector2f direction) {
    mSpeed = speed;
    mDirection = direction;
    mActivated = false;
}
TractorBeam::TractorBeam(Rectanglef hitbox, float speed, Vector2f direction, bool activated) {
    Init(hitbox, speed, direction, activated);
}
void TractorBeam::Init(Rectanglef hitbox, float speed, Vector2f direction, bool activated)
{
    mHitBox = hitbox;
    mSpeed = speed;
    mDirection = direction;
    mActivated = activated;
}

void TractorBeam::SetSpeed(float speed, Vector2f direction) {
    mSpeed = speed;
    mDirection = direction;
}

Collision TractorBeam::CheckBodyCollision(RigidBody* body) {
    Collision c = solstice::CheckCollision(&body->GetHitBox(), &mHitBox);
    if(c) {
        body->ApplyImpulse(mDirection * mSpeed);
    }
    return c;
}
Collision TractorBeam::CheckMovableCollision(MovableObject* o) {
    Collision c = solstice::CheckCollision(o->GetHitShapef(), &mHitBox);
    if(c) {
        o->ApplyImpulse(mDirection * mSpeed);
        //o->AddXY(mDirection * mSpeed);
    }
    return c;
}
}
