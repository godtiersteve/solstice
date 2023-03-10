#include "bullet_missile.h"

namespace solstice {
    Missile::Missile() {
        mLockOnStartVel = 0;
        mTrackingSpeed = 0;
        mExplosionRadius = 75;
        mLockOnRadius = 200;
        mTracksTarget = false;
        mLocksOn = true;
        mTargetFound = false;
        mTracksTarget = true;
        mAlive = true;
        mImpactForce = Vector2f(30, 30);
        mCollisionType = 0;
        mExplosionTimer = 0;
        mMissileAcceleration = 1.0;
        mType = BTYPE_MISSILE;
        mBlastForce = 30;
    }
    Missile::Missile(float speed, Vector2f direction, float acceleration, float blastRadius, float blastForce) {
        mVelocity = glm::vec3(speed * direction.x, speed * direction.y, 0.0);;
        mAcceleration = glm::vec3(acceleration * direction.x, acceleration * direction.y, 0.0);
        mImpactForce = blastForce;
        mExplosionRadius = blastRadius;
        mLockOnRadius = 30;
        mAlive = true;
        mMissileAcceleration = acceleration;
        SaveInitialState();
        mCollidesWidth = PDESTROY_OBJECT | PDESTROY_WALL;
        mType = BTYPE_MISSILE;
    }
    void Missile::AimTowards(Vector2f pos) {
        return;
        if(!mTargetFound || (mTargetFound && mTracksTarget)) {
            mLockOnPoint = pos;
            pos = pos - GetHitBoxPos();
            float vel = mLockOnStartVel;
            if(vel < 0.5) {
                vel = glm::length(mVelocity);
            }
            if(vel > mMaxSpeed.x) {
                vel = mMaxSpeed.x;
            }
            float angle = atan2(pos.y, pos.x);
            float dirAngle = atan2(mVelocity.y, mVelocity.x);
            angle = DeltaAngle(dirAngle, angle);
            float angleAdjust = dirAngle + GetTrackedAngle(angle);
            mLockOnDir = Vector2f(cos(angle), sin(angle));
            mVelocity = glm::vec3(mLockOnDir.x, mLockOnDir.y, 0.0f) * vel;
            mTargetFound = true;
        }
    }
    float Missile::GetTrackedAngle(float a) {
        if(mTrackingSpeed < 0) {
            if(a < mTrackingSpeed) {
                a = mTrackingSpeed;
            }
            else if(a > -mTrackingSpeed) {
                a = -mTrackingSpeed;
            }
        }
        else {
            a*= mTrackingSpeed;
        }
        return a;
    }
    void Missile::Update(double dt) {
        if(mCollisionType) {
            Destroy();
        }
        if(mAlive) {
            mAcceleration = glm::normalize(mVelocity) * mMissileAcceleration;
        }
        BulletBase::Update(dt);
        if(mExplosionTimer) {
            mExplosionTimer--;
        }
    }
    void Missile::Explode(Vector2f force) {
    }
    void Missile::CheckExplosion(RigidBody* obj) {
        if(Exploding()) {
            Circlef blastRadius = Circlef(GetHitBoxPos(), mExplosionRadius);
            Collision c = CheckCollision(&blastRadius, &obj->GetHitBox());
            if(c) {
                Vector2f impact = obj->GetHitBox().GetCenter() - blastRadius.GetCenter();
                impact = impact.GetUnit();
                impact = impact * mBlastForce;
                obj->ApplyImpulseY(impact.y);
            }
        }
    }
    bool Missile::Exploding() {
        return mExplosionTimer;
    }
    bool Missile::CheckAndResolveCollision(BackgroundTile2& ground) {
        bool col = BulletBase::CheckAndResolveCollision(ground);
        if(col && !mExplosionTimer) {
            mExplosionTimer = 3;
        }
    }
    bool Missile::CheckAndResolveCollision(RigidBody* b) {
        if(!b) {
            return false;
        }
        bool colFound = false;
        if(mCollidesWidth & PDESTROY_OBJECT) {
            if(mAlive) {
                Collision c = CheckCollision(&mHitBox, &b->GetHitBox());
                if(c) {
                    mCollisionType|= PDESTROY_OBJECT;
                    if((mDestroyedBy & mCollisionType)) {
                        Destroy(Vector2f(mVelocity.x, mVelocity.y));
                    }
                    Vector2f impact = Vector2f(mVelocity.x, mVelocity.y).Length() * mImpactForce.x;
                    if(mVelocity.x < 0) {
                        impact.x = -impact.x;
                    }
                    if(mVelocity.y < 0) {
                        impact.y = -impact.y;
                    }
                    b->ApplyImpulse(impact);
                    ResolveCollision(c, b->GetFriction());
                    mCollisionFound = true;
                    colFound = true;
                }
                if(mLocksOn && (!mTargetFound || (mTargetFound && mTracksTarget))) {
                    Circlef lockon(GetHitBoxPos(), mLockOnRadius);
                    Collision c = CircleRectCollision(lockon, b->GetHitBox());
                    if(c) {
                        AimTowards(b->GetHitBox().GetCenter());
                        colFound = true;
                    }
                }
            }
            else if(mExplosionTimer) {
                Circlef blastRadius = Circlef(GetHitBoxPos(), mExplosionRadius);
                Collision c = CheckCollision(&blastRadius, &b->GetHitBox());
                if(c) {
                    mCollisionType|= PDESTROY_OBJECT;
                    if((mDestroyedBy & mCollisionType)) {
                        Destroy(Vector2f(mVelocity.x, mVelocity.y));
                    }
                    Vector2f impact = Vector2f(mVelocity.x, mVelocity.y).Length() * mImpactForce.x;
                    if(mVelocity.x < 0) {
                        impact.x = -impact.x;
                    }
                    if(mVelocity.y < 0) {
                        impact.y = -impact.y;
                    }
                    b->ApplyImpulse(impact);
                    ResolveCollision(c, b->GetFriction());
                    mCollisionFound = true;
                    colFound = true;
                }
            }
        }
        return colFound;
    }
    void Missile::Reset() {
        mExplosionTimer = 0;
        BulletBase::Reset();
    }
    bool Missile::Finished() {
        bool finished = BulletBase::Finished();
        if(finished && mExplosionTimer > 0) {
            finished = false;
        }
        if(finished) {
            if(!mEmitter.Finished()) {
                finished = false;
            }
        }

        return finished;
    }

}
