#include "bullet_sprayer.h"

namespace solstice {
BulletSprayer::BulletSprayer() {
    mBaseBullet = nullptr;
    mStartAngle = -PI / 2;
    mEndAngle = PI / 2;
    mVelocityModifier = 1.0;
    mReusesBullets = true;
    mCanShoot = true;
    mRespawnTimer.mDuration = -1;
    mSpawnRange.x = 0;
    mSpawnRange.y = 30;
    mBulletsToSpawn = 3;
}

BulletSprayer::BulletSprayer(std::shared_ptr<BulletBase> baseBullet, int maxBullets, TimerEntity timer) {
    mBaseBullet = baseBullet;
    mSpawnTimer = timer;
    mStartAngle = -PI / 2;
    mEndAngle = PI / 2;
    mVelocityModifier = 1.0;
    mReusesBullets = true;
    mRespawnTimer.mDuration = -1;
    mSpawnRange.x = 0;
    mSpawnRange.y = 30;
    mBulletsToSpawn = 3;
}

void BulletSprayer::SetSpray(float startAngle, float endAngle) {
    mStartAngle = startAngle;
    mEndAngle = endAngle;
}

void BulletSprayer::Update(double dt) {
    if(mActivated) {
        if(mSpawnTimer.Update(dt)) {
            for(size_t i = 0; i < mBulletsToSpawn; i++) {
                std::shared_ptr<ThrowerBullet> bullet = std::make_shared<ThrowerBullet>(*(ThrowerBullet*)mBaseBullet.get());
                float angle = Randf(mStartAngle, mEndAngle) + (mAimDirection.x < 0 || mAimDirection.y < 0 ? PI : 0);
                glm::vec2 spawnStart = mSpawnPosition - mSpawnRange;
                glm::vec2 spawnEnd = mSpawnPosition + mSpawnRange;
                glm::vec2 spawnPos = RandVec2(spawnStart, spawnEnd);
                Vector2f shootVel = Vector2f(cos(angle) * mVelocityModifier.x, sin(angle) * mVelocityModifier.y);
                Vector2f startVel = mStartVelocity;
                bullet->Shoot(shootVel, Vector2f(spawnPos.x, spawnPos.y));
                if(mAimDirection.y) {
                    bullet->mVerticallyOriented = true;
                }
                if(bullet->mAnimations.size()) {
                    int frame = Randi(0, bullet->mAnimations[0].GetNumFrames());
                    bullet->mAnimations[0].Start();
                    bullet->mAnimations[0].ForceFrame(frame);
                    bullet->mAnimations[0].Stop();
                }
                CreateBullet(bullet);
            }
        }
    }
    else {
        mSpawnTimer.Reset();
    }
    UpdateBullets(dt);
}
void BulletSprayer::Kill() {
    mActivated = false;
}
}
