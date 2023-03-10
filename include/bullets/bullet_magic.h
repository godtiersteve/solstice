#ifndef BULLET_MAGIC_H_INCLUDED
#define BULLET_MAGIC_H_INCLUDED

#include "bullet_spawner.h"
#include "engine/timerentity.h"
#include <memory>

namespace solstice {
    class MagicBullet {
    private:
        std::shared_ptr<BulletSpawner> mSpawner;
        bool mCasting, mCasted;
    public:
        TimerEntity mCastTimer;
        MagicBullet(std::shared_ptr<BulletSpawner> spawner = nullptr);
        void SetSpawner(std::shared_ptr<BulletSpawner> spawner);
        void Update(double dt);
        bool CheckAndResolveCollision(BackgroundTile2& ground);
        bool CheckAndResolveCollision(RigidBody* b);
        bool CheckAndResolveCollision(WaterBody& b);
        void ResetAnimations();
        bool Finished();
        void Reset();
        void StartCast() {mCastTimer.Reset(); mCasting = true;}
        void Render(SceneRenderer& rend);
        void DrawHitBoxes(SceneRenderer& rend, Color color);

    };

}

#endif // BULLET_MAGIC_H_INCLUDED
