#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <functional>
#include "particleeffect.h"
#include "physics/physicsrect.h"
#include "kaguya/kaguya.hpp"
#include "animation/animation.h"

namespace solstice {
void SetupBulletBinds(kaguya::State& state);
enum BulletShotType {BTYPE_PROJECTILE, BTYPE_SUSTAINED, BTYPE_LASER, BTYPE_MELEE};
class Bullet : public Particle {
protected:
    int mType, mActionDelay;
    ParticleEffect mEmitter, mImpactEmitter;
    bool mShot, mAlive, mTransfersVelocity, mTransfersPosition, mEmitsOnImpact;
    int mMaxBounces, mBounces;
    int mCollisionType;
    float mElasticityX, mElasticityY;
    Rectanglef mHitBox;
    Vector2f mImpactForce;
    float mBlastForce;
    Circlef mLockOnRadius, mBlastRadius;
    bool mCollisionFound, mDirectsImpact;
    bool mLocksOn, mTargetFound, mAdjustsAngle;
    float mLockOnVel, mLockOnStartVel;
    float mTrackingSpeed;
    Vector2f mLockOnDir;
    float mLockOnSpeed, mLockOnAccel;
    std::vector<Animation> mAnimations;
    bool mRotatesSprite;
    float mSpriteAngle;
    Vector2f mCollisionPoint;
    float mAutoDeathDur;
    kaguya::LuaRef mUpdateFunc, mCollisionFunc, mBurstFunc;
    float GetTrackedAngle(float a);
    Vector2f mLockOnPoint;
    bool mWaterStops, mBouncesTowards, mCreatesSubBullets;
    int mCreateSubState;
public:
    Bullet();
    void Update(float dt);
    void UpdateAnimation(float dt);
    void DefaultUpdate(float dt);
    bool CheckAndResolveCollision(BackgroundTile2& ground);
    bool CheckAndResolveCollision(RigidBody* b);
    bool CheckAndResolveCollision(WaterBody& b);
    void ResolveCollision(Collision& c, float friction);
    void DefaultResolveCollision(Collision& c, float friction);
    void Init(kaguya::State& state, std::string package)         {state[package]["Init"](this);}
    Vector2f ResolveBlast(Vector2f point);

    void Destroy(Vector2f force = 0);
    void RotateSprite();
    bool Alive() {return mAlive;}
    bool Finished();
    ParticleEffect& GetEmitter()                {return mEmitter;}
    ParticleEffect* GetEmitterPtr()             {return &mEmitter;}
    ParticleEffect& GetImpactEmitter()          {return mImpactEmitter;}
    ParticleEffect* GetImpactEmitterPtr()       {return &mImpactEmitter;}
    void SetEmitter(ParticleEffect e)           {mEmitter = e;}
    void SetImpactEmitter(ParticleEffect e)     {mImpactEmitter = e;}

    void SetEmitsOnImpact(bool e)               {mEmitsOnImpact = e;}
    void SetTransfersVelocity(bool f)           {mTransfersVelocity = f;}
    void SetTransfersPosition(bool f)           {mTransfersPosition = f;}
    void SetMaxBounces(int m)                   {mMaxBounces = m;}

    bool GetEmitsOnImpact()                     {return mEmitsOnImpact;}
    int GetMaxBounces()                         {return mMaxBounces;}
    int GetBounces()                            {return mBounces;}
    bool GetTransfersVelocity()                 {return mTransfersVelocity;}
    bool GetTransfersPosition()                 {return mTransfersPosition;}

    void Draw(ShaderProgram& shader, TextureArray& textures);

