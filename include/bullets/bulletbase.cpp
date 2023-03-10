#include "bulletbase.h"

namespace solstice {
    BulletBase::BulletBase() {
        mCollisionType = 0;
        mCollidesWidth = PDESTROY_GROUND | PDESTROY_ENEMY | PDESTROY_OBJECT;
        mDestroyedBy = mCollidesWidth | PDESTROY_WALL;
        mType = BTYPE_PROJECTILE;
        mDestroyAnimsFinished = false;
        mID = -1;
        mAlive = true;
        mFlipSprite = true;
        mRotatesSprite = false;
    }
    void BulletBase::Update(double dt) {
        if(mAlive) {
            Particle::Update(dt);
        }
        if(mRotatesSprite) {
            mSpriteAngle = atan2(mVelocity.y, mVelocity.x);
        }
        SetPosition(Vector2f(mPosition.x, mPosition.y));
        UpdateAnimation(dt);
        Vector2f center = mHitBox.GetCenter();
        if(mTransfersPosition) {
            mEmitter.SetPosition(glm::vec3(center.x, center.y, 0.0f));
        }
        if(mTransfersVelocity) {
            mEmitter.AddVelocity(glm::vec3(mVelocity.x, mVelocity.y, 0.0));
        }
        Vector2f offset = mSprayerOffset;
        if(offset.Magnitude2() >= 1) {
            offset.Rotate(mSpriteAngle);
        }
        mEmitter.Update(dt, mPosition + glm::vec3(offset.x, offset.y, 0));
        if(mEmitsOnImpact && !mAlive) {
            mImpactEmitter.Update(dt, mPosition + glm::vec3(offset.x, offset.y, 0));
        }
    }
    void BulletBase::SetState(BulletState b) {
        SetPosition(b.mPosition);
        mVelocity = b.mVelocity;
        mAlive = true;
        mCollisionType = 0;
    }
    void BulletBase::SaveInitialState() {
        mResetState.mPosition = GetHitBoxPos();
        mResetState.mVelocity = mVelocity;
    }
    void BulletBase::SetCollisionPoint(Vector2f point) {
        mCollisionPoint = point;
        if(mEmitsOnImpact) {
            mImpactEmitter.SetPosition(glm::vec3(point.x, point.y, 0.0));
        }
        mCollisionFound = true;
    }
    void BulletBase::UpdateAnimation(float dt) {
        if(mAlive) {
            for(size_t i = 0; i < mAnimations.size(); i++) {
                if(mFlipSprite) {
                    if(mVelocity.x < 0) {
                        mAnimations[i].SetHoriFlip(true);
                    }
                    else {
                        mAnimations[i].SetHoriFlip(false);
                    }
                }
                mAnimations[i].SetPosition(GetHitBoxPos());
                if(mRotatesSprite) {
                    mAnimations[i].SetAngle(mSpriteAngle + mAnimations[i].mBaseAngle);
                }
                mAnimations[i].SetPosition(GetHitBoxPos());
                mAnimations[i].Update(dt);
            }
        }
        else if(mShot) {
            mDestroyAnimsFinished = true;
            for(size_t i = 0; i < mDestroyAnimations.size(); i++) {
                if(mFlipSprite) {
                    if(mVelocity.x < 0) {
                        mDestroyAnimations[i].SetHoriFlip(true);
                    }
                    else {
                        mDestroyAnimations[i].SetHoriFlip(false);
                    }
                }
                mDestroyAnimations[i].SetPosition(GetHitBoxPos());
                if(mRotatesSprite) {
                    mDestroyAnimations[i].SetAngle(mSpriteAngle + mDestroyAnimations[i].mBaseAngle);
                }
                mDestroyAnimations[i].SetPosition(GetHitBoxPos());
                mDestroyAnimations[i].Update(dt);
                if(!mDestroyAnimations[i].Finished()) {
                    mDestroyAnimsFinished = false;
                }
            }
        }
    }
    void BulletBase::SetPosition(Vector2f pos) {
        mHitBox.SetPosition(pos);
        mCircleHitBox.SetPosition(pos);
        mPosition = glm::vec3(pos.x, pos.y, 0.0);
        for(size_t i = 0; i < mAnimations.size(); i++) {
            mAnimations[i].SetPosition(pos);
        }
        for(size_t i = 0; i < mDestroyAnimations.size(); i++) {
            mDestroyAnimations[i].SetPosition(pos);
        }
    }
    void BulletBase::SetPosition(glm::vec3 pos) {
        mHitBox.SetPosition(Vector2f(pos.x, pos.y));
        mCircleHitBox.SetPosition(Vector2f(pos.x, pos.y));
        mPosition = pos;
        for(size_t i = 0; i < mAnimations.size(); i++) {
            mAnimations[i].SetPosition(Vector2f(pos.x, pos.y));
        }
        for(size_t i = 0; i < mDestroyAnimations.size(); i++) {
            mDestroyAnimations[i].SetPosition(Vector2f(pos.x, pos.y));
        }
    }
    void BulletBase::Shoot(Vector2f velocity) {
        Reset();
        float x = mInitialState.velocity.x;
        float y = mInitialState.velocity.y;
        if(velocity.x < 0) {
            x = -x;
        }
        mVelocity.x = velocity.x + x;
        mVelocity.y = velocity.y + mInitialState.velocity.y;
        mAlive = true;
        mDestroyed = false;
        mShot = true;
        UpdateAnimation(0);
    }
    void BulletBase::Shoot(Vector2f velocity, Vector2f position) {
        Shoot(velocity);
        mPosition = glm::vec3(position.x, position.y, 0.0);
        SetPosition(position);
    }
    void BulletBase::ShootAngle(float angle, float vel) {
        Reset();
        mAngle = angle;
        mAngleVel = vel;
        mAlive = true;
        mShot = true;
    }
    void BulletBase::Destroy(Vector2f force) {
        if(!mAlive) {
            return;
        }
        mAlive = false;
        mDestroyed = true;
        for(size_t i = 0; i < mDestroyAnimations.size(); i++) {
            mDestroyAnimations[i].Reset();
            mDestroyAnimations[i].SetPosition(GetHitBoxPos());
        }
        if(mEmitsOnImpact) {
            mImpactEmitter.Reset();
            if(mDirectsImpact) {
                mImpactEmitter.SetInfluenceAngle(-force);
            }
            if(mCollisionFound) {
                mImpactEmitter.SetPosition(glm::vec3(mCollisionPoint.x, mCollisionPoint.y, 0.0));
            }
            else {
                mImpactEmitter.SetPosition(mPosition);
            }
        }
        mImpactEmitter.DeactivateSprayers();
        mEmitter.DeactivateSprayers();
    }
    bool BulletBase::CheckAndResolveCollision(BackgroundTile2& ground) {
        if(ground.GetType() == TILE_PLATFORM) {
            return false;
        }
        if(mAlive && (mCollidesWidth & PDESTROY_WALL) || (mCollidesWidth & PDESTROY_GROUND)) {
            Collision c = CheckCollision(&mHitBox, &ground.GetHitBox());
            if(c) {
                mCollisionFound = true;
                SetCollisionPoint(c.GetContactPoint(mHitBox, ground.GetHitBox()));
                if(fabs(c.normal.x) >= 0.75) {
                    mCollisionType|= PDESTROY_WALL;
                }
                if(fabs(c.normal.y) >= 0.25) {
                    mCollisionType|= PDESTROY_GROUND;
                }
                if((mDestroyedBy & mCollisionType)) {
                    Destroy(Vector2f(mVelocity.x, mVelocity.y));
                }
                else {
                    ResolveCollision(c, ground.GetFriction());
                }
                return true;
            }
        }
        return false;
    }
    bool BulletBase::CheckAndResolveCollision(RigidBody* b) {
        if(!b) {
            return false;
        }
        bool colFound = false;
        if(mAlive && (mCollidesWidth & PDESTROY_OBJECT)) {
            Collision c = CheckCollision(&mHitBox, &b->GetHitBox());
            if(c) {
                mCollisionType|= PDESTROY_OBJECT;
                SetCollisionPoint(c.GetContactPoint(mHitBox, b->GetHitBox()));
                if((mDestroyedBy & mCollisionType)) {
                    Destroy(Vector2f(mVelocity.x, mVelocity.y));
                }
                Vector2f impact = mImpactForce;
                if(mVelocity.x < 0) {
                    impact.x = -impact.x;
                }
                if(mVelocity.y < 0) {
                    impact.y = -impact.y;
                }
                b->ApplyImpulse(impact);
                mCollisionFound = true;
                ResolveCollision(c, b->GetFriction());
                colFound = true;
            }
        }
        return colFound;
    }
    void BulletBase::ResetAnimations() {
        for(size_t i = 0; i < mAnimations.size(); i++) {
            mAnimations[i].Reset();
        }
        for(size_t i = 0; i < mDestroyAnimations.size(); i++) {
            mDestroyAnimations[i].Reset();
        }
        mDestroyAnimsFinished = false;
    }
    bool BulletBase::CheckAndResolveCollision(WaterBody& b) {
        return false;
    }
    void BulletBase::ResolveCollision(Collision& c, float friction) {
        if(c && mAlive) {
            mHitBox.AddXY(c.mtv);
            Vector2f vel(mVelocity.x, mVelocity.y);
            Vector2f normalVel = c.normal.Dot(vel) * c.normal;
            Vector2f tangentVel = vel - normalVel;
            normalVel.x*= -(1 + mElasticityX);
            normalVel.y*= -(1 + mElasticityY);
            friction = 0;
            vel = tangentVel * -friction + normalVel;
            mVelocity.x+= vel.x;
            mVelocity.y+= vel.y;
            mCollisionFound = true;
        }
    }
    bool BulletBase::Finished() {
        return mDestroyAnimations.empty() ? !mAlive : !mAlive && mDestroyAnimsFinished;
    }
    void BulletBase::Draw(SceneRenderer& rend) {
        if(mAlive) {
            int numAnims = mAnimations.size();
            for(size_t i = 0; i < mAnimations.size(); i++) {
                rend.DrawQuad(mAnimations[i].GetDrawQuad());
            }
        }
        else if(mShot) {
            for(size_t i = 0; i < mDestroyAnimations.size(); i++) {
                if(!mDestroyAnimations[i].Finished()) {
                    rend.DrawQuad(mDestroyAnimations[i].GetDrawQuad());
                }
            }
        }
    }
    void BulletBase::DrawHitBoxes(SceneRenderer& rend, Color col) {
        rend.DrawRect(mHitBox, col);
    }
    void BulletBase::AimTowards(Vector2f pos) {
    }
    void BulletBase::Reset()  {
        SetState(mResetState);
        mCollisionType = 0;
        ResetAnimations();
        mImpactEmitter.ActivateSprayers();
        mEmitter.ActivateSprayers();
    }
}
