#ifndef PLAYER2D_H_INCLUDED
#define PLAYER2D_H_INCLUDED

#include "physics/movableobject.h"
#include "geometry/rectangle.h"
#include "animation/animation.h"
#include "graphics/3d/skeletalmodel.h"
#include "graphics/3d/modelanimset.h"
#include "engine/healthentity.h"
#include "actionkey.h"
#include "sdl.h"
#include <vector>
#include <string>

namespace solstice {
enum PlayerOrientation {FACING_RIGHT, FACING_LEFT, FACING_UP, FACING_DOWN};
class Player2D : public MovableObject {
protected:
    Rectanglef mHitBox;
    bool mGrounded;
    PlayerOrientation mOrientation;
    HealthEntity mHealth;
    HealthEntity mMana;
    Vector2f mVelocity, mAcceleration;
    bool mUpHeld, mDownHeld, mLeftHeld, mRightHeld, mJumpHeld;
    bool mOnGround, mOnPlatform;
    int mHoldJumpFrames;
    void Fall();
    void Walk();
    virtual void OnKeyUp(PlayerKey key);
    virtual void OnKeyDown(PlayerKey key);
    virtual void OnArrowsDown();
    virtual void OnArrowsUp();
    virtual void Jump();
public:
    float mWalkSpeed, mGravity, mWalkSpeedMax;
    float mJumpStrength, mHoldJumpStrength;
    int mMaxHoldJumpFrames, mMinJumpFrames;
    float mBuoyancy;
    bool mLandStop, mAirAccels;
    bool mHoldJump;
    Vector2f mMaxVelocity;

    Player2D(Rectanglef hitbox, float walkSpeed);
    virtual void InitHitBox(Vector2f pos, float h, float w);
    virtual void Update(double dt);
    virtual void Move(Vector2f distance);
    virtual void MoveTo(Vector2f pos);
    virtual void AddXY(Vector2f xy);
    virtual void AddX(float x);
    virtual void AddY(float y);
    void Stop();
    void StopX();
    void StopY();
    Rectanglef GetHitBox();
    Vector2f GetVelocity() {return mVelocity;}
    Vector2f GetAcceleration() {return mAcceleration;}
    virtual void CheckKeyUp(SDL_Keycode key);
    virtual void CheckKeyDown(SDL_Keycode key);
    void SetPosition(Vector2f pos) {mHitBox.SetPosition(pos);}
    void SetOnGround(bool g) {mOnGround = g; if(g) {mHoldJumpFrames = 0;}}
    bool GetOnGround() {return mOnGround;}
    bool GetUnderwater() {return mUnderwater;}

};
}

#endif // PLAYER2D_H_INCLUDED
