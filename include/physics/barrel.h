#ifndef BARREL_H_INCLUDED
#define BARREL_H_INCLUDED

#include "physics/physicsrect.h"
#include "graphics/3d/model.h"
#include "engine/healthentity.h"
#include "graphics/renderableobject.h"

namespace solstice {
class Barrel : public PhysicsBox2f, public RenderableObject {
protected:
    Circlef mCircleHitBox;
    bool mKnockedOver, mBroken;
public:
    Model mModel;
    HealthEntity mHealth;
    HealthEntity mWaterValue;
    float mUprightHeight, mTiltedHeight;
    Barrel();
    Barrel(Vector2f pos, Rectanglef hitbox, Circlef circleHitBox);
    virtual void Update(float tstep = 1.0f);
    virtual void UpdateVerlet(float dt);
    //virtual bool CheckCollision(WaterBody& waterBody);
    //virtual void CheckPlatform(Collision& c, BackgroundTile2& bg);
    bool GetKnockedOver() {return mKnockedOver;}
    bool GetBroken() {return mBroken;}
    void Break();
    void KnockOver(Vector2f force = 0);
    void Right();
    IShapef* GetHitBoxShape();
    IShapef* GetHitShapef();
    Circlef GetCircleHitBox() {return mCircleHitBox;}
    void SetPosition(Vector2f pos);
    Vector2f GetPosition() {return mHitBox.GetPosition();}
    void SetY(float y);
    void SetX(float x);
    void AddXY(Vector2f mv);
    void AddX(float x);
    void AddY(float y);
    void CheckAndResolveCollision(RigidBody* b);
    void CheckAndResolveCollision(BackgroundTile2& ground);
    Vector2f CalcVelocity(RigidBody* b, Collision& c);
    Vector2f CalcVelocity(BackgroundTile2& ground, Collision& c);
    Vector2f CalcVelocity(Vector2f vel, Collision& col, float friction, float elasticity);
};
}

#endif // BARREL_H_INCLUDED
