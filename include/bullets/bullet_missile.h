#ifndef BULLET_MISSILE_H_INCLUDED
#define BULLET_MISSILE_H_INCLUDED

#include "bulletbase.h"

namespace solstice {
    class Missile : public BulletBase {
    protected:
        Vector2f mLockOnPoint, mLockOnDir;
    public:
        float mLockOnStartVel;
        float mTrackingSpeed;
        float mExplosionRadius, mLockOnRadius;
        float mMissileAcceleration;
        bool mTracksTarget, mLocksOn, mTargetFound;
        int mExplosionTimer;
        Vector2f mTargetPosition;

        Missile();
        Missile(float speed, Vector2f direction, float acceleration, float blastRadius, float blastForce);

        float GetTrackedAngle(float a);

        bool CheckAndResolveCollision(RigidBody* b);
        bool CheckAndResolveCollision(BackgroundTile2& ground);
        bool CheckAndResolveCollision(WaterBody& b) {return BulletBase::CheckAndResolveCollision(b);}
        void AimTowards(Vector2f pos);
        Circlef GetLockOnCircle() {return Circlef(GetHitBoxPos(), mLockOnRadius);}
        Circlef GetBlastCircle() {return Circlef(GetHitBoxPos(), mExplosionRadius);}
        void Update(double dt);
        virtual void Explode(Vector2f force);
        virtual void Reset();
        bool Finished();
        bool Exploding();
        bool Explodes() {return true;}
        void CheckExplosion(RigidBody* obj);
    };

}


#endif // BULLET_MISSILE_H_INCLUDED
