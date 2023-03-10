#include "hovertrolley.h"

namespace solstice {
HoverTrolley::HoverTrolley(float levitateHeight, float maxWeight) {
    mNoTorque = true;
    mFriction = 0.5;
    mLevitateHeight = levitateHeight;
    mMinHeight = 15;
    if(maxWeight <= 0) {
        maxWeight = 15.0;
    }
    mMaxWeight = maxWeight;
    mCurrentWeight = 0;
    mHoverBox.SetNoGravity(true);
    mHoverBox.SetNoTorque(true);
    mInteractableType = INTERACT_TROLLEY;
}
void HoverTrolley::SetMaxWeight(float w)
{
    mMaxWeight = w;
    mBodyContainer.Clear();
}

void HoverTrolley::AddWeight(float w) {
    if(mCurrentWeight + w <= mMaxWeight) {
        mCurrentWeight+= w;
    }
}

void HoverTrolley::RemoveWeight(float w) {
    if(mCurrentWeight - w >= 0) {
        mCurrentWeight-= w;
    }
}
void HoverTrolley::Update(double dt) {
    PhysicsBox2f::Update(dt);
    mHoverBox.Update(dt);
    CalcWeight();
    CalcHeight();
}
void HoverTrolley::CalcWeight() {
    mCurrentWeight = mBodyContainer.GetWeight();
}
void HoverTrolley::CalcHeight() {
    float heightMod = mCurrentWeight / mMaxWeight;
    Vector2f height;
    height.y = (1.0 - heightMod) * mLevitateHeight + mMinHeight;
    mHoverBox.SetPosition(PhysicsBox2f::mHitBox.GetPosition() + height);
    mHoverBox.Stop();
    Interactable::mHitBox = PhysicsBox2f::mHitBox;
}
Collision HoverTrolley::CheckBodyCollision(RigidBody* body) {
    Collision c = solstice::CheckCollision(&body->GetHitBox(), &mHoverBox.GetHitBox());
    if(c && fabs(c.mtv.y) > 2.0) {
        if(c.normal.y >= 0.1 || body->GetVelocity().y < 0.0) {
            mBodyContainer.AddObject(body);
            CalcWeight();
            CalcHeight();
        }
        Vector2f vel = body->CalcVelocity(&mHoverBox, c);
        body->ResolveCollision(c, 1.0, vel);
        if(c.normal.y >= 0.1 || body->GetVelocity().y < 0.0) {
            body->SetY(mHoverBox.GetHitBox().GetTop() - 2);
        }
        //mHoverBox.CheckAndResolveCollision(body);
    }
    else if(!c) {
        mBodyContainer.RemoveObject(body);
    }
    return c;
}
}