    void Reset();
    void Reset(Vector2f pos);
    void InitHitBox(Vector2f pos, float h, float w, int drawFrom = DRAW_CENTER);
    void SetVelocity(Vector2f vel)                              {mVelocity.x = vel.x; mVelocity.y = vel.y;}
    void ApplyImpulse(Vector2f i)                               {mVelocity.x+= i.x; mVelocity.y+= i.y;}
    void ApplyImpulseX(float x)                                 {mVelocity.x+= x;}
    void ApplyImpulseY(float y)                                 {mVelocity.y+= y;}
    void ApplyForce(Vector2f f)                                 {mAcceleration.x+= f.x; mAcceleration.y+= f.y;}
    void ApplyForceX(float x)                                   {mAcceleration.x+= x;}
    void ApplyForceY(float y)                                   {mAcceleration.y+= y;}
    void SetElasticity(float e)                                 {mElasticityX = e; mElasticityY = e;}
    void SetElasticityX(float e)                                {mElasticityX = e;}
    void SetElasticityY(float e)                                {mElasticityY = e;}
    void SetLockOnAccel(float a)                                {mLockOnAccel = a;}
    void SetLocksOn(bool l)                                     {mLocksOn = l;}
    void SetSpriteAngle(float a)                                {mSpriteAngle = a;}
    void SetHitBoxPos(Vector2f pos)                             {mHitBox.SetPosition(pos); mBlastRadius.SetPosition(pos); mLockOnRadius.SetPosition(pos);}
    void SetPosition(Vector2f pos)                              {mHitBox.SetPosition(pos); mPosition.x = pos.x; mPosition.y = pos.y;mBlastRadius.SetPosition(pos); mLockOnRadius.SetPosition(pos);}
    void SetPosition(glm::vec3 pos)                             {mHitBox.SetPosition(Vector2f(pos.x, pos.y)); mPosition = pos;}
    void SetLockOnVel(float vel)                                {mLockOnVel = vel;}
    void SetAdjustsAngle(bool a)                                {mAdjustsAngle = a;}
    void SetRotatesSprite(bool r)                               {mRotatesSprite = r;}
    void SetLockOnStartVel(float s)                             {mLockOnStartVel = s;}
    void SetImpactForce(Vector2f f)                             {mImpactForce = f;}
    void SetBlastForce(float f)                                 {mBlastForce = f;}
    void SetDirectsImpact(bool d)                               {mDirectsImpact = d;}
    void SetLockOnPoint(Vector2f pt)                            {mLockOnPoint = pt;}
    Vector2f GetLockOnPoint()                                   {return mLockOnPoint;}
    int GetType()                                               {return mType;}
    void SetType(int type)                                      {mType = type;}
    void SetTrackingSpeed(float t)                              {mTrackingSpeed=  t;}
    float GetTrackingSpeed()                                    {return mTrackingSpeed;}
    void SetWaterStops(bool w)                                  {mWaterStops = w;}
    bool GetWaterStops()                                        {return mWaterStops;}
    void SetCollisionType(int i)                                {mCollisionType = i;}
    int GetCollisionType()                                      {return mCollisionType;}
    void SetBouncesTowards(bool b)                              {mBouncesTowards = b;}
    bool GetBouncesTowards()                                    {return mBouncesTowards;}
    void SetAutoDeathDur(float d)                               {mAutoDeathDur = d;}
    float GetAutoDeathDur()                                     {return mAutoDeathDur;}
    void SetCreatesSubBullets(bool b)                           {mCreatesSubBullets = b;}
    bool GetCreatesSubBullets()                                 {return mCreatesSubBullets;}
    int GetCreateSubState()                                     {return mCreateSubState;}
    void SetCreateSubState(int i)                               {mCreateSubState = i;}
    void SetActionDelay(int d)                                  {mActionDelay = d;}
    int GetActionDelay()                                        {return mActionDelay;}
    void SetCollisionPoint(Vector2f point);
    void Shoot(Vector2f velocity);
    void Shoot(Vector2f velocity, Vector2f position);
    void ShootAngle(float angle, float vel);
    void SetHoriFlip(bool f);
    void SetVertFlip(bool f);
    void InitEmitters();

    Rectanglef& GetHitBox()                                     {return mHitBox;}
    Circlef& GetLockOnRadius()                                  {return mLockOnRadius;}
    Circlef& GetBlastRadius()                                   {return mBlastRadius;}
    float GetElasticityX()                                      {return mElasticityX;}
    float GetElasticityY()                                      {return mElasticityY;}
    bool GetLocksOn()                                           {return mLocksOn;}
    Vector2f GetHitBoxPos()                                     {return mHitBox.GetPosition();}
    bool GetShot()                                              {return mShot;}
    bool DestroyFinished()                                      {return mImpactEmitter.Finished();}
    float GetLockOnVel()                                        {return mLockOnVel;}
    bool GetAdjustsAngle()                                      {return mAdjustsAngle;}
    bool GetRotatesSprite()                                     {return mRotatesSprite;}
    float GetBlastForce()                                       {return mBlastForce;}
    bool PerformedMaxBounces()                                  {return mBounces >= mMaxBounces;}
    bool GetDirectsImpact()                                     {return mDirectsImpact;}
    float GetLockOnStartVel()                                   {return mLockOnStartVel;}

    //Animation& GetAnimation()                                   {return mAnimation;}
    //Animation& GetDestroyAnim()                                 {return mDestroyAnim;}

    void SetDestroyAnim(Animation anim);
    void AddAnimation(Animation anim);
    void SetAnimation(size_t i, Animation anim);
    Animation& GetSpriteAnimation(size_t i)                           {return mAnimations[i];}
    Animation* GetSpriteAnimationPtr(size_t i)                        {return i < mAnimations.size() ? &mAnimations[i] : NULL;}

    void AimTowards(Vector2f pos);
    void SetScript(kaguya::State& state, std::string update, std::string collision);
    void SetScript(kaguya::State& state, std::string package);
    friend void SetupBulletBinds(kaguya::State& state);
    void Draw(SceneRenderer& rend);
};
}

#endif // BULLET_H_INCLUDED
