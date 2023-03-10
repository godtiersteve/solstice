#include "barrel.h"

namespace solstice {
Barrel::Barrel() {
    mKnockedOver = false;
    mBroken = false;
    mID = 300;
    mObjectType = OBJECT_BARREL;
    mCircleHitBox.SetSize(30);
    mUprightHeight = 80;
    mTiltedHeight = 50;
    mGravityAccel = 2500;
    mDeceleration = 0;
    mFriction = 0.05f;
}

Barrel::Barrel(Vector2f pos, Rectanglef hitbox, Circlef circleHitBox) {
    mKnockedOver = false;
    mBroken = false;
    mHitBox = hitbox;
    mCircleHitBox = circleHitBox;
    mHitBox.SetPosition(pos);
    mCircleHitBox.SetPosition(pos);
    mObjectType = OBJECT_BARREL;
    mGravityAccel = 2500;
    mTiltedHeight = 50;
    mDeceleration = 0;
    mFriction = 0.05f;
}

void Barrel::Update(float tstep) {
    mRotVelocity = 0;
    PhysicsBox2f::Update(tstep);
    mCircleHitBox.SetPosition(mPosition);
    mHitBox.SetPosition(mPosition);
}

void Barrel::UpdateVerlet(float dt) {
    PhysicsBox2f::UpdateVerlet(dt);
    mCircleHitBox.SetPosition(mPosition);
    mHitBox.SetPosition(mPosition);
}
IShapef* Barrel::GetHitBoxShape() {
    return &mHitBox;
    if(mKnockedOver) {
        return &mCircleHitBox;
    }
    return &mHitBox;
}
IShapef* Barrel::GetHitShapef() {
    //return &mHitBox;
    if(mKnockedOver) {
        return &mCircleHitBox;
    }
    return &mHitBox;
}


void Barrel::Break() {
    mBroken = true;
    //mHitBox.SetHeight(mUprightHeight);
}
void Barrel::SetPosition(Vector2f pos) {
    Vector2f vel = GetPosition() - pos;
    mHitBox.SetPosition(pos);
    mCircleHitBox.SetPosition(pos);
    mLastPos = pos - vel;
}
void Barrel::SetY(float y) {
    mHitBox.SetX(y);
    mCircleHitBox.SetY(y);
    mLastPos.y = y;
}
void Barrel::SetX(float x) {
    mHitBox.SetX(x);
    mCircleHitBox.SetY(x);
    mLastPos.x = x;
}
void Barrel::AddXY(Vector2f mv) {
    mHitBox.AddXY(mv);
    mCircleHitBox.AddXY(mv);
    mLastPos.x+= mv.x;
    mLastPos.y+= mv.y;
}
void Barrel::AddX(float x) {
    mHitBox.AddX(x);
    mCircleHitBox.AddX(x);
    mLastPos.x+= x;
}
void Barrel::AddY(float y) {
    mHitBox.AddX(y);
    mCircleHitBox.AddX(y);
    mLastPos.y+= y;
}
void Barrel::KnockOver(Vector2f force) {
    if(!mKnockedOver) {
        mUprightHeight = mHitBox.GetHeight();
    }
    //mHitBox.SetHeight(mTiltedHeight);
    mKnockedOver = true;
    ApplyImpulse(force);
}
void Barrel::Right() {
    mKnockedOver = false;
}
Vector2f Barrel::CalcVelocity(RigidBody* b, Collision& c) {
    return RigidBody::CalcVelocity(b, c);
}
Vector2f Barrel::CalcVelocity(BackgroundTile2& ground, Collision& c) {
    return RigidBody::CalcVelocity(ground, c);
}
Vector2f Barrel::CalcVelocity(Vector2f vel, Collision& col, float friction, float elasticity) {
    return RigidBody::CalcVelocity(vel, col, mFriction, elasticity);
}
void Barrel::CheckAndResolveCollision(RigidBody* b) {
    return;
}
void Barrel::CheckAndResolveCollision(BackgroundTile2& ground) {
    RigidBody::CheckAndResolveCollision(ground);
}

}
