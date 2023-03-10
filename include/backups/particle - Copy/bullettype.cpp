#include "bullettype.h"

namespace solstice {
BulletType::BulletType() {
    mState = NULL;
    mLastInactive = 0;
    mLastActive = 0;
    mShotsPerFrame = 1;
    mMultiShotAngleStart = 0;
    mMultiShotDirEnd = 0;
    mMultiShotDirStart = 0;
    mMultiShotAngleEnd = 0;
    mCreateFunc = NULL;
    mHoldShot = false;
    mMagicCost = 0;
    mSubBullet = NULL;
    mSubBulletsOnSpawn = false;
    mChargeable = false;
    mShootOnRelease = false;
    mChargeable;
}
BulletType::BulletType(Bullet base, int maxShots, float fireRate, float force, int type) {
    mLastInactive = 0;
    mLastActive = 0;
    mState = NULL;
    mMultiShotAngleStart = 0;
    mMultiShotDirEnd = 0;
    mMultiShotDirStart = 0;
    mMultiShotAngleEnd = 0;
    Bullet b;
    b.SetType(type);
    Init(b, maxShots, fireRate, force);
    InitBullets();
    mCreateFunc = NULL;
    mHoldShot = false;
    mMagicCost = 0;
    mSubBulletsOnSpawn = false;
    mChargeable = false;
    mShootOnRelease = false;
    mChargeable = false;
    mSubBullet = NULL;
}
BulletType::BulletType(kaguya::State* state, std::string package, int maxShots, float fireRate, float force, int type) {
    mLastInactive = 0;
    mLastActive = 0;
    mState = NULL;
    mMultiShotAngleStart = 0;
    mMultiShotDirEnd = 0;
    mMultiShotDirStart = 0;
    mMultiShotAngleEnd = 0;
    Bullet b;
    b.SetType(type);
    Init(b, maxShots, fireRate, force);
    SetState(state, package);
    mCreateFunc = NULL;
    mHoldShot = false;
    mName = package;
    mMagicCost = 0;
    mSubBulletsOnSpawn = false;
    mChargeable = false;
    mShootOnRelease = false;
    mChargeable = false;
    mSubBullet = NULL;
}
void BulletType::Init(Bullet base, int maxShots, float fireRate, float force, int type) {
    mBaseBullet = base;
    mMaxShots = maxShots;
    mFireRate = fireRate;
    mCurTime = fireRate;
    mShotForce = force;
    mShotsPerFrame = 1;
    mHoldShot = false;
}
void BulletType::Update(float dt) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i].Update(dt);
        if(mBaseBullet.GetType() == BTYPE_LASER) {
            mBullets[i].AimTowards(mTargetPosition);
        }
        if(mBullets[i].Finished() && i < mLastInactive) {
            mLastInactive = i;
        }
        if(mBullets[i].GetCreateSubState() == 1 && mBaseBullet.GetCreatesSubBullets() && mSubBullet) {
            float angle = mBullets[i].GetAngle() ? mBullets[i].GetAngle() : atan2(mBullets[i].GetVelocity().y, mBullets[i].GetVelocity().x);
            Vector2f dir = Vector2f(cos(angle), sin(angle));
            Vector2f pos = mBullets[i].GetHitBox().GetCenter();
            mSubBullet->CreateShot(dir, pos);
            mBullets[i].SetCreateSubState(2);
        }
    }
    if(mCurTime < mFireRate) {
        mCurTime+= dt;
    }
}
void BulletType::CheckCollisions(RigidBody* b) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i].CheckAndResolveCollision(b);
    }
}
void BulletType::CheckCollisions(BackgroundTile2& bg) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        if(mBullets[i].Alive()) {
            mBullets[i].CheckAndResolveCollision(bg);
        }
    }
}
void BulletType::Reset() {
    for(auto& bullet : mBullets) {
        bullet.Destroy();
        bullet.Reset();
    }
    mLastInactive = 0;
}
void BulletType::Draw(SceneRenderer& rend) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i].Draw(rend);
    }
}
void BulletType::DrawHitBoxes(SceneRenderer& rend, Color col) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        if(mBullets[i].Alive()) {
            rend.DrawRect(mBullets[i].GetHitBox(), col);
        }
    }
}
void BulletType::DrawTargetRanges(SceneRenderer& rend, int verts, Color col) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        if(mBullets[i].Alive()) {
            rend.DrawCircle(mBullets[i].GetLockOnRadius(), 32, col);
        }
    }
}
void BulletType::DrawBlastRadiuses(SceneRenderer& rend, int verts, Color col) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        if(mBullets[i].Alive()) {
            rend.DrawCircle(mBullets[i].GetBlastRadius(), verts, col);
        }
    }
}
void BulletType::DrawSprayers(ShaderProgram& sh, TextureArray& textures) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i].Draw(sh, textures);
    }
}
bool BulletType::CreateShot(Vector2f direction) {
    return CreateShot(direction, 0.0f, 0.0f);
}
bool BulletType::CreateShot(Vector2f direction, Vector2f position) {
    return CreateShot(direction, position, 0.0f);
}
bool BulletType::CreateShotBullet(Vector2f direction, Vector2f position, Vector2f vel) {
    if(mLastInactive >= 0 && mLastInactive < mBullets.size() && mCurTime >= mFireRate) {
        mCurTime = 0;
        float angleStep = 0;
        if(mShotsPerFrame > 1) {
            angleStep = DeltaAngle(mMultiShotAngleStart, mMultiShotAngleEnd) / (mShotsPerFrame - 1);
        }
        else {
            angleStep = DeltaAngle(mMultiShotAngleStart, mMultiShotAngleEnd) * 0.5;
        }
        float startAngle = 0;
        if(angleStep) {
            startAngle =atan2(direction.y, direction.x);
            if(std::isnan(startAngle)) {
                startAngle = 0;
            }
        }
        for(size_t i = 0; i < mShotsPerFrame; i++) {
            if(mLastInactive >= 0 && mLastInactive < mBullets.size()) {
                float angle = 0;
                if(fabs(angleStep) > 0.01 && mShotsPerFrame > 1) {
                    angle = startAngle +mMultiShotAngleStart + (angleStep * i);
                    direction.x = cos(angle);
                    direction.y = sin(angle);
                }
                if(!mBaseBullet.GetRotatesSprite()) {
                    if(direction.x < 0) {
                        mBullets[mLastInactive].SetHoriFlip(true);
                    }
                    else {
                        mBullets[mLastInactive].SetHoriFlip(false);
                    }
                }
                else {
                    if(direction.x < 0) {
                        mBullets[mLastInactive].SetVertFlip(true);
                    }
                    else {
                        mBullets[mLastInactive].SetVertFlip(false);
                    }
                }
                Vector2f vel2 = mShotForce * direction;
                if(fabs(angleStep) > 0.01) {
                    Vector2f rot = vel;
                    rot.Rotate(angle, vel.Magnitude(), 0.0f);
                    vel2+= rot;
                }
                else {
                    vel2+= vel;
                }
                if(!direction.x) {
                    vel2.x = 0.0f;
                }
                if(fabs(angleStep) < 0.01) {
                    vel = vel2;
                }
                mBullets[mLastInactive++].Shoot(vel2, position);
                if(mBaseBullet.GetAngleVel()) {
                    mBullets[mLastInactive - 1].SetAngle(angle);
                }
            }
        }
        return true;
    }
    return false;
}
bool BulletType::CreateShotLaser(Vector2f direction, Vector2f position, Vector2f vel) {
    if(mLastInactive >= 0 && mLastInactive < mBullets.size() && mCurTime >= mFireRate) {
        mCurTime = 0;
        float angleStep = DeltaAngle(mMultiShotAngleStart, mMultiShotAngleEnd) / mShotsPerFrame;
        for(size_t i = 0; i < mShotsPerFrame; i++) {
            if(mLastInactive >= 0 && mLastInactive < mBullets.size()) {
                float angle = atan2(direction.y, direction.x);
                mBullets[mLastInactive++].Shoot(vel, position);
                mBullets[mLastInactive - 1].SetAngle(angle);
                mBullets[mLastInactive - 1].SetSpriteAngle(angle);
            }
        }
        return true;
    }
    return false;
}
bool BulletType::CreateShotSustained(Vector2f direction, Vector2f position, Vector2f vel) {
}
bool BulletType::DefaultCreateShot(Vector2f direction, Vector2f position, Vector2f vel) {
    switch(mBaseBullet.GetType()) {
        case BTYPE_LASER:
            return CreateShotLaser(direction, position, vel);
        case BTYPE_MELEE:
            //return CreateShotLaser(direction, position, vel);
            break;
        case BTYPE_SUSTAINED:
            return CreateShotSustained(direction, position, vel);
        default:
            return CreateShotBullet(direction, position, vel);
    }
}
bool BulletType::CreateShot(Vector2f direction, Vector2f position, Vector2f vel) {
    if(mCreateFunc) {
        return mCreateFunc(this, direction, position, vel);
    }
    else {
        return DefaultCreateShot(direction, position, vel);
    }
}
void BulletType::InitBullets() {
    mBullets.resize(mMaxShots);
    if(mState) {
        mBaseBullet.SetScript(*mState, mPackageName);
    }
    //mBaseBullet.GetEmitter().CreateBuffers();
    //mBaseBullet.GetImpactEmitter().CreateBuffers();
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i] = mBaseBullet;
        if(mState) {
            mBullets[i].SetScript(*mState, mPackageName);
        }
        mBullets[i].GetEmitter().CreateBuffers();
        mBullets[i].GetImpactEmitter().CreateBuffers();
    }
    mLastInactive = 0;
}
void BulletType::SetState(kaguya::State* state, std::string package) {
    mState = state;
    mPackageName = package;
    InitBullets();
}
}
