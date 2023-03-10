#ifndef BULLET_MELEE_SPAWNER_H_INCLUDED
#define BULLET_MELEE_SPAWNER_H_INCLUDED

#include "bullet_spawner.h"
#include "bullet_melee.h"
#include "bullet_flurry.h"

namespace solstice {
    class BulletMeleeSpawner : public BulletSpawner {
    private:
        bool mFinisherCollision;
        Vector2f mFinisherTarget;
        std::list<FlurryBullet> mFinishers;
    public:
        std::vector<MeleeBullet> mBaseBullets;
        int mRepsBeforeFinish;
        FlurryBullet mFinisherBase;
        BulletMeleeSpawner();
        BulletMeleeSpawner(std::vector<MeleeBullet> baseBullets);
        void CreateSlash(size_t i, bool flipped = false);
        bool GetFinisherCollision() {return mFinisherCollision;}
        bool HasFinisher()          {return !mFinishers.empty();}
        Vector2f GetFinisherTarget() {return mFinisherTarget;}
        bool CheckAndResolveCollision(BackgroundTile2& ground) {return false;}
        bool CheckAndResolveCollision(RigidBody* b);
        bool CheckAndResolveCollision(WaterBody& b) {return false;}
        void DoFinisher();
        void UpdateBullets(double dt);
        void Draw(SceneRenderer& rend);
        void SetPositions(Vector2f pos);
        void EndFinisher();
        void ResetFinisherReps();
        bool FinisherEnding();
        int GetFinisherReps();
    };
}

#endif // BULLET_MELEE_SPAWNER_H_INCLUDED
