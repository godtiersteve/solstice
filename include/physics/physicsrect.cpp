#include "physics/physicsrect.h"

namespace solstice {
    PhysicsBox2f::PhysicsBox2f() {
        Init();
    }
    PhysicsBox2f::PhysicsBox2f(Vector2f accel, Vector2f decel, Vector2f maxVel) {
        Init(accel, decel, maxVel);
    }
    PhysicsBox2f::PhysicsBox2f(const Rectangle<float>& hitbox, Vector2f accel, Vector2f decel, Vector2f maxVel) {
        Init(hitbox, accel, decel, maxVel);
    }
    void PhysicsBox2f::Init() {
        mVelocity = 0;
        mAcceleration = 0;
        mDeceleration = 0;
        mMaxVelocity = 10;
        mFriction = 0.0f;
        mInvMass = 1;
        mBuoyancy = 1;
        mElasticity = 0.0f;
        mGravityAccel = 2500;
        mUnderwater = false;
        mGrounded = false;
        mBuoyancyDamping = 0.95f;
        mDepthModifier = 1.0f;
        mSubmergeDepth = 0;
        mNoFloat = false;
        mNoGravity = false;
        mFollowingPath = false;
        mWaterDamping = 0.85f;
        mPassThroughPlatforms = false;
        mBoundCollider = NULL;
        mIgnorePhysics = false;
        mNoCollision = false;
        mLastPos = mHitBox.GetPosition();
        mState = MOVE_GROUNDED;
    }
    void PhysicsBox2f::Init(Vector2f accel, Vector2f decel, Vector2f maxVel) {
        Init();
        mAcceleration = accel;
        mDeceleration = decel;
        mMaxVelocity = maxVel;
        mLastPos = mHitBox.GetPosition();
    }
    void PhysicsBox2f::Init(const Rectangle<float>& hitbox, Vector2f accel, Vector2f decel, Vector2f maxVel) {
        Init();
        mHitBox = hitbox;
        mAcceleration = accel;
        mDeceleration = decel;
        mMaxVelocity = maxVel;
        mLastPos = mHitBox.GetPosition();
    }
    void PhysicsBox2f::ClearColliders() {
        mCollidersMovable.clear();
        mCollidersBG.clear();
        mBoundCollider = NULL;
    }
    void                PhysicsBox2f::SetAcceleration(Vector2f accel)       {mAcceleration = accel;}
    void                PhysicsBox2f::SetAccelerationX(float a)             {mAcceleration.x = a;}
    void                PhysicsBox2f::SetAccelerationY(float a)             {mAcceleration.y = a;}
    void                PhysicsBox2f::SetDeceleration(Vector2f decel)       {mDeceleration = decel;}
    void                PhysicsBox2f::SetDecelerationX(float a)             {mDeceleration.x = a;}
    void                PhysicsBox2f::SetDecelerationY(float a)             {mDeceleration.y = a;}
    void                PhysicsBox2f::SetMaxVelocity(Vector2f maxVel)       {mMaxVelocity = maxVel;}
    void                PhysicsBox2f::SetGravityAccel(float gravAccel)      {mGravityAccel = gravAccel;}
    Vector2f            PhysicsBox2f::GetDeceleration()                     {return mDeceleration;}
    Vector2f            PhysicsBox2f::GetMaxVelocity()                      {return mMaxVelocity;}
    float               PhysicsBox2f::GetGravityAccel()                     {return mGravityAccel;}

