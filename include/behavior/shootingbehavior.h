#ifndef SHOOTINGBEHAVIOR_H_INCLUDED
#define SHOOTINGBEHAVIOR_H_INCLUDED

#include "behavior.h"
#include "engine/shootingentity.h"

namespace solstice {
    class ShootingBehavior : public Behavior {
    private:
        int mVolleysPerformed, mBulletsShot;
        bool mShooting;
        float mShootCoolDown;
        ShootingEntity* mShootingEntity;
    public:
        int mBulletID;
        int mMaxVolleys, mBulletsPerVolley;
        float mShootCoolDownMax;
        TimerEntity mVolleyTimer;
        ShootingBehavior(int bulletID = -1);
        void Update(double dt, MovableObject* obj);
        void Reset() {Behavior::Reset(); mVolleysPerformed = 0; mShooting = false; mShootCoolDown = 0;}
        float GetShootCoolDown() {return mShootCoolDown;}
        bool IsShooting()   {return mShooting;}
    };
}

#endif // SHOOTINGBEHAVIOR_H_INCLUDED
