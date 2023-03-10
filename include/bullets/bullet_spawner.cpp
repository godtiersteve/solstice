#include "bullet_spawner.h"

namespace solstice {
BulletSpawner::BulletSpawner() {
    mBaseBullet = nullptr;
    mMaxBullets = 30;
    mAimDirection = Vector2f(1.0, 0.0);
    mReusesBullets = false;
    mAutoSpawns = false;
    mRespawnTimer.mDuration = -1;
    mRespawnTimer.mLoops = 1;
    mPaused = false;
    mStarted = true;
}

BulletSpawner::BulletSpawner(std::shared_ptr<BulletBase> baseBullet, int maxBullets) {
    mBaseBullet = baseBullet;
    mMaxBullets = maxBullets;
    mAimDirection = Vector2f(1.0, 0.0);
    mReusesBullets = false;
    mAutoSpawns = false;
    mRespawnTimer.mDuration = -1;
    mRespawnTimer.mLoops = 1;
    mPaused = false;
    mStarted = true;
}

void BulletSpawner::UpdateBullets(double dt) {
    if(!mStarted) {
        return;
    }
    mRespawnTimer.Update(dt);
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        (*it)->Update(dt);
        if((*it)->Finished()) {
            if(!mReusesBullets) {
                it = mBullets.erase(it);
            }
            else {
                it = mBullets.erase(it);
            }
        }
    }
}
void BulletSpawner::Update(double dt) {
    if(!mStarted) {
        return;
    }
    if(mAutoSpawns && !mPaused) {
        if(mSpawnTimer.Update(dt)) {
            CreateBullet();
        }
    }
    UpdateBullets(dt);
    for(size_t i = 0; i < mSpawnAnimations.size(); i++) {
        mSpawnAnimations[i].SetPosition(mSpawnPosition + mSpawnOffset);
        mSpawnAnimations[i].Update(dt);
    }
}

void BulletSpawner::Render(SceneRenderer& rend) {
    for(size_t i = 0; i < mSpawnAnimations.size(); i++) {
        rend.DrawQuad(mSpawnAnimations[i].GetDrawQuad());
    }
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        (*it)->Draw(rend);
    }
}
void BulletSpawner::SetPositions(Vector2f pos) {
    for(auto iter = mBullets.begin(); iter != mBullets.end(); iter++) {
        (*iter)->SetPosition(pos);
    }
}
void BulletSpawner::AimBullets(Vector2f pos) {
    for(auto iter = mBullets.begin(); iter != mBullets.end(); iter++) {
        (*iter)->AimTowards(pos);
    }
}
void BulletSpawner::AngleBullets(float a) {
    for(auto iter = mBullets.begin(); iter != mBullets.end(); iter++) {
        (*iter)->SetAngle(a);
    }
}
bool BulletSpawner::CreateBullet(Vector2f direction, Vector2f pos, std::shared_ptr<BulletBase> bullet) {
    if((!bullet && !mBaseBullet) || !mRespawnTimer.Finished() || mBullets.size() >= mMaxBullets) {
        return false;
    }
    if(mBaseBullet && !bullet) {
        switch(mBaseBullet->GetType()) {
            case BTYPE_LASER:
                bullet = std::make_shared<Laser>(*static_cast<Laser*>(mBaseBullet.get()));
                if(mAimDirection.x || mAimDirection.y) {
                    bullet->SetAngle(atan2(mAimDirection.y, mAimDirection.x));
                }
                break;
            case BTYPE_MISSILE:
                bullet = std::make_shared<Missile>(*static_cast<Missile*>(mBaseBullet.get()));
                break;
            case BTYPE_MELEE:
                bullet = std::make_shared<MeleeBullet>(*static_cast<MeleeBullet*>(mBaseBullet.get()));
                break;
            case BTYPE_EXPANDING:
                bullet = std::make_shared<ExpandingBullet>(*static_cast<ExpandingBullet*>(mBaseBullet.get()));
                break;
            default:
                bullet = std::make_shared<BulletBase>(*mBaseBullet);
                break;
        }
        bullet->Shoot(glm::length(bullet->GetVelocity()) * direction + mStartVelocity.Magnitude() * direction);
    }
    for(size_t i = 0; i < mMods.size(); i++) {
        mMods[i]->OnSpawn(bullet.get());
    }
    bullet->SetPosition(pos);
    mRespawnTimer.Reset();
    AddBullet(bullet);
    return true;
}
bool BulletSpawner::CreateBullet(std::shared_ptr<BulletBase> bullet) {
    return CreateBullet(mAimDirection, mSpawnPosition + mSpawnOffset, bullet);
}
void BulletSpawner::AddBullet(std::shared_ptr<BulletBase> bullet) {
    if(mBullets.size() < mMaxBullets) {
        mBullets.push_back(bullet);
    }
}
void BulletSpawner::AddBullet(size_t i) {
}
void BulletSpawner::DrawHitBoxes(SceneRenderer& rend, Color color) {
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        (*it)->DrawHitBoxes(rend, color);
    }
}
void BulletSpawner::Draw(SceneRenderer& rend) {
    if(mStarted && !Finished()) {
        for(size_t i = 0; i < mSpawnAnimations.size(); i++) {
            rend.DrawQuad(mSpawnAnimations[i].GetDrawQuad());
        }
    }
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        (*it)->Draw(rend);
    }
}
bool BulletSpawner::CheckAndResolveCollision(BackgroundTile2& ground) {
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        (*it)->CheckAndResolveCollision(ground);
    }
}

bool BulletSpawner::CheckAndResolveCollision(RigidBody* b) {
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        (*it)->CheckAndResolveCollision(b);
    }
}

bool BulletSpawner::CheckAndResolveCollision(WaterBody& b) {
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        (*it)->CheckAndResolveCollision(b);
    }
}

void BulletSpawner::ResetAnimations() {
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        (*it)->ResetAnimations();
    }
}
void BulletSpawner::CheckExplosions(RigidBody* obj) {
    if(obj) {
        for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
            (*it)->CheckExplosion(obj);
        }
    }
}
void BulletSpawner::Reset() {
    Stop();
    mSpawnTimer.Reset();
    mRespawnTimer.Reset();
}
void BulletSpawner::AddMod(std::shared_ptr<GunMod> mod) {
}
void BulletSpawner::RemoveMode(size_t i) {
}
void BulletSpawner::RemoveMod(std::shared_ptr<GunMod> mod) {
}
bool BulletSpawner::Finished() {return mBullets.empty();}
}