    void PhysicsBox2f::AddVelocityX(float x)            {mLastPos.x-= x;}
    void PhysicsBox2f::AddVelocityY(float y)            {mLastPos.y-= y;}
    void PhysicsBox2f::SetVelocityX(float v)            {mLastPos.x = GetPosition().x - v;}
    void PhysicsBox2f::SetVelocityY(float v)            {mLastPos.y = GetPosition().y - v;}
    void PhysicsBox2f::AddVelocity(Vector2f velocity)   {mLastPos-= velocity;}
    void PhysicsBox2f::SetForce(Vector2f force) {
        mAppliedForce = force;
    }
    void PhysicsBox2f::Accelerate() {
        if((int)mAcceleration.x == 0 && (int)mAcceleration.y == 0) {
            return;
        }
        ApplyForce(mAcceleration);
    }
    void PhysicsBox2f::Decelerate() {
        if(mDeceleration.x == 0 && mDeceleration.y == 0) {
            return;
        }
        Vector2f decel;
        if(mVelocity.x > 0) {
            decel.x = -mDeceleration.x;
        }
        if(mVelocity.x < 0) {
            decel.x = mDeceleration.x;
        }
        if(mVelocity.y > 0) {
            decel.y = -mDeceleration.y;
        }
        else if(mVelocity.y < 0) {
            decel.y = mDeceleration.y;
        }
        if(decel.x != 0 || decel.y != 0) {
            ApplyForce(decel);
        }
    }
    void PhysicsBox2f::Move(float tstep) {
        Vector2f moveVel = mVelocity * tstep;
        mHitBox.AddXY(moveVel);
    }
    void PhysicsBox2f::Fall(float vel) {
        if(!mNoGravity && !mGrounded && (CheckState(MOVE_GROUNDED) || CheckState(MOVE_SWIMMING_SURFACE))) {
            float gravitationalForce = 0;
            float submergeArea = ((mSubmergeDepth - mHitBox.GetHeight()) / mHitBox.GetHeight());
            float gravity = fabs(vel) > 100 ? vel : mGravityAccel;
            gravitationalForce = gravity * submergeArea;
            ApplyForceY(gravitationalForce);
        }
    }
    void PhysicsBox2f::Float() {
        if(mUnderwater && mSubmergeDepth && !mNoFloat && !mNoGravity && !CheckState(MOVE_SWIMMING_UNDERWATER)) {
            float submergedArea = mSubmergeDepth / mHitBox.GetHeight();
            float buoyantForce = mGravityAccel * submergedArea * mDepthModifier;
            ApplyForceY(buoyantForce);
            if(mVelocity.y < 5.0) {
                mLastPos.y = mHitBox.GetPosition().y - (mHitBox.GetPosition().y - mLastPos.y) * mBuoyancyDamping;
            }
            submergedArea = submergedArea * mDepthModifier;
            if(fabs(mVelocity.y) <= 2.0f && submergedArea >= 0.4f && submergedArea <= 0.8f) {
                //ApplyForceY(mGravityAccel * 0.1);
            }
        }
    }
    void PhysicsBox2f::AdjustVelocity() {
        if(mVelocity.x > mMaxVelocity.x) {
            mVelocity.x = mMaxVelocity.x;
        }
        else if(mVelocity.x < -mMaxVelocity.x) {
            mVelocity.x = -mMaxVelocity.x;
        }
        if(mVelocity.y > mMaxVelocity.y) {
            mVelocity.y = mMaxVelocity.y;
        }
        else if(mVelocity.y < -mMaxVelocity.y) {
            mVelocity.y = -mMaxVelocity.y;
        }
        if(fabs(mVelocity.x) < 0.075) {
           mVelocity.x = 0.0f;
        }
        if(fabs(mVelocity.y) < 0.075) {
           mVelocity.y = 0.0f;
        }
        mVelocity.Truncate(2);
    }
    void PhysicsBox2f::AdjustVelocity(Vector2f& vel) {
        if(vel.x > mMaxVelocity.x) {
            vel.x = mMaxVelocity.x;
        }
        else if(vel.x < -mMaxVelocity.x) {
            vel.x = -mMaxVelocity.x;
        }
        if(vel.y > mMaxVelocity.y) {
            vel.y = mMaxVelocity.y;
        }
        else if(vel.y < -mMaxVelocity.y) {
            vel.y = -mMaxVelocity.y;
        }
        if(fabs(vel.x) < fabs(mDeceleration.x) * mStep * mStep) {
           vel.x = 0.0f;
        }
        if(fabs(vel.y) < fabs(mDeceleration.y) * mStep * mStep) {
           vel.y = 0.0f;
        }
        vel.Truncate(2);
    }
    void PhysicsBox2f::AdjustAngle(float& angle) {
    }
    void PhysicsBox2f::Shoot() {
        if(mFollowingPath) {
            Stop();
        }
        mVelocity = 100;
        mAcceleration = 0;
        mIgnorePhysics = false;
        mFollowingPath = false;
    }
    void PhysicsBox2f::Shoot(Vector2f velocity) {
        if(mFollowingPath) {
            Stop();
        }
        mVelocity = velocity;
        mAcceleration = 0;
        mIgnorePhysics = false;
        mFollowingPath = false;
        mDeceleration.x = 0;
    }
    void PhysicsBox2f::UpdatePlatform() {
        if(mBoundCollider) {
            float y = mHitBox.GetY();
            mHitBox.SetBottom(mBoundCollider->GetHitBox().GetTop() - 2);
            mLastPos.y-= y - GetPosition().y;
            mBoundCollider->GetSpring().height = mBoundCollider->GetSpring().targetHeight - 20;
        }
    }
    void PhysicsBox2f::Update(float tstep) {
        if(mInvMass2 <= 0.001f) {
            return;
        }
        mStep = tstep;
        mVelocity = GetPosition() - mLastPos;
        if(mUnderwater) {
            ChangeState(MOVE_SWIMMING_SURFACE);
        }
        else {
            ChangeState(MOVE_GROUNDED);
        }
        if(!mIgnorePhysics && !mFollowingPath) {
            UpdatePlatform();
            Accelerate();
            Fall();
            Float();
            Decelerate();
            Settle();
            VerletNoVelocity(tstep);
        }
        else if(mFollowingPath) {
            mVelocity.x = 0;
            mVelocity.y = 0;
            mAppliedForce.x = 0;
            mAppliedForce.y = 0;
            mPath.Update();
            mVelocity.x = mPath.GetPosition().x - mHitBox.GetOrigin().x;
            mVelocity.y = mPath.GetPosition().y - mHitBox.GetOrigin().y;
        }
        ResetFlags();
        mBoundCollider = NULL;
        mGroundedCheck++;
    }
    void PhysicsBox2f::UpdateVerlet(float dt) {
        mStep = dt;
        Accelerate();
        Fall();
        Float();
        Decelerate();
        Settle();
        if(mUnderwater) {
            mLastAngle = mHitBox.GetAngle();
            if(mAppliedForce.y < 0) {
                mAppliedForce*= mWaterDamping;
            }
        }
        mVelocity = GetPosition() - mLastPos;
        VerletNoVelocity(dt);
        ResetFlags();
    }
    void PhysicsBox2f::UpdatePhysics() {
        Accelerate();
        Decelerate();
        Fall();
        Float();
        Settle();
        ApplyForces();
    }
    void PhysicsBox2f::Settle() {
    }
    void PhysicsBox2f::MoveTo(Motion mv) {
        mPath = mv;
        mFollowingPath = true;
    }
    void PhysicsBox2f::CheckPlatform(Collision& c, BackgroundTile2& bg) {
        if(mPassThroughPlatforms || mNoPlatform) {
            return;
        }
        Rectanglef hitbox = bg.GetHBox()->GetAABB();
        if(bg.GetSprings()) {
            if(mVelocity.y <= 0) {
                if(mHitBox.GetBottom() >= hitbox.GetBottom()) {
                    if(!mBoundCollider) {
                        mBoundCollider = &bg;
                    }
                    if(!bg.Springing()) {
                        CalcTorque(mVelocity, c, bg);
                    }
                    mVelocity.y = 0.0f;
                    //mHitBox.SetBottom(bg.GetHitBox().GetTop());
                    mHitBox.SetBottom(hitbox.GetTop());
                    mLastPos.y = GetPosition().y;
                    mGrounded = true;
                }
            }
        }
        else if(mHitBox.GetBottom() >= hitbox.GetBottom() && mVelocity.y <= 0) {
            mGrounded = true;
            float bottom = mHitBox.GetBottom();
            float bottom2 = bg.GetHitBox().GetBottom();
            float y = mHitBox.GetBottom() - hitbox.GetBottom();
            //mHitBox.SetBottom(bg.GetHitBox().GetTop());
            mHitBox.SetBottom(hitbox.GetTop());
            mLastPos.y = GetPosition().y;
            if(!mBoundCollider) {
                mBoundCollider = &bg;
            }
        }
    }
    bool PhysicsBox2f::CheckCollision(WaterBody& waterBody) {
        float depth = waterBody.CheckCollision(GetHitBoxShape());
        if(depth > 0.0f) {
            if(!mUnderwater) {
                if(mSubmergeDepth < mHitBox.GetHeight() / 2) {
                    Vector2f velocity = mHitBox.GetPosition() - mLastPos;
                    if(velocity.y < 0) {
                        waterBody.Splash(mHitBox, mVelocity.y * 2.0);
                        waterBody.Splash(mHitBox, mVelocity.y * 2.0);
                        mLastPos = mHitBox.GetPosition() - (mHitBox.GetPosition() - mLastPos) * mBuoyancyDamping;
                    }
                    else {
                        waterBody.Splash(mHitBox, mVelocity.y * 0.25);
                    }
                }
                mUnderwater = true;
            }
            mSubmergeDepth = depth;
        }
        return depth > 0.0f;
    }
    Rectanglef PhysicsBox2f::GetCollisionBox() {
        Vector2f origin = mHitBox.GetBotLeft();
        float height = mHitBox.GetHeight() + fabs(mVelocity.y);
        float width = mHitBox.GetWidth() + fabs(mVelocity.x);
        if(mVelocity.x < 0) {
            origin.x-= fabs(mVelocity.x);
        }
        if(mVelocity.y < 0) {
            origin.y-= fabs(mVelocity.y);
        }
        return Rectanglef(origin, height, width);
    }
}
