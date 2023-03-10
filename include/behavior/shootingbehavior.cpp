#include "shootingbehavior.h"

namespace solstice {
ShootingBehavior::ShootingBehavior(int bulletID) {
    mBulletID = bulletID;
    mBehaviorType = BEHAVIOR_SHOOT;
    mVolleysPerformed = 0;
    mBulletsShot = 0;
    mShooting = true;
    mShootCoolDownMax = 0;
    mShootingEntity = nullptr;
    mName = "Shooting";
    mMotion.mStopsMomentum = true;
}

void ShootingBehavior::Update(double dt, MovableObject* obj) {
    if(mBulletID < 0) {
        return;
    }
    if(obj->GetCurFrameNum() >= mStartFrame) {
        if(!mFinished) {
            if(!mShootingEntity) {
                if(obj->HasEntity(ENTITY_SHOOTING)) {
                    mShootingEntity = static_cast<ShootingEntity*>(obj->GetEntity(ENTITY_SHOOTING));
                }
                else {
                    return;
                }
            }
            if(!mMotion.mVelocity.x) {
                obj->StopX();
            }
            if(!mShooting && mVolleyTimer.Finished()) {
                mShooting = true;
                mVolleyTimer.Reset();
            }
            if(mShooting) {
                if(obj->IsType(OBJECT_ENEMY) && mTimer.Update(dt)) {
                    mShootingEntity->mBullets[mBulletID]->CreateShot(mShootingEntity->mShootDirection, mDestination, 0);
                    mBulletsShot+= mShootingEntity->mBullets[mBulletID]->GetShotsPerFrame();
                }
            }
            if(mBulletsShot >= mBulletsPerVolley) {
                mShooting = false;
                mBulletsShot = 0;
                mVolleysPerformed++;
                mTimer.Reset();
            }
        }
        else {
            mShootCoolDown+= dt;
            if(mShootCoolDown >= mShootCoolDownMax) {
                Reset();
            }
        }
        mFinished = mVolleysPerformed >= mMaxVolleys;
    }
}

}
