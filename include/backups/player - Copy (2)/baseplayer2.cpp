#include "baseplayer2.h"

namespace solstice {
BasePlayer2::BasePlayer2() {
    Init();
}
BasePlayer2::BasePlayer2(Rectanglef hitbox) {
    Init();
    mHitBox = hitbox;
    mLastPos = hitbox.GetPosition();
}
void BasePlayer2::Init() {
    mActionKeys.clear();
    mActionKeys.push_back(ActionKey(SDLK_LEFT, PKEY_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT));
    mActionKeys.push_back(ActionKey(SDLK_RIGHT, PKEY_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
    mActionKeys.push_back(ActionKey(SDLK_UP, PKEY_UP, SDL_CONTROLLER_BUTTON_DPAD_UP));
    mActionKeys.push_back(ActionKey(SDLK_DOWN, PKEY_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN));
    mActionKeys.push_back(ActionKey(SDLK_LSHIFT, PKEY_JUMP, SDL_CONTROLLER_BUTTON_A));
    mActionKeys.push_back(ActionKey(SDLK_c, PKEY_ATTACK, SDL_CONTROLLER_BUTTON_X));
    mActionKeys.push_back(ActionKey(SDLK_x, PKEY_SHOOT, SDL_CONTROLLER_BUTTON_B));
    mActionKeys[PKEY_LEFT].SetAnalog(SDL_CONTROLLER_BUTTON_LEFTSTICK_X, -1600);
    mActionKeys[PKEY_RIGHT].SetAnalog(SDL_CONTROLLER_BUTTON_LEFTSTICK_X, 1600);
    mActionKeys[PKEY_UP].SetAnalog(SDL_CONTROLLER_BUTTON_LEFTSTICK_Y, 1600);
    mActionKeys[PKEY_DOWN].SetAnalog(SDL_CONTROLLER_BUTTON_LEFTSTICK_Y, -1600);
    mHitStun = 0;
    mHitStunCounter = 0;
    mJumpStrength = 25;
    mHoldJumpStrength = 0;
    mWalkSpeed =  10;
    mWalkAccel = 1;
    mLandStop = false;
    mAirAccels = true;
    mHoldJump = false;
    mHoldJumpFrames = 0;
    mHoldJumpTimer = 0;
    mWaterJumpStrength = 0.75f;
    mJumped = false;
    mFlipped = false;
    mIgnoreInput = false;
    mIgnorePhysics = false;
    mRightHeldFrames = 0;
    mLeftHeldFrames = 0;
    mJumping = false;
    mUsesModel = false;
    mSlopeForce = 0.0f;
    mSpecialAnim = false;
    mCancelableSpecial = false;
    mHurling = false;
    mKnockdownTime = false;
    mMaxKnockdownTime = 1.0f;
    mKnockedDown = false;
    mHealth = 100;
    mMagic = 100;
    mMaxHealth = 100;
    mMaxMagic = 100;
}
Animation& BasePlayer2::GetAnimation(string anim) {
    for(size_t i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetRef() == anim) {
            return mAnimations[i];
        }
    }
    return mAnimations[0];
}
void BasePlayer2::SetHoldJump(float strength, int duration) {
    mHoldJump = true;
    mHoldJumpStrength = strength;
    mHoldJumpFrames = duration;
}
void BasePlayer2::Jump(float extraStrength) {
    if(mGrounded && !mJumped && (!mSpecialAnim || (mSpecialAnim && mCancelableSpecial))) {
        mSlopeForce = 0.0f;
        ChangeAnimID(ANIM_JUMP, 0.0f);
        mSpecialAnim = false;
        mLastPos.y = GetPosition().y - (mJumpStrength + extraStrength);
        mAppliedForce.y = 0.0;
        mHoldJumpTimer = mHoldJumpFrames;
        mJumped = true;
        mGrounded = false;
        mActionKeys[PKEY_JUMP].keyState = KEY_DOWNCHECKED;
    }
    else if(!mJumped && mUnderwater && (mVelocity.y < 1 && mVelocity.y > -1)) {
        mSlopeForce = 0.0f;
        ChangeAnimID(ANIM_JUMP, 0.0f);
        mSpecialAnim = false;
        mAppliedForce.y = 0.0;
        mLastPos.y = GetPosition().y - (mJumpStrength * mWaterJumpStrength);
        mHoldJumpTimer = mHoldJumpFrames;
        mJumped = true;
        mActionKeys[PKEY_JUMP].keyState = KEY_DOWNCHECKED;
    }
    else if(!mJumped){
        if(mHoldJumpTimer > 0 && mHoldJump)  {
            AddVelocityY(mHoldJumpStrength);
        }
    }
}
void BasePlayer2::Float() {
    PhysicsBox2f::Float();
}
void BasePlayer2::UpdateKeys(SDL_Event& event) {
    if(!mIgnoreInput) {
        for(size_t i = 0; i < mActionKeys.size(); i++) {
            mActionKeys[i].Update(event);
        }
    }
}
void BasePlayer2::UpdateKeys(vector<SDL_Event>& events) {
    for(size_t i = 0; i < events.size(); i++) {
        if(!mIgnoreInput) {
            for(size_t j = 0; j < mActionKeys.size(); j++) {
                mActionKeys[j].Update(events[i]);
            }
        }
    }
}
void BasePlayer2::UpdateKeys(SDL_Event& event, SDL_GameController* controller) {
    if(!mIgnoreInput) {
        for(size_t i = 0; i < mActionKeys.size(); i++) {
            mActionKeys[i].Update(event, controller);
        }
    }
}
void BasePlayer2::CheckKeyDown(SDL_Keycode code) {
    if(!mIgnoreInput) {
        for(size_t j = 0; j < mActionKeys.size(); j++) {
            mActionKeys[j].CheckKeyDown(code);
        }
    }
}
void BasePlayer2::CheckKeyUp(SDL_Keycode code) {
    if(!mIgnoreInput) {
        for(size_t j = 0; j < mActionKeys.size(); j++) {
            mActionKeys[j].CheckKeyUp(code);
        }
    }
}
void BasePlayer2::CheckInput() {
    mLeftHeld = GetKeyDown(PKEY_LEFT);
    mRightHeld = GetKeyDown(PKEY_RIGHT);
    mUpHeld = GetKeyDown(PKEY_UP);
    mDownHeld = GetKeyDown(PKEY_DOWN);
    if(mRightHeld) {
        mRightHeldFrames+= mStep;
    }
    else {
        mRightHeldFrames = 0;
    }
    if(mLeftHeld) {
        mLeftHeldFrames+= mStep;
    }
    else {
        mLeftHeldFrames = 0;
    }
    if(!mIgnoreInput) {
        Vector2f velocity = GetPosition() - mLastPos;
        if(!mLeftHeld && velocity.x < 0) {
            if(mVelocity.x - mWalkAccel < 0) {
                mVelocity.x = 0.0f;
            }
            if(mWalkAccel > -1.0f) {
                ApplyForceX(mWalkAccel);
            }
            else {
                mLastPos.x = GetPosition().x;
            }
        }
        else if(!mRightHeld && velocity.x > 0) {
            if(mVelocity.x + mWalkAccel > 0) {
                mVelocity.x = 0.0f;
            }
            if(mWalkAccel > -1.0f) {
                ApplyForceX(-mWalkAccel);
            }
            else {
                mLastPos.x = GetPosition().x;
            }
        }
        if(GetKeyDown(PKEY_JUMP)) {
            if(mDownHeld && mOnPlatform) {
                ApplyForceY(-2);
                ChangeAnimID(ANIM_FALL);
                mGrounded = false;
            }
            else if((!mUnderwater || (mUnderwater && mUpHeld)) && !(mDownHeld && mBoundCollider)) {
                Jump();
            }
        }
    }
}
void BasePlayer2::Update(std::vector<SDL_Event>& events, float tstep) {
    mStep = tstep;
    UpdateKeys(events);
    mOnPlatform = false;
    for(auto& tile : mCollidersBG) {
        if(tile->GetType() == TILE_PLATFORM) {
            mOnPlatform = true;
        }
    }
    CheckInput();
    CheckAnimations();
    if(mGrounded || mSubmergeDepth >= mHitBox.GetHeight() / 2) {
        mJumped = false;
    }
    if(mHoldJumpTimer > 0) {
        mHoldJumpTimer--;
    }
    Walk();
    Fall();
    Float();
    Decelerate();
    Settle();
    VerletNoVelocity(mStep);
    UpdateAnimation();
    ResetFlags();
}
void BasePlayer2::UpdateAnimation() {
    if(mVelocity.x > 0 || (mRightHeld && !mLeftHeld)) {
        mFlipped = false;
    }
    if(mVelocity.x < 0 || (!mRightHeld && mLeftHeld)) {
        mFlipped = true;
    }
    mCurrentAnimation.SetHoriFlip(mFlipped);
    mCurrentAnimation.SetPosition(mHitBox.GetOrigin());
    mCurrentAnimation.Update();
}
void BasePlayer2::CheckPlatform() {
    mOnPlatform = false;
    for(auto& tile : mCollidersBG) {
        if(tile->GetType() == TILE_PLATFORM) {
            mOnPlatform = true;
        }
    }
}
void BasePlayer2::Walk() {
    Vector2f velocity = GetPosition() - mLastPos;
    if(mLeftHeld) {
        if(mWalkAccel > -1.0f) {
            if((mGrounded || mAirAccels) && velocity.x > -mWalkSpeed) {
                ApplyForceX(-mWalkAccel);
            }
        }
        else {
            mLastPos.x = GetPosition().x + mWalkSpeed;
        }
        if(velocity.x < -mWalkSpeed) {
            mLastPos.x = GetPosition().x + mWalkSpeed;
        }
    }
    if(mRightHeld) {
        if(mWalkAccel > -1.0f) {
            if((mGrounded || mAirAccels)  && velocity.x < mWalkSpeed) {
                ApplyForceX(mWalkAccel);
            }
        }
        else {
            mLastPos.x = GetPosition().x - mWalkSpeed;
        }
        if(velocity.x > mWalkSpeed) {
            mLastPos.x = GetPosition().x - mWalkSpeed;
        }
    }
    if(mLeftHeld && mRightHeld) {
        if(velocity.x > 0) {
            ApplyForceX(-mWalkAccel);
        }
        else if(velocity.x < 0) {
            ApplyForceX(mWalkAccel);
        }
    }
}
void BasePlayer2::CheckAnimations() {
    if(!mUsesModel) {
        CheckAnimationsSprite();
    }
    else {
        CheckAnimationsModel();
    }
}
void BasePlayer2::CheckAnimationsSprite() {
    if(mGrounded && mVelocity.x != 0) {
        if((mRightHeldFrames > 4 && mVelocity.x > 0) || (mLeftHeldFrames > 4 && mVelocity.x < 0)) {
            ChangeAnimation("walking");
        }
        if(mVelocity.x >= mWalkSpeed || mVelocity.x <= -mWalkSpeed) {
            ChangeAnimation("walking");
        }
        else if(mCurrentAnimation == "idle") {
            ChangeAnimation("nudge");
        }
    }
    if(!mGrounded) {
        if(mVelocity.y < -1) {
            bool forceFrame = false;
            if(mCurrentAnimation != "jumping") {
                forceFrame = true;
            }
            if(mCurrentAnimation != "falling") {
                ChangeAnimation("falling");
                if(forceFrame) {
                    mCurrentAnimation.ForceFrame(2);
                }
            }
        }
    }
    if(mGrounded && (!mLeftHeld && !mRightHeld)) {
        ChangeAnimation("idle");
    }
}
void BasePlayer2::CheckAnimationsModel() {
    if(!mSpecialAnim) {
        if(mGrounded && mVelocity.x != 0) {
            if((mRightHeldFrames > 4 && mVelocity.x > 0) || (mLeftHeldFrames > 4 && mVelocity.x < 0)) {
                ChangeAnimID(ANIM_WALK);
            }
            if(mVelocity.x >= mWalkSpeed || mVelocity.x <= -mWalkSpeed) {
                ChangeAnimID(ANIM_WALK);
            }
        }
        if(!mGrounded) {
            if(mVelocity.y < -1) {
                if(*mModel.GetCurrentAnim() != ANIM_FALL) {
                    if(GetCurrentAnimID() == ANIM_JUMP) {
                        ChangeAnimID(ANIM_FALL);
                    }
                    else {
                        TransitionToAnimation(ANIM_FALL, 0.1, GetCurrentFrameNum(), 9);
                    }
                }
            }
        }
        if(mGrounded && (!mLeftHeld && !mRightHeld)) {
            ChangeAnimID(ANIM_IDLE);
        }
    }
    else if(mModel.GetCurrentAnim()->Finished()) {
        mSpecialAnim = false;
    }
}
ActionKey& BasePlayer2::GetActionKey(PlayerKey key) {
    for(int i = 0; i < mActionKeys.size(); i++) {
        if(mActionKeys[i].playerKey == key) {
            return mActionKeys[i];
        }
    }
    return mActionKeys[0];
}
void BasePlayer2::ForceAnimID(int i, int frame) {
    if(i == mModel.GetCurrentAnimID()) {
        return;
    }
    if(i < mAnimationCache.size()) {
        ChangeAnimation(mAnimationCache[i], 0.0f);
            mModel.GetCurrentAnim()->ForceFrame(frame);
    }
}
void BasePlayer2::ChangeAnimID(int i, float transitionDur) {
    if(i == mModel.GetCurrentAnimID()) {
        return;
    }
    if(i < mAnimationCache.size()) {
        int k = mModel.GetCurrentAnim()->GetID();
        ChangeAnimation(mAnimationCache[i], transitionDur);
    }
}
void BasePlayer2::TransitionToAnimation(int i, float transitionSpeed, int srcFrame, int destFrame) {
    if(i == mModel.GetCurrentAnimID()) {
        return;
    }
    mModel.TransitionToAnimation(mAnimationCache[i], transitionSpeed, srcFrame, destFrame);
    //printf("%s (%f)\n", mModel.GetCurrentAnim()->GetName().c_str(), transitionSpeed);
}
unsigned int BasePlayer2::GetCurrentFrameNum() {
    if(mUsesModel) {
        return mModel.GetCurrentAnim()->GetCurFrameNum();
    }
    else {
        return mCurrentAnimation.GetCurFrameNum();
    }
}
int BasePlayer2::GetCurrentAnimID() {
    if(mUsesModel) {
        return mModel.GetCurrentAnim()->GetID();
    }
    else {
        return mCurrentAnimation.GetID();
    }
}
bool BasePlayer2::GetCurrentAnimFinished() {
    if(mUsesModel) {
        return mModel.GetCurrentAnim()->Finished();
    }
    else {
        return mCurrentAnimation.Finished();
    }
}
void BasePlayer2::ChangeAnimation(int i, float transitionDur) {
    if(i < 0) {
        return;
    }
    if(mUsesModel) {
        mModel.ChangeAnimation(i, transitionDur);
    }
    else {
        if(i < mAnimations.size()) {
            mCurrentAnimation = mAnimations[i];
            mCurrentAnimation.Start();
            mCurrentAnimation.SetHoriFlip(mFlipped);
        }
    }
}
void BasePlayer2::ChangeAnimation(string anim) {
    for(int i = 0; i < mAnimations.size(); i++) {
        if(mAnimations[i].GetRef() == anim && mCurrentAnimation.GetRef() != anim) {
            mCurrentAnimation = mAnimations[i];
            mCurrentAnimation.Start();
            mCurrentAnimation.SetHoriFlip(mFlipped);
            return;
        }
    }
}
void BasePlayer2::AdjustVelocity() {
    mVelocity = GetPosition() - GetLastPosition();
    if(mRightHeld && mVelocity.x > mWalkSpeed) {
        mVelocity.x = mWalkSpeed;
    }
    if(mLeftHeld && mVelocity.x < -mWalkSpeed) {
        mVelocity.x = -mWalkSpeed;
    }
    if(fabs(mVelocity.x) < mWalkSpeed || fabs(mVelocity.x) > mMaxVelocity.x) {
        mLastPos.x = GetPosition().x;
    }
    if(fabs(mVelocity.y) > mMaxVelocity.y || fabs(mVelocity.y) < 0.5f) {
        mLastPos.y = GetPosition().y;
    }
}
void BasePlayer2::AdjustVelocity(Vector2f& vel) {
    if(vel.x && Trunc(fabs(vel.x), 5) < Trunc(mWalkAccel * mStep * mStep, 5)) {
        vel.x = 0;
    }
    if(vel.y && fabs(vel.y) < 0.5f && mGrounded) {
        vel.y = 0.0f;
    }
    if(vel.x > mMaxVelocity.x) {
        vel.x = mMaxVelocity.x;
    }
    else if(vel.x < -mMaxVelocity.x) {
        vel.x = -mMaxVelocity.x;
    }
    if(vel.y > mMaxVelocity.y) {
        vel.y = mMaxVelocity.y;
    }
    else if(vel.y < -mMaxVelocity.y) {
        vel.y = -mMaxVelocity.y;
    }
}
bool BasePlayer2::GetKeyDown(PlayerKey key) {
    return mActionKeys[key].keyState == KEY_DOWN;
    for(auto& akey : mActionKeys) {
        if(akey.playerKey == key) {
            return akey.keyState == KEY_DOWN;
        }
    }
    return false;
}
bool BasePlayer2::GetKeyDown(string key) {
    for(auto& akey : mActionKeys) {
        if(akey.name == key) {
            return akey.keyState == KEY_DOWN;
        }
    }
    return false;
}
bool BasePlayer2::NoDirections() {
    return(!mUpHeld && !mRightHeld && !mLeftHeld && !mDownHeld);
}
KeyState BasePlayer2::GetKeyState(PlayerKey key) {
    for(auto& akey : mActionKeys) {
        if(akey.playerKey == key) {
            return akey.keyState;
        }
    }
    return KEY_NONE;
}
void BasePlayer2::Respawn() {
    mHitBox.SetPosition(mSpawnPosition);
    mLastPos = mSpawnPosition;
    mGrounded = false;
    Stop();
}
void BasePlayer2::CheckAndResolveCollision(BackgroundTile2& ground) {
    Collision c = solstice::CheckCollision(&GetHitBox(), &ground.GetHitBox());
    if(!c) {
        return;
    }
    if(c.hasCollision && GetInvMass()) {
        if(ground.GetType() == TILE_GROUND) {
            if(c.mtv.Length2() <= 2.0f & fabs(c.normal.x) >= 0.5f) {
                if(c.normal.y > 0.0f) {
                    mGrounded = true;
                }
                return;
            }
            Vector2f velocity = GetPosition() - GetLastPosition();
            Vector2f normalVel = c.normal.Dot(velocity) * c.normal;
            Vector2f tangentVel = velocity - normalVel;
            float groundFriction = ground.GetFriction() * fabs(c.normal.y);
            float friction = std::max(mFriction, groundFriction);
            float elasticity = std::max(mElasticity, ground.GetElasticity());
            Vector2f vel = tangentVel * -friction + normalVel * -(1 + elasticity);
            if(c.normal.y > 0.25f) {
                vel.x = 0.0f;
            }
            ResolveCollision(c, 1.0f, vel);
        }
        else if(ground.GetType() == TILE_PLATFORM) {
            PhysicsBox2f::CheckPlatform(c, ground);
        }
        else if(ground.GetType() == TILE_WATER) {
            SetUnderwater(true);
        }
    }
}
void BasePlayer2::ResolveCollision(Collision& c, float mass, Vector2f vel) {
    if(!mHitBox.GetAngle()) {
        if(c.normal.y > 0.2 && c.normal.x) {
            float yMove = (c.mtv.x * c.mtv.x) / c.mtv.y + c.mtv.y;
            AddY(yMove * mass);
        }
        else {
            AddXY(c.mtv * mass);
        }
        Vector2f oldVel = GetPosition() - mLastPos;
        mLastPos-= vel * mass;
        if(oldVel.y < 0.0f && fabs(vel.y) < 0.01f) {
            ChangeAnimID(ANIM_LAND);
            mSpecialAnim = true;
        }
        if(c.normal.y > 0.0f && mVelocity.y <= 2.0f) {
            SetGrounded(true);
            if(c.normal.y != 1.0f) {
                mSlopeForce = 10.0f;
            }
        }
    }
    else {
        RigidBody::ResolveCollision(c, mass, vel);
    }
}
void BasePlayer2::ResolveCollision(RigidBody* b, Collision& c, Vector2f vel) {
    float massA = GetInvMass() / (GetInvMass() + b->GetInvMass());
    float massB = b->GetInvMass() / (GetInvMass() + b->GetInvMass());
    float massAy = massA, massBy = massB;
    Vector2f b1Vel = GetPosition() - GetLastPosition();
    Vector2f b2Vel = b->GetPosition() - b->GetLastPosition();

    if(SameSign(mVelocity.x, c.mtv.x)) {
        vel.x = 0.0f;
    }
    if(fabs(c.normal.y) > 0.1f && fabs(c.mtv.y) > 1.0f && c.mtv.x) {
        if(mGrounded && c.mtv.y < 0.0) {
            if(fabs(c.mtv.x) > 0.0) {
                float xMove = (c.mtv.y * c.mtv.y) / c.mtv.x + c.mtv.x;
                b->AddX(-xMove * massB);
            }
            else {
                b->AddXY(-c.mtv);
            }
        }
        else {
            float yMove = (c.mtv.x * c.mtv.x) / c.mtv.y + c.mtv.y;
            AddY(yMove * massAy);
            b->AddY(-yMove * massBy);
        }
    }
    else {
        AddXY(Vector2f(c.mtv.x * 0.5f, c.mtv.y * massAy));
        b->AddXY(Vector2f(c.mtv.x * -0.5f, c.mtv.x * massBy));
    }
    if(c.normal.y >= 0.25f) {
        mLastPos.y = GetPosition().y;
        b->GetLastPosition().y+= vel.y * massBy;
        mGrounded = true;
    }
    else {
        mLastPos.y-= vel.y * massAy;
        b->GetLastPosition().y+= vel.y * massBy;
    }
    if(fabs(c.normal.x) > 0.5f) {
        mLastPos.x-= vel.x * massA;
        b->GetLastPosition().x+= vel.x * massB;
    }
    if(c.normal.y > 0.0f && b1Vel.y < 0.0f) {
        SetGrounded(true);
    }
    if(c.normal.y < 0.0f && b1Vel.y < 0.0f) {
        b->SetGrounded(true);
    }
}
void BasePlayer2::GenerateAnimCache() {
}
void BasePlayer2::UpdateModel(float t, Vector2f scale) {
    mModel.Update(t);
    glm::vec3 modelScale = mModel.GetTransform().scale;
    if(mFlipped) {
        modelScale.z = -fabs(modelScale.z);
    }
    else {
        modelScale.z = fabs(modelScale.z);
    }
    glm::vec3 pos;
    pos.x = (GetPosition().x + mModelOffset.x) * scale.x;
    pos.y = (GetPosition().y + mModelOffset.y) * scale.y;
    pos.z = mModel.GetTransform().translation.z;
    mModel.SetScale(modelScale);
    mModel.SetTranslation(pos);
}
void BasePlayer2::SetupModel() {
    mModel.SetupModel();
    mUsesModel = true;
    GenerateAnimCache();
}
void DefaultAnimSort(std::vector<Animation>& anims) {
    GetAnimation(anims, "idle").SetID(ANIM_IDLE);
    GetAnimation(anims, "jumping").SetID(ANIM_JUMP);
    GetAnimation(anims, "falling").SetID(ANIM_FALL);
    GetAnimation(anims, "walking").SetID(ANIM_WALK);
}
bool BasePlayer2::AnyButtonPressed() {
    for(size_t i = 0; i < mActionKeys.size(); i++) {
        if(mActionKeys[i].keyState == KEY_DOWN) {
            return true;
        }
    }
    return false;
}
}
