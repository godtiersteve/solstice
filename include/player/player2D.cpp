#include "player2D.h"

namespace solstice {
Player2D::Player2D(Rectanglef hitbox, float walkSpeed) {
    mHitBox = hitbox;
    mWalkSpeed = walkSpeed;
    mWalkSpeedMax = 8;
    mMaxVelocity = 30;
    mUpHeld = false;
    mDownHeld = false;
    mRightHeld = false;
    mLeftHeld = false;
    mJumpHeld = false;
    mHoldJump = 0;
    mJumpStrength = 7;
    mHoldJumpFrames = 0;
    mMinJumpFrames = 10;
    mMaxHoldJumpFrames = 15;
    mHoldJumpStrength = 0.50;
    mGravity = 0.25;
}

void Player2D::InitHitBox(Vector2f pos, float h, float w) {
    mHitBox.SetXYHW(pos, h, w);
}

void Player2D::Update(double dt) {
    if(mHoldJump && !mOnGround) {
        mHoldJumpFrames++;
    }
    Fall();
    OnArrowsDown();
    mVelocity+= mAcceleration;
    mHitBox.AddXY(mVelocity);
    mOnGround = false;
}
void Player2D::Fall() {
    if(mVelocity.y > -mMaxVelocity.y) {
        mVelocity.y-= mGravity;
    }
}
void Player2D::Walk() {
}
void Player2D::Move(Vector2f distance) {

}

void Player2D::MoveTo(Vector2f pos) {

}

void Player2D::AddXY(Vector2f xy) {
    mHitBox.AddXY(xy);
}

void Player2D::AddX(float x) {
    mHitBox.AddX(x);
}
void Player2D::Stop() {
    mVelocity = 0;
    mAcceleration = 0;
}
void Player2D::StopX() {
    mVelocity.x = 0;
    mAcceleration.x = 0;
}
void Player2D::StopY() {
    mVelocity.y = 0;
    mAcceleration.y = 0;
}
void Player2D::AddY(float y) {
    mHitBox.AddY(y);
}
void Player2D::OnKeyUp(PlayerKey key) {
}
void Player2D::OnKeyDown(PlayerKey key) {
}
void Player2D::OnArrowsDown() {
    if(mRightHeld && mVelocity.x < mWalkSpeedMax) {
        mVelocity.x+= mWalkSpeed;
    }
    else if(mVelocity.x > 0) {
        mVelocity.x-= std::min(mWalkSpeed, mVelocity.x);
    }
    if(mLeftHeld && mVelocity.x > -mWalkSpeedMax) {
        mVelocity.x-= mWalkSpeed;
    }
    else if(mVelocity.x < 0) {
        mVelocity.x+= std::min(double(mWalkSpeed), fabs(mVelocity.x));
    }
    if(mJumpHeld) {
        Jump();
    }
}
void Player2D::OnArrowsUp() {
}
void Player2D::CheckKeyUp(SDL_Keycode key) {
    switch(key) {
        case SDLK_LEFT:
            mLeftHeld = false;
            break;
        case SDLK_RIGHT:
            mRightHeld = false;
            break;
        case SDLK_LSHIFT:
            mJumpHeld = false;
            break;
        default:
            break;
    }
}
void Player2D::CheckKeyDown(SDL_Keycode key) {
    switch(key) {
        case SDLK_LEFT:
            mLeftHeld = true;
            break;
        case SDLK_RIGHT:
            mRightHeld = true;
            break;
        case SDLK_LSHIFT:
            mJumpHeld = true;
            break;
        default:
            break;
    }
}
void Player2D::Jump() {
    if(mOnGround) {
        mVelocity.y+= mJumpStrength;
    }
    else if(mHoldJump) {
        if(mHoldJumpFrames >= mMinJumpFrames && mHoldJumpFrames < (mMaxHoldJumpFrames + mMinJumpFrames)) {
            mVelocity.y+= mHoldJumpStrength;
        }
        if(mHoldJumpFrames >= (mMaxHoldJumpFrames + mMinJumpFrames)) {
            mJumpHeld = false;
        }
    }
}
Rectanglef Player2D::GetHitBox() {return mHitBox;}

}
