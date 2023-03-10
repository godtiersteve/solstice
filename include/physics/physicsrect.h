#ifndef PHYSICSRECT_H_INCLUDED
#define PHYSICSRECT_H_INCLUDED

#include <vector>
#include <deque>
#include <algorithm>
#include <stdio.h>

#include "geometry/rectangle.h"
#include "motion2d.h"
#include "physics/rigidbody.h"
#include "physics/physics.h"
#include "geometry/collision.h"
#include "geometry/geometry.h"
#include "utility/sol_math.h"
#include "environment/backgroundtile.h"
#include "environment/waterbody.h"
// TODO : Add reversable gravity (determined by if gravitational acceleration is positive or negative)

namespace solstice {

enum CollisionType {COL_BACKGROUNDS = 0, COL_SELF = 2, COL_OTHERS = 4};
class PhysicsBox2f : public RigidBody {
    protected:
        vector<PhysicsBox2f*> mCollidersMovable;
        vector<BackgroundTile2*> mCollidersBG;
        BackgroundTile2* mBoundCollider;
        Vector2f mDeceleration;
        Vector2f mMaxVelocity;
        bool mIgnorePhysics;
        bool mPassThroughPlatforms;
        bool mFollowingPath;

        Motion mPath;
        float mGravityAccel;
        float mWaterDamping;
        float mDepthModifier, mBuoyancyDamping;
        bool mNoGravity, mNoCollision;

        void Float();
        void Settle();
        void UpdatePhysics();
        void AdjustVelocity();
        void AdjustVelocity(Vector2f& vel);
        void AdjustAngle(float& angle);
        void UpdatePlatform();
    public:
        PhysicsBox2f();
        PhysicsBox2f(Vector2f accel, Vector2f decel, Vector2f maxVel);
        PhysicsBox2f(const Rectanglef& hitbox, Vector2f accel, Vector2f decel, Vector2f maxVel);

        void Init();
        void Init(Vector2f accel, Vector2f decel, Vector2f maxVel);
        void Init(const Rectanglef&  hitbox, Vector2f accel, Vector2f decel, Vector2f maxVel);
        void ClearColliders();

        void SetVelocity(Vector2f vel)      {mVelocity = vel;}
        void SetAcceleration(Vector2f accel);
        void SetAccelerationX(float a);
        void SetAccelerationY(float a);
        void SetDeceleration(Vector2f decel);
        void SetDecelerationX(float a);
        void SetDecelerationY(float a);
        void SetMaxVelocity(Vector2f maxVel);
        void SetGravityAccel(float gravAccel);
        void SetForce(Vector2f force);
        void SetDepthModifier(float depth)      {mDepthModifier = depth;}
        void SetBuoyancyDamping(float damp)     {mBuoyancyDamping = damp;}
        void SetNoGravity(bool nograv)          {mNoGravity = nograv;}
        void SetIgnorePhysics(bool in)          {mIgnorePhysics = in;}
        void SetFollowingPath(bool path)        {mFollowingPath = path;}
        void SetPath(Motion m)                  {mPath = m;}
        void SetWaterDamping(float damping)     {mWaterDamping = damping;}
        void SetNoCollision(bool c)             {mNoCollision = c;}
        void SetPosition(Vector2f pos)          {mHitBox.SetPosition(pos); mLastPos = pos;}
        bool GetNoGravity()                 {return mNoGravity;}
        Vector2f GetForce()                 {return mAppliedForce;}
        Vector2f GetDeceleration();
        Vector2f GetMaxVelocity();
        float GetTorque();
        bool GetFalling();
        float GetGravityAccel();
        bool GetNoCollision()           {return mNoCollision;}
        float GetDepthModifier()        {return mDepthModifier;}
        float GetBuoyancyDamping()      {return mBuoyancyDamping;}
        bool IsMoving()                 {return mVelocity.x != 0 || mVelocity.y != 0;}
        bool GetIgnorePhysics()         {return mIgnorePhysics;}
        bool GetFollowingPath()         {return mFollowingPath;}
        Motion& GetPath()               {return mPath;}
        float GetWaterDamping()         {return mWaterDamping;}
        float GetArea()                 {return mHitBox.GetArea();}
        float GetHeight()               {return mHitBox.GetHeight();}
        float GetWidth()                {return mHitBox.GetWidth();}

        void SetVelocityX(float v);
        void SetVelocityY(float v);
        virtual void AddVelocityX(float x);
        virtual void AddVelocityY(float y);
        virtual void AddVelocity(Vector2f velocity);
        virtual void Accelerate();
        virtual void Decelerate();
        virtual void Fall(float vel = 0);
        virtual void MoveTo(Motion mv);
        virtual void Move(float tstep = 1.0f);
        virtual void Shoot();
        virtual void Shoot(Vector2f velocity);
        virtual void Update(float tstep = 1.0f);
        virtual void UpdateVerlet(float dt);
        virtual bool CheckCollision(WaterBody& waterBody);
        virtual void CheckPlatform(Collision& c, BackgroundTile2& bg);
        void ResetFlags() {
            mSubmergeDepth = 0;
            mUnderwater = false;
            mGrounded = false;
        }
        Rectanglef GetCollisionBox();
};
}



#endif // PHYSICSRECT_H_INCLUDED
