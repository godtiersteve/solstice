#ifndef BULLET_FLURRY_H_INCLUDED
#define BULLET_FLURRY_H_INCLUDED

#include "bulletbase.h"
#include "bullet_melee.h"

namespace solstice {
class FlurryBullet : public BulletBase {
private:
    std::deque<Animation> mSlashAnimations;
    Animation NextSlash(int id = -1);
    bool mFinishing;
    int mRepsPerformed;
public:
    int mMaxSlashes;
    float mCreateDelay;
    MeleeBullet mFinisher;
    FlurryBullet(int numSlashes = 2);
    FlurryBullet(std::vector<Animation>& slashAnimations, int numSlashes = 2);

    void Shoot(Vector2f velocity);
    void Shoot(Vector2f velocity, Vector2f position);
    void Destroy(Vector2f force = 0);

    void Update(double dt);
    void UpdateAnimation(float dt);
    void Reset();

    bool CheckAndResolveCollision(BackgroundTile2& ground) {return false;}
    bool CheckAndResolveCollision(RigidBody* b);
    bool CheckAndResolveCollision(WaterBody& b) {return false;}
    void ResetAnimations()  {BulletBase::ResetAnimations(); mSlashAnimations.clear();}
    int GetRepsPerformed();
    void ResetReps() {mRepsPerformed = 0;}
    //bool Finished();

    void Draw(SceneRenderer& rend);
    void DrawHitBoxes(SceneRenderer& rend, Color col);
    void EndFlurry();
    bool Finished();
    bool GetFinishing() {return mFinishing;}
};
}

#endif // BULLET_FLURRY_H_INCLUDED
