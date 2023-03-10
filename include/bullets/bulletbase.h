#ifndef BULLETBASE_H_INCLUDED
#define BULLETBASE_H_INCLUDED

#include <functional>
#include "particle/particleeffect.h"
#include "physics/physicsrect.h"
#include "kaguya/kaguya.hpp"
#include "animation/animation.h"
#include "geometry/hitdetection.h"

namespace solstice {
void SetupBulletBinds(kaguya::State& state);
enum BulletShotType {BTYPE_PROJECTILE, BTYPE_SUSTAINED, BTYPE_LASER, BTYPE_MELEE, BTYPE_MAGIC, BTYPE_THROWER, BTYPE_MISSILE, BTYPE_WATERHOOK, BTYPE_EXPANDING, BTYPE_FLURRY};
struct BulletState {
    Vector2f mPosition;
    glm::vec3 mVelocity;
};
class BulletBase : public Particle {
protected:
    int mType;
    bool mShot, mAlive;
    int mMaxBounces, mBounces;
    Rectanglef mHitBox;
    Circlef mCircleHitBox;
    Vector2f mCollisionPoint;
    CollisionLocation mCollisions;
    bool mDestroyAnimsFinished;
    bool mCollisionFound;
    virtual void ResolveCollision(Collision& c, float friction);
public:
    std::vector<Animation> mAnimations, mDestroyAnimations;
    Vector2f mImpactForce, mKickback, mKnockback;
    int mSelfDamage;
    int mDamage;
    float mHitStun, mHitStop;
    bool mTransfersVelocity, mTransfersPosition, mEmitsOnImpact;
    bool mDirectsImpact;
    ParticleEffect mEmitter, mImpactEmitter;
    int mCollisionType;
    float mElasticityX, mElasticityY;
    float mBlastForce;
    float mSpriteAngle;
    bool mRotatesSprite, mFlipSprite;
    float mAutoDeathDur;
    BulletState mResetState;
    std::string mName;
    int mID;
    Vector2f mSprayerOffset;

    Vector2f mStartDirection;

    BulletBase();

    int GetType() {return mType;}
    bool GetShot() {return mShot;}
    bool GetAlive() {return mAlive;}
    int GetMaxBounces() {return mBounces;}
    int GetBounces() {return mBounces;}

    void SetBounces(int max) {mMaxBounces = max; mBounces = 0;}
    Rectanglef GetHitBox() {return mHitBox;}

    virtual void SetPosition(Vector2f pos);
    virtual void SetPosition(glm::vec3 pos);
    virtual Vector2f GetPosition() {return mHitBox.GetPosition();}
    virtual Vector2f GetHitBoxPos() {return mHitBox.GetCenter();}
    void SetCircleHitBoxRadius(float r) {mCircleHitBox.SetSize(r);}
    void SetHitboxHW(float h, float w) {mHitBox.SetHW(h, w);}
    void SetDrawPoint(DrawPoint pt)   {mHitBox.SetDrawFrom(pt);}

    Vector2f GetCollisionPoint() {return mCollisionPoint;}
    CollisionLocation GetCollisions() {return mCollisions;}

    virtual void Shoot(Vector2f velocity);
    virtual void Shoot(Vector2f velocity, Vector2f position);
    virtual void ShootAngle(float angle, float vel);
    virtual void SetCollisionPoint(Vector2f point);
    virtual void Destroy(Vector2f force = 0);

    virtual void Update(double dt);
    virtual void UpdateAnimation(float dt);
    virtual void Reset();
    virtual void SetState(BulletState b);
    virtual void SaveInitialState();

    virtual bool CheckAndResolveCollision(BackgroundTile2& ground);
    virtual bool CheckAndResolveCollision(RigidBody* b);
    virtual bool CheckAndResolveCollision(WaterBody& b);
    virtual void ResetAnimations();
    virtual bool Finished();
    virtual bool Explodes() {return false;}
    virtual void CheckExplosion(RigidBody* obj) {}

    virtual void Draw(SceneRenderer& rend);
    virtual void DrawHitBoxes(SceneRenderer& rend, Color col);

    friend void SetupBulletBinds(kaguya::State& state);
    virtual void AimTowards(Vector2f pos);

    ParticleEffect& GetEmitter()                {return mEmitter;}
    ParticleEffect* GetEmitterPtr()             {return &mEmitter;}
    ParticleEffect& GetImpactEmitter()          {return mImpactEmitter;}
    ParticleEffect* GetImpactEmitterPtr()       {return &mImpactEmitter;}
    void SetEmitter(ParticleEffect e)           {mEmitter = e;}
    void SetImpactEmitter(ParticleEffect e)     {mImpactEmitter = e;}
};
}


#endif // BULLETBASE_H_INCLUDED
