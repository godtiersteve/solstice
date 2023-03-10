#ifndef BULLET_THROWER_H_INCLUDED
#define BULLET_THROWER_H_INCLUDED

#include "bulletbase.h"
#include "engine/timerentity.h"

namespace solstice {

class ThrowerBullet : public BulletBase {
public:
    int mInitialDamage, mMaxDamage, mDamageStep;
    int mLifetime, mCurrentLifetime;
    TimerEntity mDamageFalloffTimer;
    float mDeathDamping;

    ThrowerBullet();
    ThrowerBullet(int initDamage, int maxDamage, int damageStep);
    void Update(double dt);
    void SetDamageFalloff(int initial, int max, int step, TimerEntity timer);
    void SetLifetime(int lifetime) {mLifetime = lifetime; mCurrentLifetime = lifetime;}
    void Reset();
    bool Finished();
};
}

#endif // BULLET_THROWER_H_INCLUDED
