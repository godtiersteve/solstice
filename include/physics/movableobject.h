#ifndef MOVABLEOBJECT_H_INCLUDED
#define MOVABLEOBJECT_H_INCLUDED

#include "graphics/vector2.h"
#include "engine/gameobject.h"
#include "geometry/shape.h"

namespace solstice {
enum MovementState {MOVE_GROUNDED, MOVE_FLYING, MOVE_SWIMMING_UNDERWATER, MOVE_SWIMMING_SURFACE};
class MovableObject : public GameObject {
protected:
    Vector2f mPosition, mLastPos;
    Vector2f mAppliedForce, mAppliedImpulse;
    MovementState mState;
    bool mGrounded, mUnderwater;
    float mSubmergeDepth;
    int mCollisions;
public:
    virtual void Move(Vector2f distance) = 0;
    virtual void MoveTo(Vector2f pos) = 0;
    virtual void SetPosition(Vector2f pos)  {mPosition = pos; mLastPos = pos;}
    virtual void Stop() {mAppliedForce = 0; mAppliedImpulse = 0; SetPosition(mPosition);}
    virtual void StopX() {mAppliedForce.x = 0; mAppliedImpulse.x = 0; SetPosition(Vector2f(mPosition.x, mPosition.y - mLastPos.y));}
    virtual void StopY() {mAppliedForce.y = 0; mAppliedImpulse.y = 0; SetPosition(Vector2f(mPosition.x - mLastPos.x, mPosition.y));}
    Vector2f GetVelocity()      {return mPosition - mLastPos;}
    virtual Vector2f GetVelocity2()           {return mPosition - mLastPos;}
    Vector2f& GetLastPosition()     {return mLastPos;}
    virtual Vector2f GetPosition() {return mPosition;}
    virtual void ApplyForce(Vector2f force) {mAppliedForce+= force;}
    virtual void ApplyForceX(float x) {mAppliedForce.x+= x;}
    virtual void ApplyForceY(float y) {mAppliedForce.y+= y;}
    virtual void ApplyImpulse(Vector2f impulse) {mAppliedImpulse+= impulse;}
    virtual void ApplyImpulseX(float x)          {mAppliedImpulse.x+=x;}
    virtual void ApplyImpulseY(float y)           {mAppliedImpulse.y+=y;}
    virtual void ChangeState(MovementState state)   {mState = state;}
    virtual void AddXY(Vector2f xy) = 0;
    virtual void AddX(float x) = 0;
    virtual void AddY(float y) = 0;
    bool CheckState(MovementState s)                {return mState == s;}
    MovementState GetState()                    {return mState;}
    virtual void Update(double dt)       = 0;
    Vector2f GetAppliedForce()          {return mAppliedForce;}
    Vector2f GetAppliedImpulse()        {return mAppliedImpulse;}
    bool GetGrounded()                  {return mGrounded;}
    float GetSubmergeDepth()            {return mSubmergeDepth;}
    float GetSubmergeRatio()            {return mSubmergeDepth / GetHeight();}
    virtual float GetHeight() {return 1;}
    virtual float GetWidth() {return 1;}
    virtual float GetArea()                     {return 1.0;}
    bool GetUnderwater()                {return mUnderwater;}
    virtual IShapef* GetHitShapef()     {return nullptr;}
};

}


#endif // MOVABLEOBJECT_H_INCLUDED
