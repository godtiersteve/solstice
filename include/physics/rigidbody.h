#ifndef RIGIDBODY_H_INCLUDED
#define RIGIDBODY_H_INCLUDED

#include "geometry/rectangle.h"
#include "environment/backgroundtile.h"
#include "graphics/vector2.h"
#include "geometry/collision.h"
#include "geometry/hitdetection.h"
#include "environment/waterbody.h"
#include "movableobject.h"

#include <glm/glm.hpp>

namespace solstice {
class RigidBody : public MovableObject {
protected:
    std::string mName;
    int mCollisionLocations[4];
    float mInvMass, mInertia;
    float mInvMass2;
    float mFriction;
    float mElasticity;
    float mBuoyancy;
    float mRotVelocity, mLastRotVelocity, mMaxRotVel;
    float mStep;
    float mLastAngle, mAngleDamping;
    bool mCollisionFound;
    bool mNoTorque, mNoPlatform, mIsPlatform, mNoFloat;
    Vector2f mVelocity, mAcceleration;
    Rectanglef mHitBox;
    float mZOffset;
    size_t mID;
    int mGroundedCheck;
    static size_t sIDs;
    virtual void ApplyForces(float tstep = 1.0f);
    virtual void AdjustVelocity() {}
    virtual void AdjustVelocity(Vector2f& vel) {}
    virtual void AdjustAngle(float& angle) {}
    virtual void CheckPlatform(Collision& c, BackgroundTile2& bg);
    virtual void CalcTorque(Vector2f vel, Collision& c, BackgroundTile2& ground);
    virtual void CalcTorque(Vector2f vel, Vector2f contactPoint);
public:
    RigidBody();
    RigidBody(float invMass, float fric, float elast, Vector2f vel);
    void SetDefaults();
    RigidBody(BackgroundTile2 tile);
    void SetMass(float m)                               {mInvMass2 = 1.0f / m;}
    void SetInvMass(float m)                            {mInvMass2 = m;}
    void SetInertia(float i)                            {mInertia = i;}
    void SetVelocity(Vector2f v)                        {mVelocity = v;}
    void SetElasticity(float e)                         {mElasticity = e;}
    void SetFriction(float f)                           {mFriction = f;}
    void SetBuoyancy(float b)                           {mBuoyancy = b;}
    void SetAcceleration(Vector2f v)                    {mAcceleration = v;}
    void SetAngle(float a)                              {mLastAngle = a; mHitBox.SetAngle(a);}
    void SetLastAngle(float a)                          {mLastAngle = a;}
    void SetNoTorque(bool t)                            {mNoTorque = t;}
    void SetUnderwater(float f)                         {mUnderwater = f;}
    void SetAngleDamping(float a)                       {mAngleDamping = a;}
    void SetMaxRotVel(float a)                          {mMaxRotVel = a;}
    bool GetUnderWater()                                {return mUnderwater;}
    bool GetNoPlatform()                                {return mNoPlatform;}
    bool GetIsPlatform()                                {return mIsPlatform;}
    float GetSubmergeRatio()                            {return mSubmergeDepth / mHitBox.GetHeight();}
    virtual void SetPosition(Vector2f pos);
    virtual void SetY(float y)                                  {mHitBox.SetY(y); mLastPos.y = GetPosition().y;}
    virtual void SetX(float x)                                  {mHitBox.SetX(x); mLastPos.x = GetPosition().x;}
    virtual void AddXY(Vector2f mv);
    virtual void AddX(float x);
    virtual void AddY(float y);
    void Move(Vector2f mv);
    void MoveTo(Vector2f mv);
    void DampVelocity(float damp);
    void DampX(float damp);
    void DampY(float damp);
    void ClearAcceleration()                            {mAppliedForce = 0.0f;}
    virtual Rectanglef& GetHitBox()                     {return mHitBox;}
    virtual Rectanglef* GetHitBoxPtr()                  {return &mHitBox;}
    virtual IShapef* GetHitBoxShape()                   {return &mHitBox;}
    float GetMass()                                     {return mInvMass2 ? 1.0f / mInvMass2 : 0;}
    float GetInertia()                                  {return mInertia;}
    float GetInvMass()                                  {return mInvMass2;}
    float GetRotationSpeed()                            {return mHitBox.GetAngle() - mLastAngle;}
    Vector2f& GetVelocity()                             {return mVelocity;}
    Vector2f GetVelocity2()                             {return GetPosition() - mLastPos;}
    Vector2f& GetAcceleration()                         {return mAcceleration;}
    Vector2f GetMomentum()                              {return mInvMass2 ? (1.0f) / mInvMass2 * mVelocity : 0;}
    bool GetNoTorque()                                  {return mNoTorque;}
    std::string GetName()                               {return mName;}
    Vector2f GetInvMomentum();
    float GetElasticity()                               {return mElasticity;}
    float GetFriction()                                 {return mFriction;}
    float GetBuoyancy()                                 {return mBuoyancy;}
    float GetStep()                                     {return mStep;}
    float GetLastAngle()                                {return mLastAngle;}
    float GetAngleDamping()                             {return mAngleDamping;}
    size_t GetID()                                      {return mID;}
    float GetRotVelocity()                              {return mRotVelocity;}
    float GetRotVel2()                                  {return mHitBox.GetAngle() - mLastAngle;}
    float GetMaxRotVel()                                {return mMaxRotVel;}
    void ResetGrounded()                                {if(mGroundedCheck >= 2) {mGroundedCheck = 0; mGroundedCheck = false;}}
    void ApplyRotImpulse(float v)                       {mLastAngle-= v;}
    void AddRotVelocity(float v);
    void SetRotVelocity(float v)                        {mRotVelocity = v;}
    void KillRotVel()                                   {mRotVelocity = 0.0f; mLastAngle = mHitBox.GetAngle();}
    void Stop();
    void StopX();
    void StopY();
    static size_t GetIDs()                              {return sIDs;}
    void SetAppliedForce(Vector2f f)                    {mAppliedForce = f;}
    virtual void SetGrounded(bool g)                            {mGrounded = g;}
    void SetName(std::string n)                         {mName = n;}
    void SetNoPlatform(bool n)                          {mNoPlatform = true;}
    void SetIsPlatform(bool i)                          {mIsPlatform = i;}
    void SetZOffset(float z)                            {mZOffset = z;}
    float GetZOffset()                                  {return mZOffset;}
    bool GetCollisionFound()                            {return mCollisionFound;}
    void SetCollisionFound(bool f)                      {mCollisionFound = f;}
    bool GetCollisionLocation(int loc)                  {return mCollisionLocations[loc];}
    void SetCollisionLocation(int loc, bool flag)       {mCollisionLocations[loc] = flag;}
    void SetNoFloat(bool nofloat)           {mNoFloat = nofloat;}
    bool GetNoFloat()                   {return mNoFloat;}
    void ResetCollisionLocs();
    void CheckCollisions(Vector2f mtv);

