#include "bullet_magic.h"

namespace solstice {
MagicBullet::MagicBullet(std::shared_ptr<BulletSpawner> spawner)
{
    SetSpawner(spawner);
    //mCastTimer.Init(1.0 / 60.0 * 15, 0);
    mCastTimer.Init(0, -1);
    mCastTimer.Finish();
    mCasting = true;
    mCasted = false;
}
void MagicBullet::SetSpawner(std::shared_ptr<BulletSpawner> spawner) {
    mSpawner = spawner;
}
void MagicBullet::Update(double dt) {
    if(mSpawner) {
        if(mCasting && mCastTimer.Update(dt)) {
            mCasted = true;
            mCasting = false;
        }
        if(mCasted) {
            mSpawner->Update(dt);
        }
    }
}

bool MagicBullet::CheckAndResolveCollision(BackgroundTile2& ground)
{
    if(mSpawner) {
        mSpawner->CheckAndResolveCollision(ground);
    }
}

bool MagicBullet::CheckAndResolveCollision(RigidBody* b)
{
    if(mSpawner) {
        mSpawner->CheckAndResolveCollision(b);
    }
}

bool MagicBullet::CheckAndResolveCollision(WaterBody& b)
{
    if(mSpawner) {
        mSpawner->CheckAndResolveCollision(b);
    }
}

void MagicBullet::ResetAnimations()
{
    if(mSpawner) {
        mSpawner->ResetAnimations();
    }
}

bool MagicBullet::Finished()
{

}
void MagicBullet::Render(SceneRenderer& rend) {
    if(mSpawner) {
        mSpawner->Render(rend);
    }
}
void MagicBullet::DrawHitBoxes(SceneRenderer& rend, Color color) {
    if(mSpawner) {
        mSpawner->DrawHitBoxes(rend, color);
    }
}

void MagicBullet::Reset()
{
    mCastTimer.Reset();
    if(mSpawner) {
        mSpawner->Reset();
    }
    mCasting = true;
    mCasted = false;
}
}
