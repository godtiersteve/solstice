#ifndef SHOOTINGENTITY_H_INCLUDED
#define SHOOTINGENTITY_H_INCLUDED

#include "physics/physicsrect.h"
#include "bullets/bullettype.h"
#include <memory>

namespace solstice {
    class ShootingEntity : public GameObject {
    public:
        std::vector<std::shared_ptr<BulletType> > mBullets;
        Vector2f mShootDirection;

    };

}

#endif // SHOOTINGENTITY_H_INCLUDED