    virtual void InitHitBox(Vector2f pos, float h, float w, int drawFrom = DRAW_CENTER);
    void SetAttributes(float invMass, float buoyancy, float elast);
    void VerletNoVelocity(float dt);
    void Update(double dt) {VerletNoVelocity(dt);}
    void ResetFlags();

    virtual Vector2f GetPosition()                      {return mHitBox.GetPosition();}
    virtual void AddVelocityX(float x)                  {mVelocity.x+= x;}
    virtual void AddVelocityY(float y)                  {mVelocity.y+= y;}
    virtual void AddVelocity(Vector2f velocity)         {mVelocity+= velocity;}
    virtual void CheckAndResolveCollision(RigidBody* b);
    virtual void CheckAndResolveCollision(BackgroundTile2& ground);
    virtual bool CheckCollision(WaterBody& body)        {return false;}
    virtual Vector2f CalcVelocity(RigidBody* b, Collision& c);
    virtual Vector2f CalcVelocity(BackgroundTile2& ground, Collision& c);
    virtual Vector2f CalcVelocity(Vector2f vel, Collision& col, float friction, float elasticity);
    virtual void ResolveCollision(Collision& c, float mass, Vector2f vel);
    virtual void ResolveCollision(RigidBody* b, Collision& c, Vector2f vel);
    virtual IShapef* GetHitShapef()     {return &mHitBox;}
    virtual void OnCollision(Collision c) {}
};
}


#endif // RIGIDBODY_H_INCLUDED
