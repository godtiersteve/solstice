#ifndef BULLET_SPAWNER_H_INCLUDED
#define BULLET_SPAWNER_H_INCLUDED

#include "bullet.h"
#include "bullet_laser.h"
#include "bullet_missile.h"
#include "bullet_melee.h"
#include "bullet_expand.h"
#include "gunmod.h"
#include "graphics/scenerenderer.h"
#include "engine/timerentity.h"
#include <list>
#include <vector>
#include <memory>

namespace solstice {
    struct BulletSpawnerStartupAnimation {
        std::vector<Animation> mAnimations;
        TimerEntity mTimer;
    };
    class BulletSpawner {
    protected:
        std::list<std::shared_ptr<BulletBase> > mBullets;
        std::vector<std::shared_ptr<GunMod> > mMods;
        int mMaxBullets;
        bool mCanShoot;
        bool mPaused, mStarted;
        virtual void UpdateBullets(double dt);
        TimerEntity mRespawnTimer;
    public:
        BulletSpawnerStartupAnimation mStatupAnimation;
        Vector2f mSpawnPosition, mAimDirection, mSpawnOffset;
        std::vector<Animation> mSpawnAnimations;
        TimerEntity mSpawnTimer;
        Color mFlashColor;
        bool mReusesBullets, mAutoSpawns;
        Vector2f mStartVelocity;
        BulletSpawner();
        BulletSpawner(std::shared_ptr<BulletBase> baseBullet, int maxBullets);
        std::shared_ptr<BulletBase> mBaseBullet;

        std::list<std::shared_ptr<BulletBase> > GetBullets() {return mBullets;}
        void SetBaseBullet(std::shared_ptr<BulletBase> b) {mBaseBullet = b;}
        void SetMaxBullets(size_t i) {mMaxBullets = i;}
        virtual void Update(double dt);
        virtual void Render(SceneRenderer& rend);
        virtual void DrawHitBoxes(SceneRenderer& rend, Color color);
        virtual void Draw(SceneRenderer& rend);
        virtual bool CheckAndResolveCollision(BackgroundTile2& ground);
        virtual bool CheckAndResolveCollision(RigidBody* b);
        virtual bool CheckAndResolveCollision(WaterBody& b);
        virtual void ResetAnimations();
        virtual bool CreateBullet(std::shared_ptr<BulletBase> bullet = nullptr);
        virtual bool CreateBullet(Vector2f direction, Vector2f pos, std::shared_ptr<BulletBase> bullet = nullptr);
        virtual void Reset();
        virtual void AddMod(std::shared_ptr<GunMod> mod);
        virtual void RemoveMode(size_t i);
        virtual void RemoveMod(std::shared_ptr<GunMod> mod);
        bool Finished();
        void SetRespawnTimer(TimerEntity t) {mRespawnTimer = t; mRespawnTimer.Finish();}
        void SetFireRate(float d) {mRespawnTimer.mDuration = d; mRespawnTimer.Finish();}
        TimerEntity GetRespawnTimer() {return mRespawnTimer;}
        virtual void Kill() {mBullets.clear();}
        virtual void SetPositions(Vector2f pos);
        virtual void AimBullets(Vector2f pos);
        virtual void AngleBullets(float a);
        virtual void AddBullet(std::shared_ptr<BulletBase> bullet);
        virtual void AddBullet(size_t i = 0);
        virtual void CheckExplosions(RigidBody* obj);
        size_t GetNumBullets() {return mBullets.size();}
        void Start() {mPaused = false; mStarted = true;}
        void Pause() {mPaused = true;}
        void Stop() {mPaused = true; mStarted = false; Kill();}
        bool GetPaused() {return mPaused;}
    };

}

#endif // BULLET_SPAWNER_H_INCLUDED
