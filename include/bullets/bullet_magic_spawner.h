#ifndef BULLET_MAGIC_SPAWNER_H_INCLUDED
#define BULLET_MAGIC_SPAWNER_H_INCLUDED

#include "bullet_spawner.h"

namespace solstice {
    enum MagicBulletLevel {MAGIC_LEVEL_1, MAGIC_LEVEL_2, MAGIC_LEVEL3};
    class MagicBulletSpawner : public BulletSpawner {
    public:
        MagicBullet();
    };
}

#endif // BULLET_MAGIC_SPAWNER_H_INCLUDED
