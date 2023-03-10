#include "multishot.h"

namespace solstice {
    MultishotBullet::MultishotBullet(Bullet base, int maxShots, float fireRate, float force, int type):
    BulletType(base, maxShots, fireRate, force, type)
    {
        mShotsMade = 0;
        mBulletsPerShot = 0;
        mTimeBetweenShots = 10;
        mShooting = false;
    }
    void MultishotBullet::Update(float dt) {
        if(mShooting && mCurTime >= mTimeBetweenShots && mBulletsPerShot > mShotsMade) {
            mShotQueued = true;
            if(CreateShot(mNextSpawnDir, mNextSpawnPosition, mNextSpawnVel)) {
                mShotsMade++;
                mShotQueued = false;
            }
        }
        if(mShotsMade >= mBulletsPerShot) {
            mShotsMade = 0;
            mShooting = false;
        }
        BulletType::Update(dt);
    }
    int MultishotBullet::CreateShot(Vector2f direction, Vector2f position, Vector2f vel) {
        mShooting = true;
        mNextSpawnDir = direction;
        mNextSpawnPosition = position;
        mNextSpawnVel = vel;
        mChargeDirection = direction;
        return BulletType::CreateShot(direction, position, vel);
    }
}
