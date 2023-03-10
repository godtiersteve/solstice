#ifndef BULLET_SPRAYER_H_INCLUDED
#define BULLET_SPRAYER_H_INCLUDED

#include "bullet_spawner.h"
#include "bullet_thrower.h"
#include "engine/timerentity.h"

namespace solstice {
    class BulletSprayer : public BulletSpawner {
    public:
        TimerEntity mSpawnTimer;
        float mStartAngle, mEndAngle;
        Vector2f mSpawnRange;
        bool mActivated;
        size_t mBulletsToSpawn;
        Vector2f mVelocityModifier;
        BulletSprayer();
        BulletSprayer(std::shared_ptr<BulletBase> baseBullet, int maxBullets, TimerEntity timer);
        void SetSpray(float startAngle, float endAngle);
        void Update(double dt);
        void Kill();
    };

}


#endif // BULLET_SPRAYER_H_INCLUDED
