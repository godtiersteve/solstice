#include "bullet_thrower.h"

namespace solstice {
ThrowerBullet::ThrowerBullet()
{
    mInitialDamage = 10;
    mDamage = mInitialDamage;
    mMaxDamage = 10;
    mDamageStep = 0;
    mDamageFalloffTimer = TimerEntity(0, 0);
    mVelocity = glm::vec3(1.0, 1.0, 0.0);
    mAcceleration = glm::vec3(0.0, 0.0, 0.0);
    mMaxSpeed = glm::vec3(5.0, 5.0, 0.0);
    mCurrentLifetime = mLifetime = 10;
    mAlive = true;
    mDeathDamping = 0.7f;
}

ThrowerBullet::ThrowerBullet(int initDamage, int maxDamage, int damageStep) {
    SetDamageFalloff(initDamage, maxDamage, damageStep, TimerEntity(10, -1));
    mDamageFalloffTimer = TimerEntity(0, 0);
    mVelocity = glm::vec3(1.0, 1.0, 0.0);
    mAcceleration = glm::vec3(0.0, 0.0, 0.0);
    mMaxSpeed = glm::vec3(5.0, 5.0, 0.0);
    mCurrentLifetime = mLifetime = 10;
    mAlive = true;
    mDeathDamping = 0.7f;
}
void ThrowerBullet::SetDamageFalloff(int initial, int max, int step, TimerEntity timer) {
    mInitialDamage = initial;
    mMaxDamage = max;
    mDamageStep = step;
    mDamageFalloffTimer = timer;
    mDamage = mInitialDamage;
}

void ThrowerBullet::Update(double dt) {
    if(mCurrentLifetime > 0) {
        mCurrentLifetime--;
    }
    if(mCurrentLifetime <= 0 && mAlive) {
        Destroy();
    }
    BulletBase::Update(dt);
    if(!mAlive) {
        mPosition+= mVelocity;
        mVelocity*= mDeathDamping;
    }
    if(mDamageFalloffTimer.Update(dt) && mDamage != mMaxDamage) {
        mDamage+= mDamageStep;
    }
}
void ThrowerBullet::Reset() {
    mCurrentLifetime = mLifetime;
    BulletBase::Reset();
}
bool ThrowerBullet::Finished() {
    bool finished = BulletBase::Finished();
    if(mCurrentLifetime <= 0 && finished) {
        return true;
    }
    return finished;
}

}
