#include "constraint.h"

namespace solstice {
Constraint::Constraint(float minLen, float stiffness, float damping) {
    mFirst = nullptr;
    mSecond = nullptr;
    SetAttributes(minLen, stiffness, damping);
    mNumIter = 1;
}
void Constraint::SetAttributes(float minLen, float stiffness, float damping) {
    mMinLength = minLen;
    mStiffness = stiffness;
    mDamping = damping;
}
void Constraint::Solve(RigidBody* b1, RigidBody* b2) {
    for(size_t i = 0; i < mNumIter; i++) {
        if(!b1 || !b2) {
            return;
        }
        Vector2f delta = b1->GetPosition() - b2->GetPosition();
        float currentLength = delta.Magnitude();
        if(!currentLength) {
            return;
        }
        if(currentLength <= mMinLength) {
            return;
        }

        float diff = (currentLength - mMinLength) / currentLength;
        float stiffness = mStiffness;
        Vector2f force = delta * diff * stiffness;
        float totalMass = b1->GetInvMass() + b2->GetInvMass();
        if(!totalMass) {
            return;
        }
        Vector2f normal;
        float normalVelocity;
        Vector2f relativeVelocity;
        if(mDamping) {
            normal = delta / currentLength;
            Vector2f vel = b1->GetPosition() - b1->GetLastPosition();
            Vector2f vel2 = b2->GetPosition() - b2->GetLastPosition();
            relativeVelocity = vel2 - vel;
            normalVelocity = normal.Dot(relativeVelocity);
        }
        float m1 = b1->GetInvMass() / totalMass;
        float m2 = b2->GetInvMass() / totalMass;
        Vector2f b1Vel = -m1 * force;
        Vector2f b2Vel = m2 * force;
        Vector2f b1LastVel = mDamping * normalVelocity * m1 * normal;
        Vector2f b2LastVel = mDamping * normalVelocity * m2 * normal;

        b1->GetHitBox().AddXY(b1Vel);
        b2->GetHitBox().AddXY(b2Vel);
        b1->GetLastPosition()-= b1LastVel;
        b2->GetLastPosition()+= b2LastVel;
        b1->GetVelocity() = b1->GetPosition() - b1->GetLastPosition();
        b2->GetVelocity() = b2->GetPosition() - b2->GetLastPosition();
    }
}
}
