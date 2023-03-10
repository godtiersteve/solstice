#ifndef MULTISHOT_H_INCLUDED
#define MULTISHOT_H_INCLUDED

#include "bullettype.h"

namespace solstice {
    class MultishotBullet : public BulletType {
    private:
        int mShotsMade;
        bool mShooting;
    public:
        float mTimeBetweenShots;
        int mBulletsPerShot;
        MultishotBullet(Bullet base, int maxShots, float fireRate, float force, int type);
        void Update(float dt);
        int CreateShot(Vector2f direction, Vector2f position, Vector2f vel);
    };

}

#endif // MULTISHOT_H_INCLUDED
