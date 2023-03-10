#ifndef RADIALSHOT_H_INCLUDED
#define RADIALSHOT_H_INCLUDED

#include "bullets/bullettype.h"

namespace solstice {
class RadialBullet : public BulletType {
private:
    float mAngleBetween;
    float mStartAngle;
    float mCurAngle;
public:
    RadialBullet();
    RadialBullet(Bullet baseBullet, int numBullets, int shotsPerFrame, float startAngle, float angleBetween, float shotForce, float fireRate);
    void Init(Bullet baseBullet, int numBullets, int shotsPerFrame, float startAngle, float angleBetween, float shotForce, float fireRate);
    int CreateShot(Vector2f direction = 0, Vector2f position = 0, Vector2f vel = 0);
    void Reset();
};

}


#endif // RADIALSHOT_H_INCLUDED
