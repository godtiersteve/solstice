#ifndef BULLET_MAGIC_CASTER_H_INCLUDED
#define BULLET_MAGIC_CASTER_H_INCLUDED

#include "bullet_spawner.h"

namespace solstice {
    enum MagicCasterLevel {MAG_LVL_1, MAG_LVL_2, MAG_LVL_3, MAG_LVL_4, MAG_LVL_5};
    class MagicCaster : public BulletSpawner {
    private:
        std::vector<std::shared_ptr<BulletSpawner> > mLevelOneMagic, mLevelTwoMagic, mLevelThreeMagic;
    public:
        MagicCaster();
        MagicCaster(std::shared_ptr<BulletSpawner> levelOne, std::shared_ptr<BulletSpawner> levelTwo, std::shared_ptr<BulletSpawner> levelThree);
        MagicCaster(const std::vector<std::shared_ptr<BulletSpawner> >& levelOne,
                    const std::vector<std::shared_ptr<BulletSpawner> >& levelTwo,
                    const std::vector<std::shared_ptr<BulletSpawner> >& levelThree);
        void CastSpell(MagicCasterLevel magicLevel);
        void Update(double dt);
        void SetMagic(std::shared_ptr<BulletSpawner> levelOne, std::shared_ptr<BulletSpawner> levelTwo, std::shared_ptr<BulletSpawner> levelThree);
        void SetMagic(const std::vector<std::shared_ptr<BulletSpawner> >& levelOne,
                      const std::vector<std::shared_ptr<BulletSpawner> >& levelTwo,
                      const std::vector<std::shared_ptr<BulletSpawner> >& levelThree);
        void Draw(SceneRenderer& rend);
        void DrawHitBoxes(SceneRenderer& rend, Color color);
        bool CheckAndResolveCollision(BackgroundTile2& ground);
        bool CheckAndResolveCollision(RigidBody* b);
        bool CheckAndResolveCollision(WaterBody& b);
    };
}

#endif // BULLET_MAGIC_CASTER_H_INCLUDED
