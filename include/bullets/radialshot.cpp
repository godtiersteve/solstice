#include "radialshot.h"

namespace solstice {
RadialBullet::RadialBullet() {
    SetDefaultAttributes();
    Init(Bullet(), 10, 2, 0, TWO_PI / 10, 10, 5);
}
RadialBullet::RadialBullet(Bullet baseBullet, int numBullets, int shotsPerFrame, float startAngle, float angleBetween, float shotForce, float fireRate) {
    SetDefaultAttributes();
    Init(baseBullet, numBullets, shotsPerFrame, startAngle, angleBetween, shotForce, fireRate);
}
void RadialBullet::Init(Bullet baseBullet, int numBullets, int shotsPerFrame, float startAngle, float angleBetween, float shotForce, float fireRate) {
    mBaseBullet = baseBullet;
    mBullets = std::vector<Bullet>(numBullets, baseBullet);
    mAngleBetween = angleBetween;
    mStartAngle = startAngle;
    mCurTime = fireRate;
    mFireRate = fireRate;
    mCurAngle = mStartAngle;
    mLastInactive  = 0;
    mShotsPerFrame = shotsPerFrame;
    mShotForce = shotForce;
}
int RadialBullet::CreateShot(Vector2f direction, Vector2f position, Vector2f vel) {
    int created = -1;
    for(size_t i = 0; i < mShotsPerFrame; i++) {
        if(CanCreate()) {
            mCurAngle+= mAngleBetween;
            Vector2f dir = Vector2f(cos(mCurAngle), sin(mCurAngle));
            vel = dir * mShotForce;
            mBullets[mLastInactive].Shoot(vel, position);
            glm::vec3 acc = glm::vec3(dir.x, dir.y, 0.0) * mBaseBullet.GetAcceleration().x;
            mBullets[mLastInactive].SetAcceleration(acc);
            FindNextInactive();
            created = mLastInactive;
        }
    }
    if(created > -1) {
        mCurTime = 0;
    }
    return created;
}
void RadialBullet::Reset() {
    mCurAngle = mStartAngle;
    BulletType::Reset();
}
}
