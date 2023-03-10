#include "bullets/bullet.h"

namespace solstice {
    Bullet::Bullet() {
        mAlive = false;
        mShot = false;
        mTransfersVelocity = false;
        mTransfersPosition = false;
        mEmitsOnImpact = false;
        mMaxBounces = 0;
        mBounces = 0;
        mElasticityX = 0.0f;
        mElasticityY = 0.0f;
        mLocksOn = false;
        mTargetFound = false;
        mAdjustsAngle = false;
        mLockOnVel = 0.0f;
        mUpdateFunc = nullptr;
        mCollisionFunc = nullptr;
        mRotatesSprite = false;
        mCollisionFound = false;
        mLockOnStartVel = 0;
        mAnimations.resize(2);
        mBlastForce = 0.0f;
        mDirectsImpact = false;
        mSpriteAngle = 0.0;
        mTrackingSpeed = 1.0f;
        mType = BTYPE_PROJECTILE;
        mWaterStops = false;
        mCollisionType = 0;
        mBouncesTowards = false;
        mAutoDeathDur = -1;
        mCreatesSubBullets = false;
        mCreateSubState = -1;
        mActionDelay = 0;
        mSubsToMake = 0;
        mSubsPerShot = 0;
        mSubCreateFrameCur = 0;
        mSubCreateFrame = -1;
        mSubsFollowTarget = false;
        mSecondShotType = NULL;
        mSecondShotFrame = 0;
        mDamagesSelf = false;
        mKnockbacksSelf = false;
        mDestroyedBy = PDESTROY_BACKGROUND;
    }
    void Bullet::Update(float dt) {
        if(!mUpdateFunc) {
            DefaultUpdate(dt);
        }
        else {
            mUpdateFunc(this, dt);
        }
    }
    void Bullet::DefaultUpdate(float dt) {
        if(mAlive) {
            if(mAutoDeathDur > 0 && mCurFrame >= mAutoDeathDur) {
                Destroy();
            }
            else if(mMaxBounces) {
                if(mCollisionFound) {
                    if(mCollisionType & mCollidesWidth) {
                        mBounces++;
                    }
                }
                if(mBounces > mMaxBounces) {
                    Destroy();
                }
            }
            if(mSubCreateFrame >= 0) {
                mSubCreateFrameCur++;
            }
            if(!mCollisionFound && mTargetFound && mAdjustsAngle) {
                mTargetFound = false;
            }
            if(mTargetFound && glm::length(mVelocity) < glm::length(mMaxSpeed)) {
                mLockOnVel+= mLockOnAccel;
                mVelocity = glm::vec3(mLockOnDir.x, mLockOnDir.y, 0.0f) * mLockOnVel;
            }
            if(mTargetFound) {
                AimTowards(mLockOnPoint);
            }
            Particle::Update(dt);
            mHitBox.AddXY(mVelocity.x, mVelocity.y);
            mHitBox.SetAngle(mAngle);
            mPosition.x = mHitBox.GetPosition().x;
            mPosition.y = mHitBox.GetPosition().y;
            Vector2f center = mHitBox.GetCenter();
            if(mTransfersPosition) {
                mEmitter.SetPosition(glm::vec3(center.x, center.y, 0.0f));
            }
            if(mTransfersVelocity) {
                mEmitter.AddVelocity(glm::vec3(mVelocity.x, mVelocity.y, 0.0));
            }
            if(mType == BTYPE_LASER) {
                mEmitter.SetInfluenceAngle(Vector2f(cos(mAngle), sin(mAngle)));
            }
            mEmitter.Update(dt, mPosition);
            mLockOnRadius.SetPosition(center);
        }
        else if(mEmitsOnImpact && mShot) {
            if(mType == BTYPE_PROJECTILE) {
                if(mCollisionFound) {
                    mImpactEmitter.SetPosition(glm::vec3(mCollisionPoint.x, mCollisionPoint.y, 0.0f));
                }
                if(mTransfersVelocity) {
                    mImpactEmitter.AddVelocity(glm::vec3(mVelocity.x, mVelocity.y, 0.0));
                }
                mImpactEmitter.Update(dt);
            }
        }
        if(mType == BTYPE_LASER && mCollisionFound) {
            mImpactEmitter.Update(dt);
            if(mCollisionFound) {
                mImpactEmitter.SetPosition(glm::vec3(mCollisionPoint.x, mCollisionPoint.y, 0.0));
            }
        }
        if(mCollisionFound) {
            mBlastRadius.SetPosition(mCollisionPoint);
        }
        else {
            mBlastRadius.SetPosition(mHitBox.GetCenter());
        }
        if(mShot && mRotatesSprite) {
            RotateSprite();
        }
        UpdateAnimation(dt);
        if(mAlive) {
            mCollisionFound = false;
            mCollisionType = 0;
        }
        for(size_t i = 0; i < mSubBullets.size(); i++) {
            if(mSubBullets[i]) {
                if(!(*mSubBullets[i]).Alive()) {
                    mSubBullets[i] = NULL;
                }
            }
        }
    }
    void Bullet::RotateSprite() {
        if(mType == BTYPE_PROJECTILE && mRotatesSprite) {
            mSpriteAngle = atan2(mVelocity.y, mVelocity.x);
        }
    }
    void Bullet::DefaultResolveCollision(Collision& c, float friction) {
        if(c && mAlive) {
            mHitBox.AddXY(c.mtv);
            Vector2f vel(mVelocity.x, mVelocity.y);
            Vector2f normalVel = c.normal.Dot(vel) * c.normal;
            Vector2f tangentVel = vel - normalVel;
            normalVel.x*= -(1 + mElasticityX);
            normalVel.y*= -(1 + mElasticityY);
            if(mTargetFound && mBouncesTowards) {
                Vector2f dir = GetHitBoxPos() - mLockOnPoint;
                float angle = atan2(dir.y, dir.x);
                normalVel = Vector2f(cos(angle) * mElasticityX, sin(angle) * mElasticityY) * normalVel.Magnitude();
            }
            friction = 0;
            vel = tangentVel * -friction + normalVel;
            mVelocity.x+= vel.x;
            mVelocity.y+= vel.y;
            mCollisionFound = true;
        }
    }
    void Bullet::SetCollisionPoint(Vector2f point) {
        mCollisionPoint = point;
        if(mEmitsOnImpact) {
            mImpactEmitter.SetPosition(glm::vec3(point.x, point.y, 0.0));
        }
        mCollisionFound = true;
    }
    Vector2f Bullet::ResolveBlast(Vector2f point) {
        Vector2f diff = point - mBlastRadius.GetCenter();
        float dist = diff.Length2();
        float r2 = mBlastRadius.GetRadius() * mBlastRadius.GetRadius() * 0.5;
        float attn = r2 / dist;
        Vector2f force = diff.GetUnit();
        return force * mBlastForce * attn;
    }
    void Bullet::ResolveCollision(Collision& c, float friction) {
        if(mType != BTYPE_PROJECTILE) {
            return;
        }
        if(mCollisionFunc) {
            mCollisionFunc(this, c, friction);
        }
        else {
            DefaultResolveCollision(c, friction);
        }
    }
    void Bullet::DirectTowards(Vector2f pt) {
        if(mType == BTYPE_LASER) {
            float angle = atan2(pt.y, pt.x);
            angle = GetTrackedAngle(DeltaAngle(mAngle, angle));
            mAngle = mAngle + angle;
            mSpriteAngle = mAngle + angle;
        }
        else {
            mAngle = atan2(pt.x, pt.y);
        }

    }
    void Bullet::AimTowards(Vector2f pos) {
        if(mSubsFollowTarget) {
            for(size_t i = 0; i < mSubBullets.size(); i++) {
                if(mSubBullets[i]) {
                    mSubBullets[i]->AimTowards(pos);
                }
            }
        }
        if(mActionDelay <= mCurFrame) {
            if(mType == BTYPE_PROJECTILE && mLocksOn) {
                if(!mTargetFound || (mTargetFound && mAdjustsAngle)) {
                    mLockOnPoint = pos;
                    if(!mBouncesTowards) {
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
                        mLockOnDir = Vector2f(cos(angleAdjust), sin(angleAdjust));
                        mVelocity = glm::vec3(mLockOnDir.x, mLockOnDir.y, 0.0f) * vel;
                        mTargetFound = true;
                    }
                }
            }
            else if(mType == BTYPE_LASER) {
                pos = pos - GetHitBoxPos();
                float angle = atan2(pos.y, pos.x);
                angle = GetTrackedAngle(DeltaAngle(mAngle, angle));
                mAngle = mAngle + angle;
                mSpriteAngle = mAngle + angle;
            }
        }
    }
    float Bullet::GetTrackedAngle(float a) {
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

    void Bullet::Reset() {Reset(Vector2f(mInitialState.position.x, mInitialState.position.y));}
    void Bullet::AddSubBullet(Bullet* b) {
        for(size_t i = 0; i < mSubBullets.size(); i++) {
            if(mSubBullets[i] == NULL) {
                mSubBullets[i] = b;
                return;
            }
        }
        mSubBullets.push_back(b);
    }
    void Bullet::Reset(Vector2f pos) {
        Particle::Reset();
        mEmitter.Reset();
        mSubBullets.clear();
        mSubBullets.reserve(mSubsPerShot + 1);
        mAlive = false;
        mShot = false;
        mTargetFound = false;
        mCollisionFound = false;
        mCreateSubState = -1;
        mSubsToMake = mSubsPerShot;
        mLockOnPoint = 0;
        mLockOnDir = 0;
        mBounces = 0;
        SetPosition(pos);
        mVelocity.x = mInitialState.velocity.x;
        mVelocity.y = mInitialState.velocity.y;
        mLockOnRadius.SetPosition(GetHitBoxPos());
        mBlastRadius.SetPosition(GetHitBoxPos());

        mTargetFound = false;
        mAnimation.Reset();
        for(auto& anim : mAnimations) {
            anim.Reset();
            anim.SetPosition(pos);
        }
        if(mRotatesSprite) {
            if(mVelocity.x < 0) {
                SetHoriFlip(true);
            }
            else {
                SetHoriFlip(false);
            }
        }
        else {
            if(mVelocity.x < 0) {
                SetVertFlip(true);
            }
            else {
                SetVertFlip(false);
            }
        }
        UpdateAnimation(0);
    }
    void Bullet::InitHitBox(Vector2f pos, float h, float w, int drawFrom) {
        mHitBox.Init(pos, h, w, (DrawPoint)drawFrom);
    }
    void Bullet::SetScript(kaguya::State& state, std::string update, std::string collision) {
        mUpdateFunc = state[update];
        mCollisionFunc = state[collision];
        state["Init"](this);
    }
    void Bullet::SetScript(kaguya::State& state, std::string package) {
        mUpdateFunc = state[package]["Update"];
        mCollisionFunc = state[package]["Collide"];
        state[package]["Init"](this);
    }
    void Bullet::SetDestroyAnim(Animation anim) {
        if(!mAnimations.empty()) {
            mAnimations.back() = anim;
        }
    }
    void Bullet::AddAnimation(Animation anim) {
        if(mAnimations.size() >= 2) {
            mAnimations.insert(mAnimations.end() - 1, anim);
        }
        else {
            mAnimations.push_back(anim);
        }
    }
    void Bullet::SetAnimation(size_t i, Animation anim) {
        if(i < mAnimations.size()) {
            mAnimations[i] = anim;
        }
    }
    void Bullet::Draw(SceneRenderer& rend) {
        if(mAlive) {
            for(size_t i = 0; i < mAnimations.size() - 1; i++) {
                mAnimations[i].SetPosition(GetHitBoxPos());
                rend.DrawQuad(mAnimations[i].GetDrawQuad());
            }
        }
        else if(mShot && !mAnimations.back().Finished()) {
            //rend.DrawQuad(mAnimations.back().GetDrawQuad());
        }
    }
    bool Bullet::Finished() {
        if(mAlive) {
            return false;
        }
        else if(mDestroyed && mAnimations.back().Finished()) {
            if(!mEmitsOnImpact) {
                return true;
            }
            else {
                return mImpactEmitter.Finished();
            }
        }
        else {
            return true;
        }
    }
    void Bullet::SetHoriFlip(bool f) {
        for(auto& anim : mAnimations) {
            anim.SetHoriFlip(f);
        }
    }
    void Bullet::SetVertFlip(bool f) {
        for(auto& anim : mAnimations) {
            anim.SetVertFlip(f);
        }
    }
    void Bullet::SetRenderer(ParticleRenderer* r) {
        mEmitter.SetRenderer(r);
        mImpactEmitter.SetRenderer(r);
    }
    bool Bullet::CanCreateSub() {
        if(mCreateSubState == 1) {
            return true;
        }
        if(mSubsPerShot > 0 && mSubCreateFrame <= mSubCreateFrameCur) {
            return true;
        }
        return false;
    }
    bool Bullet::CreateSub() {
        if(CanCreateSub()) {
            if(mSubsToMake > 0) {
                mSubsToMake--;
                mSubCreateFrameCur = 0;
                return true;
            }
        }
        if(mSubsPerShot > 0 && mSubsToMake <= 0) {
            mCreateSubState = 2;
        }
        return false;
    }
    Vector2f Bullet::GetSubPos() {
        Vector2f ret = mSubPosition;
        int i = mSubsPerShot - (mSubsToMake + 1);
        if(mSubSpawnOffets.size() >= i + 1) {
            float angle = atan2(mSubDirection.y, mSubDirection.x);
            Vector2f pos = mSubSpawnOffets[i];
            pos.Rotate(angle);
            ret = ret + pos;
        }
        return ret;
    }
}
