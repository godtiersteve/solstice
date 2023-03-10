#include "bullets/bullettype.h"

namespace solstice {
BulletType::BulletType() {
    SetDefaultAttributes();
}
BulletType::BulletType(Bullet base, int maxShots, float fireRate, float force, int type) {
    base.SetType(type);
    Init(base, maxShots, fireRate, force);
    mState = nullptr;
    InitBullets();
    SetDefaultAttributes();
}
BulletType::BulletType(kaguya::State* state, std::string package, int maxShots, float fireRate, float force, int type) {
    Bullet b;
    b.SetType(type);
    Init(b, maxShots, fireRate, force);
    SetState(state, package);
    mName = package;
}
BulletType::BulletType(std::string package, int maxShots, float fireRate, float force, int type) {
    Bullet b;
    b.SetType(type);
    Init(b, maxShots, fireRate, force);
    mPackageName = package;
    SetDefaultAttributes();
}
void BulletType::SetDefaultAttributes() {
    mLastInactive = 0;
    mLastActive = 0;
    mState = nullptr;
    mMultiShotAngleStart = 0;
    mMultiShotAngleEnd = 0;
    mCreateFunc = nullptr;
    mHoldShot = false;
    mMagicCost = 0;
    mSubCreateDelay = -1;
    mChargeable = false;
    mShootOnRelease = false;
    mChargeable = false;
    mSubBullet = nullptr;
    mDelayTimerCur = 0;
    mDelayTimer = -1;
    mNextToMake = 0;
    mNextPerShot = 0;
    mSubBulletID = -1;
    mShotQueued = false;
    mChargingShot = false;
    mChargeFrames = 0;
    mMaxChargeFrames = 0;
    mChargeStep = 1;
    mAutoReleaseCharge = false;
    mMaxShots = 3;
}
bool BulletType::operator<(const BulletType& t) {
    if(mShotType < t.mShotType) {
        return true;
    }
    else if(mShotType == t.mShotType) {
        if(mModType < t.mModType) {
            return true;
        }
    }
    return false;
}
bool BulletType::operator>(const BulletType& t) {
    if(mShotType > t.mShotType) {
        return true;
    }
    else if(mShotType == t.mShotType) {
        if(mModType > t.mModType) {
            return true;
        }
    }
    return false;
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
    if(mChargingShot && mChargeFrames < mMaxChargeFrames) {
        mChargeFrames++;
    }
    if(mChargeFrames >= mMaxChargeFrames && mAutoReleaseCharge) {
        mShotQueued = true;
    }
    if(mShotQueued) {
        Vector2f velocity = 0;
        float shotForce = mShotForce;
        if(mChargeForceStep.Magnitude2() >= 0.1) {
            velocity = mNextSpawnVel + mChargeForceStart + mChargeForceStep * (float)(mChargeFrames / mChargeStep);
            velocity.Rotate(atan2(mNextSpawnDir.y, mNextSpawnDir.x) - PI * 0.5) ;
        }
        else {
            mShotForce*= (mChargeFrames / mChargeStep);
            velocity = mChargeForceStart;
        }
        if(CreateShot(mNextSpawnDir, mNextSpawnPosition, velocity)) {
            mChargeFrames = 0;
            mChargingShot = false;
            mShotQueued = false;
        }
        mShotForce = shotForce;
    }
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i].Update(dt);
        mBullets[i].UpdateAnimation(dt);
        if(mBaseBullet.GetType() == BTYPE_LASER) {
            //mBullets[i].AimTowards(mTargetPosition);CreateShotLaser()
            mBullets[i].DirectTowards(mAimDirection);
        }
        if(mBullets[i].Finished() && i < mLastInactive) {
            mLastInactive = i;
        }
        if(mSubBullet) {
            if(mBullets[i].GetCreateSubState() == 1 && (mBaseBullet.GetCreatesSubBullets() || (mSubCreateDelay > 0 && mBullets[i].GetCurFrame() >= mSubCreateDelay))) {
                Vector2f dir;
                float angle = mBullets[i].GetAngle() ? mBullets[i].GetAngle() : atan2(mBullets[i].GetVelocity().y, mBullets[i].GetVelocity().x);
                if(mBaseBullet.GetSubBulletDir().Magnitude2()) {
                    dir = mBaseBullet.GetSubBulletDir();
                    dir.Rotate(angle, 0);
                }
                else {
                    dir+= Vector2f(cos(angle), sin(angle));
                }
                Vector2f pos;
                if(mSubCreateDelay < 0.0) {
                    pos = mBullets[i].GetHitBox().GetCenter();
                }
                else {
                    pos = mSubCreateDelay;
                }
                mSubBullet->CreateShot(dir, pos);
                mBullets[i].SetCreateSubState(2);
            }
            if(mBullets[i].Alive() && mBullets[i].CreateSub()) {
                int index = mSubBullet->CreateShot(mBullets[i].GetSubDir(), mBullets[i].GetSubPos());
                if(index > -1) {
                    mBullets[i].AddSubBullet(&mSubBullet->GetBullet(index));
                }
            }
        }
    }
    if(mCurTime < mFireRate) {
        //mCurTime+= dt;
        mCurTime++;
    }
}
void BulletType::CreateSubBullet(Vector2f dir, Vector2f pos) {
}
void BulletType::QueueShot() {
    if(mChargingShot) {
        mShotQueued = true;
    }
}
void BulletType::SetCharge(Vector2f startForce, Vector2f stepForce, int maxFrames, int frameStep, bool autoRelease) {
    mChargeForceStart = startForce;
    mChargeForceStep = stepForce;
    mMaxChargeFrames = maxFrames;
    mChargeStep = frameStep;
    mChargeable = true;
    mAutoReleaseCharge = autoRelease;
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
int BulletType::CreateShot(Vector2f direction) {
    return CreateShot(direction, 0.0f, 0.0f);
}
int BulletType::CreateShot(Vector2f direction, Vector2f position) {
    return CreateShot(direction, position, 0.0f);
}
int BulletType::CreateShotBullet(Vector2f direction, Vector2f position, Vector2f vel) {
    if(mChargeable && !mShotQueued) {
        mNextSpawnDir = direction;
        mNextSpawnPosition = position;
        mNextSpawnVel = vel;
        mChargingShot = true;
    }
    else if(mLastInactive >= 0 && mLastInactive < mBullets.size() && (mShotQueued || mCurTime >= mFireRate)) {
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
            startAngle = atan2(direction.y, direction.x);
            if(std::isnan(startAngle)) {
                startAngle = 0;
            }
        }
        for(size_t i = 0; i < mShotsPerFrame; i++) {
            if(mLastInactive >= 0 && mLastInactive < mBullets.size()) {
                float angle = 0;
                if(fabs(angleStep) > 0.01 && mShotsPerFrame > 1) {
                    angle = startAngle + mMultiShotAngleStart + (angleStep * i);
                    direction = Vector2f(cos(angle), sin(angle));
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
                Vector2f vel2 = mShotForce * direction + vel;
                if(!direction.x) {
                    vel2.x = 0.0f;
                }
                Bullet& bullet = mBullets[mLastInactive];
                bullet.Shoot(vel2, position);
                if(mBaseBullet.GetAngleVel()) {
                    bullet.SetAngle(angle);
                }
                if(mSubCreateDelay >= 0.0) {
                    bullet.SetCreateSubState(1);
                }
                if(mBaseBullet.GetSubCreateFrame() >= 0) {
                    bullet.SetSubPos(position);
                    bullet.SetSubDir(direction);
                }
                if(mShotsPerFrame > 1) {
                    FindNextInactive();
                }
                else {
                    FindNextInactive();
                }
            }
        }
        return FindNextInactive();
    }
    return -1;
}
int BulletType::FindNextInactive() {
    if(mLastInactive < mBullets.size() && !mBullets[mLastInactive].Finished()) {
        mLastInactive = mBullets.size();
    }
    for(size_t i = 0; i < mBullets.size(); i++) {
        if(mBullets[i].Finished()) {
            if(i < mLastInactive) {
                mLastInactive = i;
                return mLastInactive;
            }
        }
    }
    return mLastInactive;
}
int BulletType::CreateShotLaser(Vector2f direction, Vector2f position, Vector2f vel) {
    if(mLastInactive >= 0 && mLastInactive < mBullets.size() && mCurTime >= mFireRate) {
        mCurTime = 0;
        float angleStep = DeltaAngle(mMultiShotAngleStart, mMultiShotAngleEnd) / mShotsPerFrame;
        for(size_t i = 0; i < mShotsPerFrame; i++) {
            if(mLastInactive >= 0 && mLastInactive < mBullets.size()) {
                float angle = atan2(direction.y, direction.x);
                mBullets[mLastInactive++].Shoot(vel, position);
                mBullets[mLastInactive - 1].SetAngle(angle);
                mBullets[mLastInactive - 1].mSpriteAngle = angle;
            }
        }
        return mLastActive - 1;
    }
    return -1;
}
int BulletType::CreateShotSustained(Vector2f direction, Vector2f position, Vector2f vel) {
}
int BulletType::DefaultCreateShot(Vector2f direction, Vector2f position, Vector2f vel) {
    switch(mBaseBullet.GetType()) {
        case BTYPE_LASER:
            return CreateShotLaser(direction, position, vel);
        case BTYPE_MELEE:
            break;
        case BTYPE_SUSTAINED:
            return CreateShotSustained(direction, position, vel);
        default:
            return CreateShotBullet(direction, position, vel);
    }
    return -1;
}
int BulletType::CreateShot(Vector2f direction, Vector2f position, Vector2f vel) {
    if(mCreateFunc) {
        return mCreateFunc(this, direction, position, vel);
    }
    else {
        return DefaultCreateShot(direction, position, vel);
    }
}
void BulletType::InitBullets(kaguya::State* state) {
    mState = state;
    InitBullets();
}
void BulletType::SetCreateFuncStr(std::string package, std::string str) {
    if(mState) {
        mCreateFunc = (*mState)[package][str];
    }
    mCreateFuncName = str;
}
void BulletType::InitBullets() {
    if(!mMaxShots) {
        return;
    }
    mBullets.resize(mMaxShots);
    if(mState && !mPackageName.empty()) {
        mBaseBullet.SetScript(*mState, mPackageName);
        if(!mCreateFuncName.empty()) {
            mCreateFunc = (*mState)[mPackageName][mCreateFuncName];
        }
    }
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i] = mBaseBullet;
        if(mState && !mPackageName.empty()) {
            mBullets[i].SetScript(*mState, mPackageName);
        }
    }
    mLastInactive = 0;
}
void BulletType::SetPosition(glm::vec3 pos) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i].SetPosition(pos);
    }
}
void BulletType::SetState(kaguya::State* state, std::string package) {
    mState = state;
    mPackageName = package;
    InitBullets();
}
void BulletType::SetRenderer(ParticleRenderer* p) {
    for(size_t i = 0; i < mBullets.size(); i++) {
        mBullets[i].SetRenderer(p);
    }
    if(mSubBullet) {
        mSubBullet->SetRenderer(p);
    }
}
}
