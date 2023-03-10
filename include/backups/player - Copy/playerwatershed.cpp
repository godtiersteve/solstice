#include "playerWatershed.h"

namespace solstice {
    WatershedPlayer::WatershedPlayer() {
        mSwimMaxVel = 9.0f;
        mSwimAccel = 0.75f;
        mSwimDecel = 0.5f;
        mSwimAngle = 0;
        mFocusMode = false;
        mFlipTimer = 0;
        mUnderwater = false;
        mSwimming = false;
        mDiving = false;
        mFocusModifier = 0.5f;
        mActionKeys.push_back(ActionKey(SDLK_z, PKEY_BUTTON1, SDL_CONTROLLER_BUTTON_L2));
        mActionKeys.back().SetAnalog(SDL_CONTROLLER_BUTTON_L2, 1000);
        mHeldObject = NULL;
        mFins = false;
        mSkidding = false;
        mDashing = false;
        mDashVel = 15.0f;
        mSpecialAnim = false;
        mSurfacing = false;
        mHoldLength = 0.0f;
        mTurning = false;
        mUpHeld = false;
        mDownHeld = false;
        mLeftHeld = false;
        mRightHeld = false;
        mFacingLeft = false;
        mThrowStrength = Vector2f(20, 10);
        mFocusTimer = 0.0f;
        mActiveBulletType = -1;
        mSurfaced = false;
        mSurfaceVel = 3500;
        mDiveVel = 10000;
        mSkidStop = false;
        mObjectPulled = false;
        mCancelableSpecial = false;
        mRodVisible = false;
        mFocusAccels = false;
        mGroundFocusModifier = 1.0f;
        mGrabRange = 250;
        mTouchingWall = false;
        mAnchorZ = 0.0f;
        mRecoiling = false;
        mReleasedObjDelay = 0;
        mReleasedObjFrames = 0;
        mPrevHeldObject = NULL;
        mGrabObjDelay = 0;
        mShootResetTimer = 0;
        mBullets.resize(30);
        mLastInactiveBullet = 0;
        mHeadAnchorID = -1;
        mHandAnchorID = -1;
        mShellFlipped = false;
        mHurlAngle = 0.0f;
    }
    void WatershedPlayer::InitSwimMovement(float maxSpeed, float accel, float decel, float focusModifier) {
        mSwimMaxVel = maxSpeed;
        mSwimAccel = accel;
        mSwimDecel = decel;
        mFocusModifier = focusModifier;
    }
    void WatershedPlayer::InitMovement(float maxSpeed, float accel, float decel) {
        mSwimMaxVel = maxSpeed;
        mSwimAccel = accel;
        mSwimDecel = decel;
    }
    void WatershedPlayer::Respawn() {
        BasePlayer2::Respawn();
        mBullets.clear();
        mHeldObject = NULL;
        mSpecialAnim = false;
        mCancelableSpecial = true;
        mObjectPulled = false;
        mHurling = false;
        for(auto& bullet : mBulletTypes) {
            bullet.Reset();
        }
    }
    void WatershedPlayer::Walk() {
        if(mSwimming || mHurling) {
            return;
        }
        float accel = mWalkAccel;
        if(mFocusMode && !mFocusAccels) {
            accel = -1.0f;
        }
        Vector2f velocity = GetPosition() - mLastPos;
        float walkSpeed = mWalkSpeed;
        if(mFocusMode) {
            walkSpeed*= mGroundFocusModifier;
        }
        if(mLeftHeld) {
            if(accel > -1.0f) {
                if((mGrounded || mAirAccels) && velocity.x > -walkSpeed) {
                    if(mSkidStop) {
                        if(GetCurrentAnimID() != IONA_RUN_BRAKE || (GetCurrentAnimID() == IONA_RUN_BRAKE && mVelocity.x > 0.0)) {
                            ApplyForceX(-accel);
                        }
                    }
                    else {
                        ApplyForceX(-accel);
                    }
                }
            }
            else {
                mLastPos.x = GetPosition().x + walkSpeed;
            }
            if(velocity.x < -walkSpeed) {
                mLastPos.x = GetPosition().x + walkSpeed;
            }
            if(!mFocusMode && mGrounded && velocity.x >= walkSpeed * 0.90) {
                if(GetCurrentAnimID() == IONA_RUNSHOOT_FORWARD || mShootResetTimer) {
                    ChangeAnimIDSpecial(IONA_SHOOT_BRAKE, true, 0.0f);
                }
                else {
                    ChangeAnimIDSpecial(IONA_RUN_BRAKE, true, 0.0f);
                }
                mFacingLeft = true;
                mLastPos.x = GetPosition().x - walkSpeed * 0.90f;
            }
        }
        else if(mRightHeld) {
            if(accel > -1.0f) {
                if((mGrounded || mAirAccels)  && velocity.x < walkSpeed) {
                    if(mSkidStop) {
                        if(GetCurrentAnimID() != IONA_RUN_BRAKE || (GetCurrentAnimID() == IONA_RUN_BRAKE && mVelocity.x < 0.0)) {
                            ApplyForceX(accel);
                        }
                    }
                    else {
                        ApplyForceX(accel);
                    }
                }
            }
            else {
                mLastPos.x = GetPosition().x - walkSpeed;
            }
            if(velocity.x > walkSpeed) {
                mLastPos.x = GetPosition().x - walkSpeed;
            }
            if(!mFocusMode && mGrounded && velocity.x <= -walkSpeed * 0.90) {
                if(GetCurrentAnimID() == IONA_RUNSHOOT_FORWARD) {
                    ChangeAnimIDSpecial(IONA_SHOOT_BRAKE, true, 0.0f);
                }
                else {
                    ChangeAnimIDSpecial(IONA_RUN_BRAKE, true, 0.0f);
                }
                mFacingLeft = false;
                mLastPos.x = GetPosition().x + walkSpeed * 0.90f;
            }
        }
        if((mLeftHeld && mRightHeld) || ((!mLeftHeld && !mRightHeld) && (GetCurrentAnimID() != IONA_HOOK || (GetCurrentAnimID() == IONA_HOOK && mGrounded)))) {
            if(accel > 0.0) {
                if(velocity.x > 0) {
                    ApplyForceX(-accel);
                }
                else if(velocity.x < 0) {
                    ApplyForceX(accel);
                }
            }
            else {
                mLastPos.x = GetPosition().x;
            }
        }
    }
    void WatershedPlayer::FocusAccelerate() {
        if(mUpHeld) {
            ApplyForceY(mSwimAccel);
        }
        if(mDownHeld) {
            ApplyForceY(-mSwimAccel);
        }
        if(mRightHeld) {
            ApplyForceX(mSwimAccel);
        }
        if(mLeftHeld) {
            ApplyForceX(-mSwimAccel);
        }
        if(!mRightHeld && mSwimVelocity.x > 0) {
            ApplyForceX(-mSwimAccel);
        }
        if(!mLeftHeld && mSwimVelocity.x < 0) {
            ApplyForceX(mSwimAccel);
        }
        if(!mUpHeld && mSwimVelocity.y > 0) {
            ApplyForceY(-mSwimAccel);
        }
        if(!mDownHeld && mSwimVelocity.y < 0) {
            ApplyForceY(mSwimAccel);
        }
    }
    void WatershedPlayer::Accelerate() {
        if(mHurling) {
            return;
        }
        if(mSwimming && !mDiving) {
            mSwimDirection = 0;
            if(!mSpecialAnim || (mSpecialAnim && mCancelableSpecial)) {
                if(mUpHeld) {
                    mSwimDirection.y = 1.0f;
                }
                if(mDownHeld) {
                    mSwimDirection.y = -1.0f;
                }
                if(mRightHeld) {
                    mSwimDirection.x = 1.0f;
                }
                if(mLeftHeld) {
                    mSwimDirection.x = -1.0f;
                }
            }

            if(!mSkidding) {
                mSwimDirection = mSwimDirection.GetUnit();
                if(mFocusMode) {
                    mSwimVelocity = mSwimDirection * mFocusModifier * mSwimMaxVel;
                }
                else {
                    Vector2f deceleration(0);
                    Vector2f acceleration = mSwimDirection * mSwimAccel;
                    if(!mSwimDirection.y && mSwimVelocity.y > 0) {
                        deceleration.y-= mSwimDecel;
                        if(!mSwimDirection.x) {
                            if(mSwimVelocity.x > 0) {
                                acceleration.x-= mSwimDecel;
                            }
                            else if(mSwimVelocity.x < 0) {
                                acceleration.x+= mSwimDecel;
                            }
                        }
                    }
                    if(!mSwimDirection.y && mSwimVelocity.y < 0) {
                        deceleration.y+= mSwimDecel;
                        if(!mSwimDirection.x) {
                            if(mSwimVelocity.x > 0) {
                                acceleration.x-= mSwimDecel;
                            }
                            else if(mSwimVelocity.x < 0) {
                                acceleration.x+= mSwimDecel;
                            }
                        }
                    }
                    if(!mSwimDirection.x && mSwimVelocity.x > 0) {
                        deceleration.x-= mSwimDecel;
                        if(!mSwimDirection.y) {
                            if(mSwimVelocity.y > 0) {
                                acceleration.y-= mSwimDecel;
                            }
                            else if(mSwimVelocity.y < 0) {
                                acceleration.y+= mSwimDecel;
                            }
                        }
                    }
                    if(!mSwimDirection.x && mSwimVelocity.x < 0) {
                        deceleration.x+= mSwimDecel;
                        if(!mSwimDirection.y) {
                            if(mSwimVelocity.y > 0) {
                                acceleration.y-= mSwimDecel;
                            }
                            else if(mSwimVelocity.y < 0) {
                                acceleration.y+= mSwimDecel;
                            }
                        }
                    }

                    mSwimVelocity+= acceleration + deceleration;
                    if(fabs(mSwimVelocity.x) < fabs(deceleration.x)) {
                        mSwimVelocity.x = 0.0f;
                    }
                    if(fabs(mSwimVelocity.y) < fabs(deceleration.y)) {
                        mSwimVelocity.y = 0.0f;
                    }
                    if(mSwimVelocity.x > mSwimMaxVel) {
                        mSwimVelocity.x = mSwimMaxVel;
                    }
                    if(mSwimVelocity.x < -mSwimMaxVel) {
                        mSwimVelocity.x = -mSwimMaxVel;
                    }
                    if(mSwimVelocity.y > mSwimMaxVel) {
                        mSwimVelocity.y = mSwimMaxVel;
                    }
                    if(mSwimVelocity.y < -mSwimMaxVel) {
                        mSwimVelocity.y = -mSwimMaxVel;
                    }
                    float swimVelSqr = mSwimMaxVel * mSwimMaxVel;
                    Vector2f swimVelUnit = mSwimVelocity.GetUnit();
                    if(mSwimVelocity.Magnitude2() > swimVelSqr) {
                        mSwimVelocity = swimVelUnit * mSwimMaxVel ;
                    }
                    float diff = mSwimDirection.Dot(swimVelUnit);
                    if(!mSkidding && (diff < -0.9 && mSwimVelocity.Magnitude2() >= swimVelSqr * 0.7f)) {
                        mSkidding = true;
                        mSkidDirection = -mSwimDirection;
                    }
                }
            }
            else {
                Skid();
            }
            if(mSwimVelocity.Magnitude2() > mSwimMaxVel * mSwimMaxVel) {
                mSwimVelocity = mSwimVelocity.GetUnit() * mSwimMaxVel;
            }
            mVelocity = mSwimVelocity;
            mSwimColImpulse = 0;
        }
    }
    void WatershedPlayer::Skid() {
        if(!mSkidding) {
            return;
        }
        mFlipTimer = 0;
        if(!UsesFocusAnims()) {
            Vector2f decel = mSwimDecel * mSkidDirection;
            if(mSwimVelocity.x < 0) {
                if(mSwimDirection.x < 0 && !GetKeyDown(PKEY_BUTTON1)) {
                    mSkidding = false;
                    mSpecialAnim = false;
                    return;
                }
                mSwimVelocity.x = mSwimVelocity.x + mSwimDecel;
                if(mSwimVelocity.x > 0.0f) {
                    mSkidding = false;
                    mSpecialAnim = false;
                    mSkidDirection = 0.0;
                }
            }
            if(mSwimVelocity.x > 0) {
                if(mSwimDirection.x > 0 && !GetKeyDown(PKEY_BUTTON1)) {
                    mSkidding = false;
                    mSpecialAnim = false;
                    return;
                }
                mSwimVelocity.x = mSwimVelocity.x - mSwimDecel;
                if(mSwimVelocity.x < 0.0f) {
                    mSkidding = false;
                    mSpecialAnim = false;
                    mSkidDirection = 0.0;
                }
            }
            if(mSwimVelocity.y < 0) {
                if(mSwimDirection.y < 0 && !GetKeyDown(PKEY_BUTTON1)) {
                    mSkidding = false;
                    mSpecialAnim = false;
                    return;
                }
                mSwimVelocity.y = mSwimVelocity.y + mSwimDecel;
                if(mSwimVelocity.y > 0.0f) {
                    mSkidding = false;
                    mSpecialAnim = false;
                    mSkidDirection = 0.0;
                }
            }
            if(mSwimVelocity.y > 0) {
                if(mSwimDirection.y > 0 && !GetKeyDown(PKEY_BUTTON1)) {
                    mSkidding = false;
                    mSpecialAnim = false;
                    return;
                }
                mSwimVelocity.y = mSwimVelocity.y - mSwimDecel;
                if(mSwimVelocity.y < 0.0f) {
                    mSkidding = false;
                    mSpecialAnim = false;
                    mSkidDirection = 0.0;
                }
            }
            if(mSwimDirection.y < 0.0 && mSkidDirection.y <= 0.0) {
                mSwimVelocity.y-= mSwimAccel;
            }
            if(mSwimDirection.y > 0.0 && mSkidDirection.y >= 0.0) {
                mSwimVelocity.y+= mSwimAccel;
            }
            if(mSwimDirection.x < 0.0 && mSkidDirection.x <= 0.0) {
                mSwimVelocity.x-= mSwimAccel;
            }
            if(mSwimDirection.x > 0.0 && mSkidDirection.x >= 0.0) {
                mSwimVelocity.x+= mSwimAccel;
            }
            if(mSwimVelocity.Magnitude2() == 0.0) {
                mSkidding = false;
                mSpecialAnim = false;
                mSkidDirection = 0.0;
            }
        }
    }
    void WatershedPlayer::Dive() {
        float depth = (mSubmergeDepth / mHitBox.GetHeight());
        if(depth < 0.5f) {
            mSwimming = false;
            mDiving = false;
            mNoFloat = false;
            mSkidding = false;
            mTurning = false;
            mNoGravity = false;
            mSurfacing = false;
            if(GetCurrentAnimID() == IONA_SWIM && mVelocity.y >= 5.0f) {
                mLastPos.y = GetPosition().y;
            }
        }
        else if(!mSwimming) {
            if(mUnderwater && !mSwimming) {
                if(mDownHeld && depth < 4.0f) {
                    mDiving = true;
                    mNoFloat = true;
                    mNoGravity = true;
                }
                else if(!mDownHeld) {
                    mDiving = false;
                    if(GetCurrentAnimID() == IONA_MERMAID_DIVE) {
                        mSpecialAnim = false;
                        StopY();
                    }
                }
            }
            if(mFocusMode && mDownHeld) {
                mSwimVelocity.y = -mSwimMaxVel * mFocusModifier;
                mLastPos.y = GetPosition().y + mSwimMaxVel * mFocusModifier;
                mDiving = false;
                mNoFloat = true;
            }
        }
        if(depth >= 1.5f) {
            mSwimming = true;
            mNoFloat = true;
            mNoGravity = true;
            mSurfacing = false;
            mSurfaced = false;
            mDiving = false;
        }
        else if(!mFocusMode) {
            if(GetCurrentAnimID() == IONA_MERMAID_DIVE) {
                if(mLastPos.y > GetPosition().y) {
                    mLastPos.y = GetPosition().y;
                }
                if(GetCurrentFrameNum() >= 8) {
                    ApplyForceY(-mDiveVel);
                }
            }
        }
    }
    void WatershedPlayer::CheckJump() {
        if(mHurling || (mSpecialAnim && !mCancelableSpecial)) {
            return;
        }
        if(GetKeyDown(PKEY_JUMP) && mDownHeld && !mUnderwater) {
            mPassThroughPlatforms = true;
            ChangeAnimID(ANIM_FALL, 0.05f);
        }
        else {
            mPassThroughPlatforms = false;
        }
        UpdatePlatform();
        if(mGrounded || mSubmergeDepth >= (mHitBox.GetHeight() / 2)) {
            mJumped = false;
        }
        if(mHoldJumpTimer > 0) {
            mHoldJumpTimer--;
        }
    }
    void WatershedPlayer::Surface() {
        float depth = mHitBox.GetHeight() * 1.5;
        if(mSwimming) {
            if(mSubmergeDepth <= depth && mVelocity.Magnitude2() == 0) {
                mSurfacing = true;
            }
            if(mSubmergeDepth > depth) {
                mDiving = false;
            }
        }
        if(mSubmergeDepth > depth * 2.0f) {
            mSwimming = true;
            mSurfacing = false;
            mDiving = false;
        }
        if(mSurfacing) {
            ApplyForceY(mSurfaceVel);
        }
    }
    void WatershedPlayer::CheckFlip() {
        if(mShootResetTimer) {
            if(mLeftHeld || (mFacingLeft && !mRightHeld)) {
                if(!mFlipped) {
                    //ChangeAnimIDSpecial(IONA_RUN_BRAKE, true);
                }
                mFlipped = true;
            }
            else if(mRightHeld) {
                if(!mFlipped) {
                    //ChangeAnimIDSpecial(IONA_RUN_BRAKE, true);
                }
                mFlipped = false;
            }
        }
        else {
            if(!mSwimming) {
                if(GetCurrentAnimID() != IONA_TURN && GetCurrentAnimID() != IONA_RUN_BRAKE && !mFocusMode) {
                    if(!mSpecialAnim && mGrounded && fabs(mVelocity.x) < 2.0) {
                        if(mFlipped && mRightHeld) {
                            ChangeAnimIDSpecial(IONA_TURN, true, 0.0f);
                        }
                        else if(!mFlipped && mLeftHeld) {
                            ChangeAnimIDSpecial(IONA_TURN, true, 0.0f);
                        }
                    }
                    if(!mSpecialAnim) {
                        if(mVelocity.x >= 0.0f && !mFacingLeft) {
                            mFlipped = false;
                        }
                        else if(mVelocity.x <= 0.0f && mFacingLeft) {
                            mFlipped = true;
                        }
                    }
                }
                mFlipTimer = 0;
            }
            else if(!mFocusMode) {
                if(mFlipTimer >= 30 && !mFocusMode) {
                    if(mSwimAngle < DegreesToRadians(-90) || mSwimAngle > DegreesToRadians(90)) {
                        if(!mFlipped) {
                            ChangeAnimIDSpecial(IONA_SWIM_TWIRL, true, 0.0f);
                            mFlipTimer = 0;
                        }
                    }
                    else if(mSwimAngle > DegreesToRadians(-90) && mSwimAngle < DegreesToRadians(90)) {
                        if(mFlipped) {
                            ChangeAnimIDSpecial(IONA_SWIM_TWIRL, true, 0.0f);
                            mFlipTimer = 0;
                        }
                    }
                }
            }
            else {
                if(mFacingLeft) {
                    mFlipped = true;
                }
                else {
                    mFlipped = false;
                }
            }
            if(fabs(mVelocity.x) > 4 || mVelocity.Magnitude2() == 0) {
                mFlipTimer++;
            }
            else {
                mFlipTimer = 0;
            }
            if(mLeftHeld) {
                if(!mFlipped && mVelocity.x == 0) {
                    mFlipTimer = 60;
                }
            }
            else if(mRightHeld) {
                if(mFlipped && mVelocity.x == 0) {
                    mFlipTimer = 60;
                }
            }
        }
    }
    void WatershedPlayer::Update(double t, const std::vector<RigidBody*>& grabbableObjects) {
        Update(t);
        CheckGrabbleObjects(grabbableObjects);
    }
    void WatershedPlayer::Update(double t) {
        mStep = t;
        mVelocity = GetPosition() - mLastPos;
        float subDepth = mHitBox.GetHeight() / mSubmergeDepth;
        if(!mHurling && ((mUnderwater && subDepth >= 1.5) || mSwimming)) {
            SetFins(true);
        }
        else {
            mSurfaced = false;
        }
        if(!mHurling && !mUnderwater && mGrounded && GetCurrentAnimID() != IONA_LAND_FINS) {
            SetFins(false);
        }
        mFocusTimer+= mStep;
        if(!GetKeyDown(PKEY_BUTTON1)) {
            if(mFocusTimer >= 0.5f) {
                if(mFocusMode && mSwimming && mFacingLeft) {
                    mSwimAngle = PI;
                    mFlipped = true;
                }
                mFocusMode = false;
                mFocusTimer = 0.0f;
            }
        }
        if(mFocusMode) {
            mFlipTimer = 0;
        }
        if(GetCurrentAnimID() != IONA_LAND_FINS || (GetCurrentAnimID() == IONA_LAND_FINS && GetCurrentFrameNum() >= 9)) {
            CheckInput();
        }
        CheckJump();
        CheckPlatform();
        Dive();
        Walk();
        Accelerate();
        Fall();
        Float();
        Surface();
        if(mSwimming) {
            mLastPos = GetPosition() - mVelocity;
        }
        else {
            mVelocity = GetPosition() - mLastPos;
            mSwimVelocity = 0.0f;
        }
        if(mFocusMode || mSurfacing || !mSwimming) {
            mSwimAngle = 0.0f;
        }

        VerletNoVelocity(t);
        CheckFlip();
        Shoot();
        CheckAnimations();
        Recover();
        if(!mUnderwater && !mSpecialAnim && !mFocusMode && !mLeftHeld && !mRightHeld) {
            if(mFacingLeft) {
                mFlipped = true;
            }
            else {
                mFlipped = false;
            }
        }
        mSubmergeDepth = 0;
        //mGrounded = false;
        ClearColliders();
        UpdateHeldObject();
        UpdateRod(t);
        mSlopeForce = 0.0f;
        if(mFlipped) {
            if(mSurfacing) {
                mHitBox.SetAngle(0);
                mSwimAngle = DegreesToRadians(90);
            }
        }
        if(mShellFlipped != mFlipped) {
            mShellFlipped = mFlipped;
            mShellModel.ChangeAnimation(4, 0.0f);
        }
        if(mShellModel.GetCurrentAnim()->Finished()) {
            mShellModel.ChangeAnimation(0, 0.0);
        }
        mHurlHitbox.SetPosition(mHitBox.GetPosition() + Vector2f(0.0, 30.0));
    }
    void WatershedPlayer::Update(std::vector<SDL_Event>& events, float tstep) {
        mVelocity = GetPosition() - mLastPos;
        mStep = tstep;
        UpdateKeys(events);
        Update(tstep);
    }
    void WatershedPlayer::CheckGrabbleObjects(const std::vector<RigidBody*>& grabbableObjects) {
        if(GetKeyDown(PKEY_ATTACK)) {
            if(mHeldObject && (GetCurrentAnimID() < IONA_HOOK || GetCurrentAnimID() > IONA_HOOK_PULL_FINS)) {
                if(mDownHeld) {
                    DropHeldObject();
                }
                else {
                    mModel.SwapBoneGroup(mAnimationCache[IONA_SHOOT_RECOILA], 0);
                    mRecoiling = true;
                    ThrowObject();
                }
            }
            else if(!mHeldObject) {
                if(mLeftHeld) {
                    mFlipped = true;
                }
                if(mRightHeld) {
                    mFlipped = false;
                }
                if(!mFins) {
                    ChangeAnimIDSpecial(IONA_HOOK, false, 0.05f);
                }
                if(mFins) {
                    ChangeAnimIDSpecial(IONA_HOOK_FINS, false, 0.05f);
                }
            }
            mActionKeys[PKEY_ATTACK].keyState = KEY_DOWNCHECKED;
        }
        if(GetCurrentAnimID() == IONA_HOOK || GetCurrentAnimID() == IONA_HOOK_FINS) {
            if(GetCurrentFrameNum() >= 14 && GetCurrentFrameNum() <= 19) {
                if(!mHeldObject) {
                    Vector2f center = GetHitBox().GetCenter();
                    RigidBody* closestBody = NULL;
                    float closestDistance = FLT_MAX;
                    for(auto& body : grabbableObjects) {
                        //Vector2f dir = mFlipped ? Vector2f(-1.0, FLT_MAX) : Vector2f(1.0f, FLT_MAX);
                        Rectanglef grabBox = GetGrabRect();
                        if(RectRectCollision(body->GetHitBox(), grabBox)) {
                            float dist = body->GetHitBox().GetCenter().Distance(mHitBox.GetCenter());
                            if(dist < closestDistance) {
                                closestBody = body;
                                closestDistance = dist;
                            }
                        }
                    }
                    if(closestBody) {
                        GrabObject(closestBody);
                        if(!mFins) {
                            mCancelableSpecial = true;
                            ChangeAnimIDSpecial(IONA_HOOK_PULL, false, 0.0f);
                        }
                        else {
                            mCancelableSpecial = true;
                            ChangeAnimIDSpecial(IONA_HOOK_PULL_FINS, false, 0.0f);
                        }
                    }
                }
            }
        }
        if(!mObjectPulled && mHeldObject && GetCurrentFrameNum() >= 6) {
            mObjectPulled = true;
        }
        if(!mObjectPulled && (GetCurrentAnimID() < IONA_HOOK || GetCurrentAnimID() > IONA_HOOK_PULL_FINS)) {
            mHeldObject = NULL;
        }
                if(mObjectPulled && mHeldObject && mGrabObjDelay < 10) {
            mGrabObjDelay++;
        }
        if(mObjectPulled && mHeldObject) {
            float dist = mHeldObject->GetPosition().Distance(mHitBox.GetCenter());
            float maxDist = mGrabRange * mGrabRange * 0.75;
            if(dist > maxDist && mGrabObjDelay >= 10) {
                if(mUnderwater) {
                    ChangeAnimIDSpecial(IONA_HOOK_BREAK_FINS, true, 0.0f);
                }
                else {
                    ChangeAnimIDSpecial(IONA_HOOK_BREAK, true, 0.0f);
                }
                DropHeldObject();
            }
            else {
                mLassoChain.Update(mStep);
                mHeldObject->DampX(0.85f);
            }
        }
    }
    void WatershedPlayer::Hurt(float damage, Vector2f kb, bool knockback) {
        mHealth-= damage;
        if(mHealth < 0) {
            mHealth = 0;
        }
        mLastPos = GetPosition();
        ApplyImpulse(kb);
        if(kb.x > 0.0) {
            mFlipped = true;
            mFacingLeft = true;
            mSwimAngle = PI;
        }
        else if(kb.x < 0.0) {
            mFlipped = false;
            mFacingLeft = false;
            mSwimAngle = 0.0f;
        }
        if(knockback) {
            mHurling = true;
            mHurlAngle = 0.0f;
        }
        if(!mSwimming) {
            if(knockback) {
                ChangeAnimIDSpecial(IONA_KNOCKBACKA, false, 0.0f);
            }
            else {
                ChangeAnimIDSpecial(IONA_HITSTUN1A, false, 0.0f);
            }
        }
        else {
            if(knockback) {
                ChangeAnimIDSpecial(IONA_KNOCKBACKB, false, 0.0f);
            }
            else {
                ChangeAnimIDSpecial(IONA_HITSTUN1B, false, 0.0f);
            }
        }
    }
    void WatershedPlayer::Recover() {
        if(mKnockedDown && GetCurrentAnimID() > 0 ) {
            mKnockdownTime+= mStep;
            if(mKnockdownTime >= mMaxKnockdownTime || (GetCurrentAnimID() >= mModel.GetCurrentAnim()->GetLoopFrame() && AnyButtonPressed())) {
                mKnockdownTime = 0.0f;
                mSpecialAnim = false;
                mCancelableSpecial = true;
                mKnockedDown = false;
                if(GetCurrentAnimID() == IONA_KNOCKDOWN1A) {
                    ChangeAnimIDSpecial(IONA_KDRECOV1A, false);
                }
                else if(GetCurrentAnimID() == IONA_KNOCKDOWN1B) {
                    ChangeAnimIDSpecial(IONA_KDRECOV1B, false);
                }
                else if(GetCurrentAnimID() == IONA_KNOCKDOWN2A) {
                    ChangeAnimIDSpecial(IONA_KDRECOV2A, false);
                }
                else if(GetCurrentAnimID() == IONA_KNOCKDOWN2B) {
                    ChangeAnimIDSpecial(IONA_KDRECOV2B, false);
                }
            }
        }
        if(mHurling) {
            if(mHurlAngle <= PI * 0.5f && mFlipped) {
                mHurlAngle+= RADIAN * 5;
            }
            else if(mHurlAngle >= -PI * 0.5f && !mFlipped) {
                mHurlAngle-= RADIAN * 5;
            }
            if(mSwimming) {
                if(mVelocity.y >= 0.0) {
                    ApplyForceY(-mGravityAccel);
                }
                if(mVelocity.x > 0.0) {
                    ApplyForceX(-1500);
                }
                else if(mVelocity.x < 0.0) {
                    ApplyForceX(1500);
                }
                //mLastPos.x = GetPosition().x - mVelocity.x * 0.95;
            }
            if(GetCurrentFrameNum() >= 5) {
                if(mGrounded || mTouchingWall || (mSwimming && mVelocity.y <= 0.0 && fabs(mVelocity.x) <= 1.5) || (!mSwimming && mUnderwater)) {
                    ForceRecover();
                }
            }
        }
        else if(GetCurrentAnimID() != IONA_KDRECOV1B && GetCurrentAnimID() != IONA_KDRECOV2B) {
            mHurlAngle = 0.0f;
        }
        if(mModel.GetCurrentAnim()->Finished()) {
            if(GetCurrentAnimID() == IONA_HITSTUN1A) {
                mSpecialAnim = false;
                mCancelableSpecial = true;
                mHurling = false;
                ChangeAnimIDSpecial(ANIM_IDLE, false, 0.2f);
            }
            else if(GetCurrentAnimID() == IONA_HITSTUN1B) {
                mSpecialAnim = false;
                mCancelableSpecial = true;
                mHurling = false;
                if(mSwimming) {
                    ChangeAnimIDSpecial(IONA_SWIM_IDLE, false, 0.2f);
                }
                else {
                    ChangeAnimIDSpecial(IONA_MERMAID_IDLE, false, 0.2f);
                }
            }
        }
    }
    void WatershedPlayer::ForceRecover() {
        mLastPos = GetPosition();
        mCancelableSpecial = true;
        mHurling = false;
        mKnockdownTime = 0.0f;
        Stop();
        mSwimVelocity = 0.0f;
        mHurlAngle = 0.0f;
        if(mSwimming) {
            mSwimAngle = PI * 0.5f;
        }
        if(mUnderwater) {
            if(mFins) {
                ChangeAnimIDSpecial(IONA_KNOCKDOWN2B, false);
            }
            else {
                ChangeAnimIDSpecial(IONA_KNOCKDOWN2A, false);
            }
        }
        else {
            if(mFins) {
                ChangeAnimIDSpecial(IONA_KNOCKDOWN1B, false);
            }
            else {
                ChangeAnimIDSpecial(IONA_KNOCKDOWN1A, false);
            }
        }
        mKnockedDown = true;
    }
    void WatershedPlayer::UpdateRod(double d) {
        mRodVisible = false;
        if((GetCurrentAnimID() == IONA_HOOK || GetCurrentAnimID() == IONA_HOOK_FINS)) {
            if(GetCurrentFrameNum() == 2) {
                if(mHoloRod.GetCurrentAnimID() != 1) {
                    mHoloRod.ChangeAnimation(1, 0.0f);
                    mHoloRod.GetCurrentAnim()->SetTime(mModel.GetCurrentAnim()->GetCurTime());
                }
            }
            else if(GetCurrentFrameNum() >= 30) {
                mShellModel.ChangeAnimation(2, 0.0f);
            }
            if(GetCurrentFrameNum() >= 2 && GetCurrentFrameNum() < 30) {
                mRodVisible = true;
            }
            else {
                mHoloRod.ChangeAnimation(0, 0.0f);
                mRodVisible = false;
            }
        }
        else if(GetCurrentAnimID() == IONA_HOOK_PULL || GetCurrentAnimID() == IONA_HOOK_PULL_FINS) {
            if(mHoloRod.GetCurrentAnimID() != 2) {
                mHoloRod.ChangeAnimation(2, 0.0f);
                mHoloRod.GetCurrentAnim()->SetTime(mModel.GetCurrentAnim()->GetCurTime());
            }
            if(GetCurrentFrameNum() <= 17) {
                mRodVisible = true;
            }
            else if(GetCurrentFrameNum() > 17) {
                mShellModel.ChangeAnimation(3, 0.0f);
            }
            else {
                mHoloRod.ChangeAnimation(0, 0.0f);
                mRodVisible = false;
            }
        }
        else {
            mHoloRod.ChangeAnimation(0, 0.0f);
            mRodVisible = false;
        }
    }

    void WatershedPlayer::CheckAndResolveCollision(RigidBody* b) {
        if(b == mHeldObject || mPrevHeldObject == b) {
            return;
        }
        else {
            BasePlayer2::CheckAndResolveCollision(b);
        }
    }
    bool WatershedPlayer::CheckCollision(WaterBody& waterBody) {
        float depth = waterBody.CheckCollision(GetHitBox());
        if(depth > 0.0f) {
            if(!mUnderwater && !mSwimming) {
                mUnderwater = true;
                if(mSubmergeDepth < mHitBox.GetHeight() / 2) {
                    if(mVelocity.y < 0) {
                        float splashStrength = 1.25f * mVelocity.y;
                        if(mHurling) {
                            splashStrength = -2.0f * mVelocity.y;
                            ForceRecover();
                        }
                        waterBody.Splash(mHitBox, splashStrength);
                        waterBody.Splash(mHitBox, splashStrength);
                        mLastPos = mHitBox.GetPosition() - (mHitBox.GetPosition() - mLastPos) * mWaterDamping;
                    }
                    else {
                        waterBody.Splash(mHitBox, mVelocity.y * 0.25);
                    }
                }
            }
            else if(GetCurrentAnimID() == IONA_MERMAID_SURFACE && GetCurrentFrameNum() == 0) {
                waterBody.Splash(mHitBox, 10);
            }
            mSubmergeDepth = depth;
        }
        return depth > 0.0f;
    }
    void WatershedPlayer::ResolveCollision(Collision& c, float mass, Vector2f vel) {
        CheckCollisions(c.mtv);
        if(!mSwimming) {
            if(c.normal.y > 0.2 && c.normal.x) {
                float yMove = (c.mtv.x * c.mtv.x) / c.mtv.y + c.mtv.y;
                AddY(yMove * mass);
            }
            else {
                AddXY(c.mtv * mass);
            }
            mLastPos-= vel * mass;
            if(c.normal.y >= 0.3 && vel.y < 0.0) {
                mLastPos.y = GetPosition().y;
            }
            if(c.normal.y >= 0.0f && !mUnderwater) {
                if(!mFins && fabs(mVelocity.x) < 0.01f && mVelocity.y <= -3.0f) {
                    ChangeAnimIDSpecial(ANIM_LAND, true, 0.0f);
                }
                if(mFins && mVelocity.y < 0.0f) {
                    if(mHurling) {
                        ChangeAnimIDSpecial(IONA_LAND_FINS, false, 0.0f);
                    }
                    else {
                        mCancelableSpecial = true;
                        ChangeAnimIDSpecial(IONA_KDRECOV2A, true, 0.0f);
                    }
                    mFocusMode = false;
                    mActionKeys[PKEY_BUTTON1].keyState = KEY_DOWNCHECKED;
                    StopX();
                }
                if(mVelocity.y <= 0.0f && c.normal.y >= 0.75f) {
                    SetGrounded(true);
                }
                if(mVelocity.y <= 2.0f) {
                    if(c.normal.y < 0.75f) {
                        mSlopeForce = mWalkSpeed;
                    }
                }
            }
        }
        else {
            mSwimVelocity+= vel * mass;
            if(mHurling) {
                mSwimVelocity = 0.0f;
                mLastPos = GetPosition();
                ForceRecover();
            }
            AddXY(c.mtv * mass);
        }
        if(c.mtv.x) {
            mTouchingWall = true;
        }
    }
    void WatershedPlayer::ResolveCollision(RigidBody* b, Collision& c, Vector2f vel) {
        CheckCollisions(c.mtv);
        b->CheckCollisions(-c.mtv);
        if(c.normal.y >= 0.5) {
            mGrounded = true;
            if(fabs(mVelocity.x) < 0.01f) {
                if(!mFins && mVelocity.y <= -2.0f) {
                    ChangeAnimIDSpecial(ANIM_LAND, true, 0.0f);
                }
                if(mFins) {
                    ChangeAnimIDSpecial(IONA_LAND_FINS, false, 0.0f);
                    mFocusMode = false;
                    mActionKeys[PKEY_BUTTON1].keyState = KEY_DOWNCHECKED;
                    StopX();
                }
            }
        }
        if(!mSwimming) {
            BasePlayer2::ResolveCollision(b, c, vel);
        }
        else {
            float massA = GetInvMass() / (GetInvMass() + b->GetInvMass());
            float massB = b->GetInvMass() / (GetInvMass() + b->GetInvMass());
            mSwimVelocity+= vel * massA;
            AddXY(c.mtv * massA);
            b->GetLastPosition()+= vel * massB;
            if(mHurling) {
                mSwimVelocity = 0.0f;
                mLastPos = GetPosition();
                ForceRecover();
            }
            b->AddXY(c.mtv * massB);
            mTouchingWall = true;
        }
    }
    void WatershedPlayer::CheckBulletCollisions(LevelRoom& room) {
        std::vector<BackgroundTile2>& tiles = room.GetGroundTiles();
        std::vector<RigidBody*>& bodies = room.GetBodyPointers();
        for(auto& bullet : mBulletTypes) {

            for(auto& tile : tiles) {
                bullet.CheckCollisions(tile);
            }
            for(auto& body : bodies) {
                bullet.CheckCollisions(body);
            }
        }
    }
    void  WatershedPlayer::CheckPlatform(Collision& c, BackgroundTile2& bg) {
        if(c.normal.y >= 0.5) {
            mGrounded = true;
            if(fabs(mVelocity.x) < 0.01f && mVelocity.y <= -2.0f) {
                if(!mFins) {
                    ChangeAnimIDSpecial(ANIM_LAND, true, 0.0f);
                }
                if(mFins) {
                    ChangeAnimIDSpecial(IONA_LAND_FINS, false, 0.0f);
                    mFocusMode = false;
                    mActionKeys[PKEY_BUTTON1].keyState = KEY_DOWNCHECKED;
                    StopX();
                }
            }
        }
        RigidBody::CheckPlatform(c, bg);
    }
    void WatershedPlayer::ToggleSpellRight() {
        mActiveBulletType++;
        if(mActiveBulletType >= mBulletTypes.size()) {
            mActiveBulletType = 0;
        }
    }
    void WatershedPlayer::ToggleSpellLeft() {
        mActiveBulletType--;
        if(mActiveBulletType < 0) {
            mActiveBulletType = mBulletTypes.size() - 1;
        }
    }
    void WatershedPlayer::Shoot() {
        if(mShootResetTimer > 0) {
            mShootResetTimer--;
        }
        for(auto& bullet : mBulletTypes) {
            bullet.Update(mStep);
        }
        if(GetKeyDown(PKEY_SHOOT)) {
            mActionKeys[PKEY_SHOOT].keyState = KEY_DOWNCHECKED;
            if(mActiveBulletType < mBulletTypes.size() && mActiveBulletType >= 0) {
                Vector2f shootDirection, shotOrigin = mShotOrigin;
                Vector2f startVel;
                startVel.x = mVelocity.x;
                if(!mFocusMode) {
                    if((mFacingLeft && !mRightHeld) || mLeftHeld) {
                        shootDirection.x = -1.0;
                        shotOrigin.x = -shotOrigin.x;
                    }
                    else {
                        shootDirection.x = 1.0;
                    }
                }
                else {
                    if(mFacingLeft) {
                        shootDirection.x = -1.0;
                        shotOrigin.x = -shotOrigin.x;
                    }
                    else {
                        shootDirection.x = 1.0;
                    }
                }
                if(mUpHeld) {
                    shootDirection.y = 1.0;
                    shootDirection.x = 0.0f;
                }
                if(mDownHeld) {
                    shootDirection.y = -1.0;
                    shootDirection.x = 0.0f;
                }
                if(mBulletTypes[mActiveBulletType].CreateShot(shootDirection.GetUnit(), shotOrigin + GetPosition(), 0.0)) {
                    if(mSkidding) {
                        mSkidding = false;
                        mSpecialAnim = false;
                    }
                    mShootResetTimer = (mBulletTypes[mActiveBulletType].GetFireRate() / mStep) + 10;
                    if(mLeftHeld || (mFacingLeft && !mRightHeld)) {
                        mFlipped = true;
                    }
                    else if(mRightHeld) {
                        mFlipped = false;
                    }
                    if(mSwimming) {
                        if(GetCurrentAnimID() == IONA_SWIM_TWIRL) {
                            mSwimAngle = atan2(shootDirection.y, shootDirection.x);
                        }
                        else {
                            if(mFlipped) {
                                mSwimAngle = PI;
                            }
                            else {
                                mSwimAngle = 0.0f;
                            }
                        }
                    }
                }
            }
        }
        if(mShootResetTimer) {
            if(mSkidding) {
                mSkidding = false;
                ChangeAnimID(IONA_SWIM_IDLE);
            }
        }
    }
    void WatershedPlayer::ShootBullet(Bullet b) {
        if(mLastInactiveBullet < mBullets.size()) {
            mBullets[mLastInactiveBullet++] = b;
        }
    }
    void WatershedPlayer::AdjustVelocity() {
        if(!mSwimming) {
            BasePlayer2::AdjustVelocity();
            mSwimVelocity = 0;
        }
        else {
            Vector2f maxVel = mSwimVelocity * mSwimMaxVel;
            //mVelocity = mSwimVel2 * mSwimVelocity;
            if(mVelocity.x > 0 && mVelocity.x > maxVel.x) {
                mVelocity.x = maxVel.x;
            }
            if(mVelocity.x < 0 && mVelocity.x < maxVel.x) {
                mVelocity.x = maxVel.x;
            }
            if(mVelocity.y > 0 &&mVelocity.y > maxVel.y) {
                mVelocity.y = maxVel.y;
            }
            if(mVelocity.y < 0 && mVelocity.y < maxVel.y) {
                mVelocity.y = maxVel.y;
            }
        }
    }
    void WatershedPlayer::AdjustVelocity(Vector2f& vel) {
        float diff = fabs(vel.x) - (mWalkAccel * mStep * mStep);
        if(diff < 0.01f && !mRightHeld && !mLeftHeld) {
            vel.x = 0.0f;
        }
    }
    void WatershedPlayer::UpdateAnimation(float tstep) {
        /*
        if(!mSwimming) {
            if(mVelocity.x > 0 || (mRightHeld && !mLeftHeld)) {
                mFlipped = false;
            }
            if(mVelocity.x < 0 || (!mRightHeld && mLeftHeld)) {
                mFlipped = true;
            }
            mCurrentAnimation.SetHoriFlip(mFlipped);
            mCurrentAnimation.SetVertFlip(false);
            mCurrentAnimation.SetAngle(0);
            mLegAnim.SetHoriFlip(mFlipped);
            mLegAnim.SetVertFlip(false);
            mFlipTimer = 0;
        }
        else {
            if(mFlipTimer >= 30 && !mFocusMode) {
                if(mSwimAngle < DegreesToRadians(-90) || mSwimAngle > DegreesToRadians(90)) {
                    if(!mFlipped) {
                        //ChangeAnimation("flipping");
                        ChangeAnimID(IONA_SWIM_TWIRL, 0.0f);
                        mSpecialAnim = true;
                        mFlipTimer = 0;
                    }
                }
                else if(mSwimAngle > DegreesToRadians(-90) && mSwimAngle < DegreesToRadians(90)) {
                    if(mFlipped) {
                        //ChangeAnimation("flipping");
                        ChangeAnimID(IONA_SWIM_TWIRL, 0.0f);
                        mSpecialAnim = true;
                        mFlipTimer = 0;
                    }
                }
            }

            mCurrentAnimation.SetAngle(mSwimAngle);
            mLegAnim.SetAngle(mSwimAngle);
            mCurrentAnimation.SetHoriFlip(false);
            mCurrentAnimation.SetVertFlip(mFlipped);
            mLegAnim.SetHoriFlip(false);
            mLegAnim.SetVertFlip(mFlipped);
        }
        mCurrentAnimation.SetPosition((Vector2i)mHitBox.GetOrigin());
        mCurrentAnimation.Update(tstep);
        if(mCurrentAnimation.Finished()) {
            mSpecialAnim = false;
        }
        mLegAnim.SetPosition((Vector2i)mHitBox.GetOrigin());
        mLegAnim.Update(tstep);
        UpdateHeldObject();
        */
    }
    void WatershedPlayer::UpdateModel(float t, Vector2f scale) {
        if(mFins) {
            mModel.GetMeshPtr("Iona-LowerTail")->SetHidden(false);
            mModel.GetMeshPtr("Iona-LowerLegs")->SetHidden(true);
        }
        else {
            mModel.GetMeshPtr("Iona-LowerTail")->SetHidden(true);
            mModel.GetMeshPtr("Iona-LowerLegs")->SetHidden(false);
        }

        glm::vec3 modelScale = mModel.GetTransform().scale;
        if(mFocusMode) {
            if(mFacingLeft) {
                mFlipped = true;
            }
            else {
                mFlipped = false;
            }
        }
        if(!mSwimming || mFocusMode) {
            if(mFlipped) {
                modelScale.z = -fabs(modelScale.z);
            }
            else {
                modelScale.z = fabs(modelScale.z);
            }
            modelScale.y = fabs(modelScale.y);
        }
        else {
            if(mFlipped) {
                modelScale.y = -fabs(modelScale.y);
            }
            else {
                modelScale.y = fabs(modelScale.y);
            }
            modelScale.z = fabs(modelScale.z);
        }
        glm::vec3 pos = GetWorldPos(scale.x, scale.y);
        float xAngle = mModel.GetTransform().xAngle;
        float yAngle = 0.0f;
        if(mSwimming && !mFocusMode && !mHurling) {
            yAngle = -mSwimAngle;
        }
        yAngle+= mHurlAngle;
        mModel.SetAngle(xAngle, yAngle, 0.0f);
        mModel.SetScale(modelScale);
        mModel.SetTranslation(pos);

        mModel.Update(t);
        Bone& handBone = mModel.GetCurrentAnim()->GetCurrentFrame().bones[mHandAnchorID];
        Bone& headBone = mModel.GetCurrentAnim()->GetCurrentFrame().bones[mHeadAnchorID];
        if(mRodVisible) {
            int k = mHoloRod.GetBoneID("Rod_Base");
            Bone& rodBone = mHoloRod.GetCurrentAnim()->GetCurrentFrame().bones[k];
            mHoloRod.GetCurrentAnim()->SetParentMatrix(handBone.finalTransformation * rodBone.offsetMatrix);
            mHoloRod.Update(t);
        }
        else {
            Bone& shellBone = mShellModel.GetCurrentAnim()->GetCurrentFrame().bones[0];
            mShellModel.GetCurrentAnim()->SetParentMatrix(handBone.finalTransformation * shellBone.offsetMatrix);
            mShellModel.Update(t);
        }
        mHairband.GetCurrentAnim()->SetParentMatrix(headBone.finalTransformation * headBone.offsetMatrix);
        mHairband.Update(t);
    }
    void WatershedPlayer::Draw(ShaderProgram& shader, TextureArray& textures) {
        if(mUsesModel) {
            glm::mat4 trans = mModel.GetTransform().GetTransform();
            mModel.Draw(shader, textures, trans);
            mHairband.Draw(shader, textures, trans);
            //mHairband.Draw(shader, textures, mHairband.GetTransform().GetTransform());
            if(mRodVisible) {
                mHoloRod.Draw(shader, textures, trans);
                //mHoloRod.Draw(shader, textures, mHoloRod.GetTransform().GetTransform());
            }
            else {
                mShellModel.Draw(shader, textures, trans);
                //mShellModel.Draw(shader, textures, mShellModel.GetTransform().GetTransform());
            }
        }
    }
    void WatershedPlayer::DrawBullets(SceneRenderer& rend) {
        for(auto& bullet : mBulletTypes) {
            bullet.Draw(rend);
        }
    }
    void WatershedPlayer::CheckAnimationsSprite() {
        /*
        if(mSwimming) {
            if(mCurrentAnimation.Finished() && mSpecialAnim) {
                mSpecialAnim = false;
            }
            if(mCurrentAnimation == "flipping" && mCurrentAnimation.Finished()) {
                mFlipped = !mFlipped;
            }
            if(mCurrentAnimation == "turning") {
                if(mCurrentAnimation.Finished()) {
                    mTurning = false;
                }
            }
            else {
                mTurning = false;
            }
            if(mSkidding && !mTurning) {
                ChangeAnimation("skidding");
                mSpecialAnim = true;
                if(mCurrentAnimation == "skidding" && mCurrentAnimation.Finished()) {
                    mSkidding = false;
                    mSpecialAnim = false;
                }
            }
            if(mVelocity.Magnitude2() > 0.0f) {
                if(mCurrentAnimation != "skidding")
                    mSwimAngle = std::atan2(mVelocity.y, mVelocity.x);
                if(!mSpecialAnim) {
                    if(mCurrentAnimation != "swimming" && mCurrentAnimation != "flipping" && mCurrentAnimation != "skidrecov") {
                        ChangeAnimation("turning");
                    }
                    if((mCurrentAnimation == "turning" || mCurrentAnimation == "flipping" || mCurrentAnimation == "skidrecov") && mCurrentAnimation.Finished()) {
                        ChangeAnimation("swimming");
                    }
                }
                mHitBox.SetAngle(mSwimAngle + DegreesToRadians(-90));
            }
            else {
                if(!mSpecialAnim && !mSurfacing) {
                    if(mCurrentAnimation == IONA_SWIM) {
                        ChangeAnimation("swimstart");
                    }
                    else if(mCurrentAnimation != "swimstart" || (mCurrentAnimation == "swimstart" && mCurrentAnimation.Finished())) {
                        ChangeAnimation("idle");
                    }
                }
                mHitBox.SetAngle(mSwimAngle);
            }
        }
        else if(mUnderwater) {
            if(mVelocity.y <= 1.0f && mVelocity.y >= -1.0f ) {
                ChangeAnimation("swimIdle");
            }
            if(mCurrentAnimation == "swimIdle" && mVelocity.y >= 0.0f) {
                ApplyForceY(0.125f);
            }
        }
        else {
            mHitBox.SetAngle(0);
            mCurrentAnimation.SetAngle(0);
            mLegAnim.SetAngle(0);
            BasePlayer2::CheckAnimations();
        }
        */
    }
    void WatershedPlayer::CheckFocusAnimationsModel() {
        if(!UsesFocusAnims() || (mSpecialAnim && !mCancelableSpecial)) {
            return;
        }
        if(mUnderwater) {
            if(mVelocity.Magnitude2() > 0 && !mSurfacing) {
                if((UsesFocusAnims() && !mSpecialAnim) || (mSpecialAnim && GetCurrentAnimFinished()) ||
                   (GetCurrentAnimID() == IONA_SWIM_STOPBRAKE && mSwimVelocity.Magnitude2() <= mFocusModifier * mSwimMaxVel && !mShootResetTimer))
                {
                    float transitionSpeed = 0.05f;
                    if(!mFacingLeft) {
                        if(mVelocity.x > 0) {
                            ChangeAnimID(IONA_SWIMSHOOT_FORWARD, transitionSpeed);
                        }
                        else if(mVelocity.x < 0) {
                            if(mFocusMode || mHeldObject) {
                                ChangeAnimID(IONA_SWIMSHOOT_BACK, transitionSpeed);
                            }
                            else {
                                ChangeAnimID(IONA_SWIMSHOOT_FORWARD, transitionSpeed);
                            }
                        }
                        else if(mSwimming) {
                            if(mVelocity.y > 0) {
                                ChangeAnimID(IONA_SWIMSHOOT_UP, transitionSpeed);
                            }
                            else if(mVelocity.y < 0) {
                                ChangeAnimID(IONA_SWIMSHOOT_DOWN, transitionSpeed);
                            }
                            else {
                                ChangeAnimID(IONA_SWIMSHOOT_FORWARD, transitionSpeed);
                            }
                        }
                        else {
                            ChangeAnimID(IONA_SWIMSHOOT_IDLE);
                        }
                    }
                    else {
                        if(mVelocity.x < 0) {
                            ChangeAnimID(IONA_SWIMSHOOT_FORWARD, transitionSpeed);
                        }
                        else if(mVelocity.x > 0) {
                            if(mFocusMode || mHeldObject) {
                                ChangeAnimID(IONA_SWIMSHOOT_BACK, transitionSpeed);
                            }
                            else {
                                ChangeAnimID(IONA_SWIMSHOOT_FORWARD, transitionSpeed);
                            }
                        }
                        else if(mSwimming) {
                            if(mVelocity.y > 0) {
                                ChangeAnimID(IONA_SWIMSHOOT_UP, transitionSpeed);
                            }
                            else if(mVelocity.y < 0) {
                                ChangeAnimID(IONA_SWIMSHOOT_DOWN, transitionSpeed);
                            }
                            else {
                                ChangeAnimID(IONA_SWIMSHOOT_FORWARD, transitionSpeed);
                            }
                        }
                        else {
                            ChangeAnimID(IONA_SWIMSHOOT_IDLE);
                        }
                    }
                    mHitBox.SetAngle(0.0f);
                }
            }
            else {
                ChangeAnimID(IONA_SWIMSHOOT_IDLE);
                mHitBox.SetAngle(0.0f);
            }
        }
        else if(GetCurrentAnimID() != IONA_SHOOT_BRAKE) {
            if(mGrounded) {
                if(mVelocity.x > 0) {
                    if(!mFacingLeft) {
                        if(mDownHeld) {
                            ChangeAnimID(IONA_RUNSHOOT_DFORWARD, 0.05f);
                        }
                        else if(mUpHeld) {
                            ChangeAnimID(IONA_RUNSHOOT_UFORWARD, 0.05f);
                        }
                        else {
                            ChangeAnimID(IONA_RUNSHOOT_FORWARD, 0.05f);
                        }
                    }
                    else {
                        if(mFocusMode) {
                            ChangeAnimID(IONA_RUNSHOOT_BACK, 0.05f);
                        }
                        else {
                            if(mDownHeld) {
                                ChangeAnimID(IONA_RUNSHOOT_DFORWARD, 0.05f);
                            }
                            else if(mUpHeld) {
                                ChangeAnimID(IONA_RUNSHOOT_UFORWARD, 0.05f);
                            }
                            else {
                                ChangeAnimID(IONA_RUNSHOOT_FORWARD, 0.0f);
                            }
                        }
                    }
                }
                else if(mVelocity.x < 0) {
                    if(!mFacingLeft) {
                        if(mFocusMode) {
                            ChangeAnimID(IONA_RUNSHOOT_BACK, 0.0f);
                        }
                        else {
                            if(mDownHeld) {
                                ChangeAnimID(IONA_RUNSHOOT_DFORWARD, 0.05f);
                            }
                            else if(mUpHeld) {
                                ChangeAnimID(IONA_RUNSHOOT_UFORWARD, 0.05f);
                            }
                            else {
                                ChangeAnimID(IONA_RUNSHOOT_FORWARD, 0.0f);
                            }
                        }
                    }
                    else {
                        if(mDownHeld) {
                            ChangeAnimID(IONA_RUNSHOOT_DFORWARD, 0.05f);
                        }
                        else if(mUpHeld) {
                            ChangeAnimID(IONA_RUNSHOOT_UFORWARD, 0.05f);
                        }
                        else {
                            ChangeAnimID(IONA_RUNSHOOT_FORWARD, 0.0f);
                        }
                    }
                }
                else {
                    if(mDownHeld) {
                        ChangeAnimID(IONA_SHOOT_DOWN, 0.05f);
                    }
                    else if(mUpHeld) {
                        ChangeAnimID(IONA_SHOOT_UP, 0.05f);
                    }
                    else {
                        ChangeAnimID(IONA_SHOOT_GROUND, 0.05f);
                    }
                }
            }
            else if(mVelocity.y > 0.0f || mVelocity.y <= -1.0f) {
                float transSpeed = 0.0f;
                if(GetCurrentAnimID() == IONA_SHOOT_JUMP || GetCurrentAnimID() == IONA_SHOOT_BACK_JUMP) {
                    transSpeed = 0.05f;
                }
                if(mVelocity.x > 0) {
                    if(!mFacingLeft) {
                        ChangeAnimID(IONA_SHOOT_JUMP, transSpeed);
                    }
                    else {
                        if(mFocusMode) {
                            ChangeAnimID(IONA_SHOOT_BACK_JUMP, transSpeed);
                        }
                        else {
                            ChangeAnimID(IONA_SHOOT_JUMP, transSpeed);
                        }
                    }
                }
                else if(mVelocity.x < 0) {
                    if(!mFacingLeft) {
                        if(mFocusMode) {
                            ChangeAnimID(IONA_SHOOT_BACK_JUMP, transSpeed);
                        }
                        else {
                            ChangeAnimID(IONA_SHOOT_JUMP, transSpeed);
                        }
                    }
                    else {
                        ChangeAnimID(IONA_SHOOT_JUMP, transSpeed);
                    }
                }
                else if(GetCurrentAnimID() != IONA_SHOOT_BACK_JUMP) {
                    ChangeAnimID(IONA_SHOOT_JUMP, 0.0f);
                }
            }
        }
    }
    void WatershedPlayer::CheckAnimationsModel() {
        if(mModel.GetCurrentAnimNum() <= -1) {
            return;
        }
        if(UsesFocusAnims()) {
            CheckFocusAnimationsModel();
        }
        if(mRecoiling && GetCurrentAnim().SubAnimFinished()) {
            mRecoiling = false;
        }
        if(mSpecialAnim) {
            if(GetCurrentAnimID() <= IONA_FIN_FALL && GetCurrentAnimID() != ANIM_LAND) {
                mSpecialAnim = false;
                mCancelableSpecial = true;
            }
            if(GetCurrentAnimID() == IONA_MERMAID_SURFACE && mUpHeld && GetCurrentFrameNum() >= 5) {
                Jump();
                mSpecialAnim = false;
                mCancelableSpecial = true;
            }
            if(mVelocity.x && GetCurrentAnimID() == IONA_SHOOT_GROUND) {
                ChangeAnimIDSpecial(IONA_RUNSHOOT_FORWARD, true, 0.1f);
            }
            else if(GetCurrentAnimID() == IONA_IDLE_BRAKE) {
                if((mLeftHeld && mVelocity.x < 0.0f && !mFlipped) || (mRightHeld && mVelocity.x > 0.0f && mFlipped)) {
                    ChangeAnimIDSpecial(IONA_RUN_BRAKE, true, 0.0f);
                }
                else if((mLeftHeld && mVelocity.x < 0.0f && mFlipped) || (mRightHeld && mVelocity.x > 0.0f && !mFlipped)) {
                    mSpecialAnim = false;
                }
            }
            else if(GetCurrentAnimID() == IONA_RUN_BRAKE || GetCurrentAnimID() == IONA_SHOOT_BRAKE) {
                if(!mLeftHeld && !mRightHeld && fabs(mVelocity.x) < 0.25f ) {
                    mSpecialAnim = false;
                }
                if(mLeftHeld && mVelocity.x < 2.0f) {
                    mFlipped = false;
                }
                if(mRightHeld && mVelocity.x > -2.0) {
                    mFlipped = true;
                }
            }
            else if(GetCurrentAnimID() == IONA_SWIM_BRAKE && GetCurrentAnimFinished()) {
                mFlipped = !mFlipped;
                mFlipTimer = 0.0f;
                if(mLeftHeld || mRightHeld || mDownHeld || mUpHeld) {
                    TransitionToAnimation(IONA_SWIM, 0.1, 0, 10);
                }
            }
            if((GetCurrentAnimID() == IONA_KDRECOV2A || GetCurrentAnimID() == IONA_KDRECOV2A) && GetCurrentFrameNum() == 5) {
                SetFins(!mFins);
            }
            if(GetCurrentAnimID() == IONA_SWIM_STOPBRAKE) {
                if(mVelocity.Magnitude2() < 0.01f) {
                    ChangeAnimIDSpecial(IONA_SWIM_BRAKE_RECOV, true);
                }
                if(mLeftHeld || mRightHeld || mUpHeld || mDownHeld) {
                    mSpecialAnim = false;
                }
            }
            if(GetCurrentAnimID() == IONA_MERMAID_SURFACE && !mUnderwater) {
                mSpecialAnim = false;
            }
            if(GetCurrentAnimID() == IONA_RUNSHOOT_FORWARD && fabs(mVelocity.x) < 1.0) {
                ChangeAnimID(IONA_SHOOT_GROUND);
            }
            if(IsShooting() && !mShootResetTimer) {
                mSpecialAnim = false;
            }
            if(mModel.GetCurrentAnim()->Finished()) {
                mSpecialAnim = false;
                mCancelableSpecial = true;
                mHurling = false;
            }
            if(GetCurrentAnimID() == ANIM_IDLE || GetCurrentAnimID() == IONA_SWIM_IDLE) {
                mSpecialAnim = false;
                mCancelableSpecial = true;
            }

        }
        else {
            mCancelableSpecial = true;
        }
        if(mSwimming) {
            CheckSwimAnims();
        }
        else if(mUnderwater && !mSpecialAnim) {
            CheckSurfaceAnims();
        }
        else {
            CheckGroundAnims();
        }
        AdjustAnimations();
    }
    void WatershedPlayer::CheckSurfaceAnims() {
        float depth = (mSubmergeDepth / mHitBox.GetHeight());
        if(!UsesFocusAnims() && GetCurrentAnimID() != IONA_FIN_JUMP) {
            mHitBox.SetAngle(0);
            if(!mDiving) {
                if(GetCurrentAnimID() == IONA_MERMAID_SURFACE) {
                    if(GetCurrentAnimFinished()) {
                        mSurfaced = true;
                    }
                    if(mVelocity.y >= 2.0f) {
                        mLastPos.y = GetPosition().y - mVelocity.y * 0.95;
                    }
                }
                if(!mSurfaced && mVelocity.y >= -1.0f) {
                    ChangeAnimID(IONA_MERMAID_SURFACE, 0.0f);
                }
                else if(mSurfaced) {
                    if(mVelocity.x != 0.0f) {
                        ChangeAnimID(IONA_MERMAID_SWIM);
                    }
                    else if(!mLeftHeld && !mRightHeld) {
                       ChangeAnimID(IONA_MERMAID_IDLE);
                    }
                }
            }
            else {
                ChangeAnimID(IONA_MERMAID_DIVE, 0.0f);
            }
        }
    }
    void WatershedPlayer::CheckSwimAnims() {
        if(mKnockedDown || (mSpecialAnim && !mCancelableSpecial)) {
            return;
        }
        if(GetCurrentAnimID() == IONA_FIN_FALL) {
            ChangeAnimID(IONA_SWIM);
        }
        if(mModel.GetCurrentAnimID() == IONA_SWIM_TWIRL && mModel.GetCurrentAnim()->Finished()) {
            mFlipped = !mFlipped;
            mFlipTimer = 0.0f;
            //mSpecialAnim = false;
        }
        if(mSkidding && GetCurrentAnim() != IONA_SWIM_STOPBRAKE) {
            if(GetCurrentAnimID() == IONA_SWIM_TWIRL) {
                ChangeAnimIDSpecial(IONA_SWIM_BRAKE, true, 0.0f);
            }
            else {
                ChangeAnimIDSpecial(IONA_SWIM_BRAKE, true, 0.0f);
            }
            if(mModel.GetCurrentAnimID() == IONA_SWIM_BRAKE && mVelocity.Magnitude2() < 0.001f) {
                mSkidding = false;
                mSpecialAnim = false;
            }
        }
        if(!UsesFocusAnims() && !mSpecialAnim || (mSpecialAnim && mCancelableSpecial)) {
            if(mDiving) {
                ChangeAnimID(IONA_MERMAID_DIVE, 0.0);
            }
            if(mSurfacing) {
                if(mSubmergeDepth < 0.5f && !mSurfaced) {
                    ChangeAnimID(IONA_MERMAID_SURFACE);
                }
                else {
                    ChangeAnimID(IONA_SWIM_IDLE);
                }
            }
            if(mVelocity.Magnitude2() > 0.0f && !mSurfacing && !mDiving) {
                if(GetCurrentAnimID() == IONA_SWIMSHOOT_BACK || GetCurrentAnimID() == IONA_SWIMSHOOT_IDLE ) {
                    if(mFlipped && !mFacingLeft) {
                        ChangeAnimID(IONA_TURN_SWIM, 0.0f);
                        mSpecialAnim = true;
                        mSwimAngle = mSwimAngle = PI;
                    }
                    else if(!mFlipped && mFacingLeft) {
                        ChangeAnimID(IONA_TURN_SWIM, 0.0f);
                        mSpecialAnim = true;
                        mSwimAngle = 0.0f;
                    }
                }
                if(GetCurrentAnimID() == IONA_SWIM_BRAKE && !mSpecialAnim) {
                    mFlipped = !mFlipped;
                    mFlipTimer = 0.0f;
                    if(mFlipped) {
                        mSwimAngle = PI;
                    }
                    else {
                        mSwimAngle = 0.0f;
                    }
                    //mSwimAngle = atan2(mSwimDirection.y, mSwimDirection.x);
                    //std::cout << RadiansToDegrees(mSwimAccel) << std::endl;
                    //ChangeAnimID(IONA_SWIM);
                    //TransitionToAnimation(IONA_SWIM, 10.1, GetCurrentFrameNum(), 10);
                    TransitionToAnimation(IONA_SWIM, 0.1, 0, 11);
                }
                else if(mModel.GetCurrentAnimID() != IONA_SWIM_BRAKE && !GetKeyDown(PKEY_BUTTON1) && GetCurrentAnimID() != IONA_TURN_SWIM && !mShootResetTimer) {
                    float angleComp = RADIAN * 5;
                    if(mTouchingWall) {
                        float angleComp = RADIAN * 3;
                    }

                    float lastAngle = mSwimAngle;
                    float angle = std::atan2(mVelocity.y, mVelocity.x);
                    float angleDiff = DeltaAngle(lastAngle, angle);

                    if(angleDiff > angleComp) {
                        mSwimAngle+= angleComp;
                    }
                    else if(angleDiff < -angleComp) {
                        mSwimAngle-= angleComp;
                    }
                    else {
                        mSwimAngle = angle;
                    }
                }
                if(mModel.GetCurrentAnimID() == IONA_SWIM_TWIRL && mModel.GetCurrentAnim()->Finished()) {
                    ChangeAnimID(IONA_SWIM, 0.0f);
                    mModel.GetCurrentAnim()->ForceFrame(10);
                }
                else if(!mSpecialAnim) {
                    if(GetCurrentAnimID() == IONA_TURN_SWIM) {
                        mFlipped = !mFlipped;
                        mSwimAngle = atan2(mSwimDirection.y, mSwimDirection.x);
                        mFlipTimer = 0.0f;
                        ChangeAnimID(IONA_SWIM, 0.0f);
                        mModel.GetCurrentAnim()->ForceFrame(2);
                    }
                    else if(!mShootResetTimer && GetCurrentAnim() != IONA_SWIM_TWIRL) {
                        ChangeAnimID(IONA_SWIM, 0.0f);
                    }
                }

                if(!mSurfacing) {
                    mHitBox.SetAngle(mSwimAngle + DegreesToRadians(-90));
                }
                else {
                    mHitBox.SetAngle(0);
                }
                if(GetCurrentAnimID() != IONA_SWIM_TWIRL) {
                    float val = mSwimMaxVel * 0.95;
                    val*= val;
                    bool noHori = !mLeftHeld && !mRightHeld;
                    bool noVert = !mUpHeld && !mDownHeld;
                    if(noHori && noVert && mVelocity.Magnitude2() >= val)
                    {
                        ChangeAnimIDSpecial(IONA_SWIM_STOPBRAKE, true);
                    }
                }
            }
            else if((!mSpecialAnim && !mSurfacing && !mDiving)) {
                float speed = 0.1f;
                if(abs(mModel.GetCurrentAnimID()) == IONA_SWIM_STOPBRAKE) {
                    mFlipTimer = 0.0f;
                    if((mRightHeld && mFlipped) ||
                       (mUpHeld && mFlipped) ||
                       (mDownHeld && !mFlipped) ||
                       (mLeftHeld && !mFlipped))
                    {
                        //ChangeAnimIDSpecial(IONA_TURN_SWIM, true, 0.0f);
                        ChangeAnimID(IONA_SWIM, speed);
                    }
                    else {
                        ChangeAnimIDSpecial(IONA_SWIM_BRAKE_RECOV, true, speed);
                    }
                }
                else {
                    if(mModel.GetCurrentAnimID() == IONA_SWIM) {
                       speed = 0.1f;
                    }
                    if(GetCurrentAnimID() == IONA_SWIMSHOOT_IDLE) {
                        if(mFacingLeft) {
                            mSwimAngle = PI;
                            mFlipped = true;
                        }
                    }
                    if(GetCurrentAnimID() == IONA_TURN_SWIM) {
                        TransitionToAnimation(IONA_SWIM_IDLE, 0.05f, 0, 0);
                        mFlipped = !mFlipped;
                        mFlipTimer = 0.0f;
                        if(mSwimDirection.Magnitude2() < 0.1f) {
                            if(mFlipped ) {
                                mSwimAngle = PI;
                            }
                            else {
                                mSwimAngle = 0.0f;
                            }
                        }
                        else {
                            mSwimAngle = atan2(mSwimDirection.y, mSwimDirection.x);
                        }
                    }
                    else {
                        ChangeAnimID(IONA_SWIM_IDLE, speed);
                    }
                }
                mHitBox.SetAngle(mSwimAngle);
            }
        }
        if((GetKeyDown(PKEY_BUTTON1) && abs(GetCurrentAnimID() == IONA_SWIM_STOPBRAKE))
           || GetCurrentAnimID() == IONA_SWIM_IDLE || GetCurrentAnimID() == IONA_SWIM_STOPBRAKE || GetCurrentAnimID() == IONA_SWIM_BRAKE_RECOV
           || GetCurrentAnimID() == IONA_KDRECOV2B || GetCurrentAnimID() == IONA_KDRECOV1B || GetCurrentAnimID() == IONA_KNOCKDOWN1B ||
           GetCurrentAnimID() == IONA_KNOCKDOWN2B) {
            float angleStep = RADIAN * 5;
            if(!mFlipped) {
                if(mSwimAngle > 0.0f) {
                    mSwimAngle-= RADIAN * 5;
                }
                if(mSwimAngle < 0.0f) {
                    mSwimAngle+= RADIAN * 5;
                }
            }
            else if(mSwimAngle != 0) {
                if(mSwimAngle > PI) {
                    mSwimAngle-= RADIAN * 5;
                }
                if(mSwimAngle < PI) {
                    if(mSwimAngle > 0) {
                        mSwimAngle+= RADIAN * 5;
                    }
                    else {
                        mSwimAngle-= RADIAN * 5;
                        if(mSwimAngle < -PI) {
                            mSwimAngle = PI;
                        }
                    }
                }
            }
        }
    }
    void WatershedPlayer::CheckGroundAnims() {
        mHitBox.SetAngle(0);
        mCurrentAnimation.SetAngle(0);
        mLegAnim.SetAngle(0);

        if(!UsesFocusAnims()) {
            if(!mSpecialAnim) {
                if(GetCurrentAnimID() == IONA_MERMAID_SURFACE || GetCurrentAnimID() == IONA_MERMAID_SWIM || GetCurrentAnimID() == IONA_MERMAID_IDLE) {
                    if(mVelocity.y > 0.0f) {
                        ChangeAnimID(ANIM_JUMP);
                    }
                    else {
                        ChangeAnimID(ANIM_FALL);
                    }
                }
                if(mGrounded && mVelocity.x != 0) {
                    float transSpeed = 0.1f;
                    if(GetCurrentAnimID() == ANIM_FALL || GetCurrentAnimID() == IONA_FIN_FALL) {
                        transSpeed = 0.05f;
                    }
                    if(GetCurrentAnimID() == IONA_TURN || GetCurrentAnimID() == IONA_RUN_BRAKE || GetCurrentAnimID() == IONA_SHOOT_BRAKE) {
                        int transFrame = 0;
                        float transSpeed = 0.1f;
                        if(GetCurrentAnimID() == IONA_RUN_BRAKE) {
                            transSpeed = 0.0f;
                        }
                        TransitionToAnimation(ANIM_WALK, transSpeed, 0, 0);
                        mFlipped = !mFlipped;
                    }
                    ChangeAnimID(ANIM_WALK, transSpeed);
                    if(!mLeftHeld && !mRightHeld && fabs(mVelocity.x) >= mWalkSpeed * 0.95f) {
                        ChangeAnimIDSpecial(IONA_IDLE_BRAKE, true, 0.0f);
                    }
                }
                if(!mGrounded) {
                    if(mVelocity.y < -1) {
                        if(!mFins) {
                            if(GetCurrentAnimID() != ANIM_FALL) {
                                if(GetCurrentAnimID() == ANIM_JUMP) {
                                    ChangeAnimID(ANIM_FALL, 0.0);
                                }
                                else {
                                    TransitionToAnimation(ANIM_FALL, 0.1, GetCurrentFrameNum(), 9);
                                }
                            }
                        }
                        else {
                            if(GetCurrentAnimID() != IONA_FIN_FALL) {
                                if(GetCurrentAnimID() == IONA_FIN_JUMP) {
                                    ChangeAnimID(IONA_FIN_FALL, 0.0);
                                }
                                else {
                                    TransitionToAnimation(IONA_FIN_FALL, 0.1, GetCurrentFrameNum(), 9);
                                }
                            }
                        }
                    }
                }
                if(mGrounded && (!mLeftHeld && !mRightHeld)) {
                    float transSpeed = 0.1f;
                    if(GetCurrentAnimID() == IONA_TURN) {
                        transSpeed = 0.0f;
                        mFlipped = !mFlipped;
                    }
                    else if(GetCurrentAnimID() == IONA_HOOK_PULL || GetCurrentAnimID() == IONA_RUN_BRAKE || GetCurrentAnimID() == IONA_SHOOT_BRAKE) {
                        transSpeed = 0.0f;
                    }
                    if(GetCurrentAnimID() == IONA_RUN_BRAKE || GetCurrentAnimID() == IONA_SHOOT_BRAKE) {
                        if(!GetCurrentAnimFinished()) {
                            TransitionToAnimation(ANIM_IDLE, transSpeed, mModel.GetAnimation(mAnimationCache[IONA_RUN_BRAKE])->GetNumFrames() - 1, 0.0f);
                        }
                        else {
                            transSpeed = 0.0f;
                        }
                        if(mFacingLeft) {
                            mFlipped = true;
                        }
                        else {
                            mFlipped = false;
                        }
                    }
                    if(fabs(mVelocity.x) < 0.1f) {
                        ChangeAnimID(ANIM_IDLE, transSpeed);
                    }
                }
            }
            else {
                if(GetCurrentAnimID() == IONA_LAND_FINS && mModel.GetCurrentAnim()->GetCurFrameNum() >= 9) {
                    mSpecialAnim = false;
                    SetFins(false);
                }
            }
        }
    }
    void WatershedPlayer::CheckInput() {
        if(mIgnoreInput || (mSpecialAnim && !mCancelableSpecial)) {
            return;
        }
        mLeftHeld = GetKeyDown(PKEY_LEFT);
        mRightHeld = GetKeyDown(PKEY_RIGHT);
        mUpHeld = GetKeyDown(PKEY_UP);
        mDownHeld = GetKeyDown(PKEY_DOWN);
        if((!UsesFocusAnims() && GetCurrentAnimID() != IONA_SWIM_BRAKE) || (GetCurrentAnimID() == IONA_SWIM_BRAKE && !mVelocity.Magnitude2())) {
            if(mLeftHeld && mVelocity.x <= 0) {
                mFacingLeft = true;
            }
            if(mRightHeld && mVelocity.x >= 0) {
                mFacingLeft = false;
            }
        }
        if(!mSwimming) {
            BasePlayer2::CheckInput();
            if(!GetKeyDown(PKEY_BUTTON1)) {
                if(mFocusTimer >= 0.5f) {
                    mFocusMode = false;
                    mFocusTimer = 0.0f;
                }
            }
            else if(!mSpecialAnim || (mSpecialAnim && mCancelableSpecial)) {
                if(!UsesFocusAnims() || (mFocusAccels && mFocusTimer > 0.0)) {
                    mFocusTimer = 0.0f;
                    if(mLeftHeld) {
                        mFacingLeft = true;
                        mFlipped = true;
                    }
                    if(mRightHeld) {
                        mFacingLeft = false;
                        mFlipped = false;
                    }
                }
                mFocusMode = true;
            }
        }
        else {
            if(GetKeyDown(PKEY_BUTTON1) && ((mSpecialAnim && mCancelableSpecial) || (!mSpecialAnim))) {
                if(mSwimVelocity.Magnitude2() > mFocusModifier * mSwimMaxVel * mSwimMaxVel) {
                    mSkidding = true;
                    mSkidDirection = -mSwimDirection;
                    ChangeAnimIDSpecial(IONA_SWIM_STOPBRAKE, true, 0.025f);
                }
                else {
                    if(!UsesFocusAnims() || (mFocusAccels && mFocusTimer > 0.0)) {
                        mFocusTimer = 0.0f;
                        if(mLeftHeld) {
                            mFacingLeft = true;
                        }
                        if(mRightHeld) {
                            mFacingLeft = false;
                        }
                    }
                    mFocusMode = true;
                    if(mSkidding) {
                        mSkidding = false;
                        mSpecialAnim = false;
                    }
                }
            }
            else {
                if(mFocusTimer >= 0.5f) {
                    if(mSwimming && mFacingLeft) {
                        mSwimAngle = DegreesToRadians(180);
                    }
                    mFocusMode = false;
                    if(mSwimming && mFacingLeft) {
                        mSwimAngle = PI;
                        mFlipped = true;
                    }
                    mFocusTimer = 0.0f;
                }
            }
            if(mVelocity.Magnitude2() <= 1.0f && !mFocusMode && GetCurrentAnimID() != IONA_SWIM_BRAKE && GetCurrentAnimID() != IONA_SWIM_BRAKE) {
                if(mLeftHeld && !mFlipped) {
                    mFlipTimer = 0.0f;
                    ChangeAnimIDSpecial(IONA_TURN_SWIM, true, 0.0f);
                }
                else if(mRightHeld && mFlipped ) {
                    mFlipTimer = 0.0f;
                    ChangeAnimIDSpecial(IONA_TURN_SWIM, true, 0.0f);
                }
            }
        }
    }
    void WatershedPlayer::SetFins(bool fins) {
        if(mFins != fins && !mUsesModel) {
            size_t frame = mLegAnim.GetCurFrameNum();
            string anim = mLegAnim.GetRef();
            if(fins) {
                for(int i = 0; i < mFinAnims.size(); i++) {
                    if(mFinAnims[i].GetRef() == anim) {
                        mLegAnim = mFinAnims[i];
                        mLegAnim.Start();
                        mLegAnim.ForceFrame(frame);
                        if(mSwimming) {
                            mLegAnim.SetVertFlip(mFlipped);
                        }
                        else {
                            mLegAnim.SetHoriFlip(mFlipped);
                        }
                        break;
                    }
                }
            }
            else {
                for(int i = 0; i < mLegAnims.size(); i++) {
                    if(mLegAnims[i].GetRef() == anim) {
                        mLegAnim = mLegAnims[i];
                        mLegAnim.Start();
                        if(mSwimming) {
                            mLegAnim.SetVertFlip(mFlipped);
                        }
                        else {
                            mLegAnim.SetHoriFlip(mFlipped);
                        }
                        mLegAnim.ForceFrame(frame);
                        break;
                    }
                }
            }
        }
        mFins = fins;
    }
    void WatershedPlayer::SetHeldObject(RigidBody* heldObject, float maxLength) {
        mHeldObject = heldObject;
        mHoldLength = maxLength;
    }
    void WatershedPlayer::DropHeldObject(bool brkAnim) {
        if(mHeldObject) {
            mHeldObject->Stop();
            mHeldObject->SetNoPlatform(false);
            if(brkAnim) {
                if(!mUnderwater) {
                    ChangeAnimIDSpecial(IONA_HOOK_BREAK, false, 0.0f);
                }
                else {
                    ChangeAnimIDSpecial(IONA_HOOK_BREAK_FINS, false, 0.0f);
                }
            }
        }
        ReleaseHeldObject();
    }
    void WatershedPlayer::GrabObject(RigidBody* obj) {
        mHeldObject = obj;
        mHeldObject->SetAngle(0.0f);
        mHeldObject->KillRotVel();
        mHeldObject->SetNoPlatform(true);
        mHeldObject->Stop();
        mHeldObject->SetInvMass(-fabs(mHeldObject->GetInvMass()));
        mLassoChain.Attach(obj);
    }
    void WatershedPlayer::ThrowObject() {
        if(mHeldObject && mObjectPulled) {
            mHeldObject->Stop();
            if(!mFlipped) {
                mHeldObject->ApplyImpulse(mThrowStrength + mVelocity);
                mLassoChain.GetBack().ApplyImpulse(mThrowStrength * 4.0f);
                mHeldObject->ApplyRotImpulse(RADIAN * 8);
            }
            else {
                Vector2f thr = mThrowStrength;
                thr.x = -thr.x;
                mHeldObject->ApplyImpulse(thr + mVelocity);
                mLassoChain.GetBack().ApplyImpulse(thr * 4.0f);
                mHeldObject->ApplyRotImpulse(-RADIAN * 8);
            }
            mSpecialAnim = true;
            mShootResetTimer = 10;
            ReleaseHeldObject();
        }
    }
    void WatershedPlayer::ReleaseHeldObject() {
        mHeldObject->SetInvMass(fabs(mHeldObject->GetInvMass()));
        mHeldObject->GetLastPosition() = mHeldObject->GetPosition() - mVelocity;
        mHeldObject->SetZOffset(0.0f);
        mHeldObject->SetNoPlatform(false);
        mObjectPulled = false;
        mPrevHeldObject = mHeldObject;
        mHeldObject = NULL;
        mLassoChain.Attach(NULL);
        mReleasedObjFrames = 0;
        mGrabObjDelay = 0;
    }
    void WatershedPlayer::UpdateHeldObject() {
        if(mHeldObject) {
            if(GetCurrentAnimID() == IONA_TURN || GetCurrentAnimID() == IONA_RUN_BRAKE || GetCurrentAnimID() == IONA_SHOOT_BRAKE) {
                if(mFlipped) {
                    mAnchorPoint.x+= 15;
                    float x = mHitBox.GetX();
                    if(mAnchorPoint.x < 20.0 ) {
                        mAnchorZ+= 0.1f;
                    }
                    else {
                        mAnchorZ-= 0.1f;
                    }
                }
                else {
                    mAnchorPoint.x-= 15;
                    float x = mHitBox.GetX();
                    if(mAnchorPoint.x > -20.0) {
                        mAnchorZ+= 0.1f;
                    }
                    else {
                        mAnchorZ-= 0.1f;
                    }
                }
            }
            else {
                mAnchorPoint = mHoldPoint;
                if(mFlipped) {
                    mAnchorPoint.x = -mHoldPoint.x;
                }
                mAnchorZ = 0.0f;
            }
            Vector2f move = (mHitBox.GetCenter() + mAnchorPoint) - mLassoChain.GetAnchorPoint();
            mLassoChain.SetAnchorPoint(mHitBox.GetCenter() + mAnchorPoint);
            mHeldObject->SetZOffset(mAnchorZ);
        }
        if(mPrevHeldObject) {
            mReleasedObjFrames++;
            if(mReleasedObjFrames >= mReleasedObjDelay) {
                mPrevHeldObject = NULL;
            }
        }
    }
    void WatershedPlayer::AdjustAnimations() {
        if(mFins) {
            if(!mHeldObject && !mObjectPulled) {
                switch(GetCurrentAnimID()) {
                    case IONA_HOOK:
                        ForceAnimID(IONA_HOOK_FINS, GetCurrentFrameNum());
                        break;
                    case IONA_HOOK_BREAK:
                        ForceAnimID(IONA_HOOK_BREAK_FINS, GetCurrentFrameNum());
                        break;
                    case IONA_HOOK_PULL:
                        ForceAnimID(IONA_HOOK_PULL_FINS, GetCurrentFrameNum());
                        break;
                    default:
                        break;
                }
            }
        }
        if((mGrounded && GetCurrentAnimID() == IONA_SHOOT_JUMP) || GetCurrentAnimID() == IONA_RUNSHOOT_FORWARD && fabs(mVelocity.x) <= 0.1) {
            ChangeAnimID(IONA_SHOOT_GROUND, 0.01f);
        }
        if(mSwimming && mSwimVelocity.Magnitude2() == 0.0 && GetCurrentAnimID() == IONA_SWIMSHOOT_FORWARD) {
            ChangeAnimID(IONA_SWIMSHOOT_IDLE, 0.01f);
        }
    }
    void WatershedPlayer::ChangeAnimIDSpecial(int i, bool cancelable, float transitionDur) {
        if(!mSpecialAnim || (mSpecialAnim && mCancelableSpecial)) {
            ChangeAnimID(i, transitionDur);
            mSpecialAnim = true;
            mCancelableSpecial = cancelable;
			if(!mCancelableSpecial) {
				mLeftHeld = false;
				mRightHeld = false;
				mUpHeld = false;
				mDownHeld = false;
			}
        }
    }
    void WatershedPlayer::ChangeAnimID(int i, float transitionDur) {
        int forceFrame = -1;
        if(mSpecialAnim && !mCancelableSpecial) {
            return;
        }
        if(mFins) {
            switch(i) {
            case ANIM_JUMP:
                if(GetCurrentAnimID() == ANIM_JUMP) {
                    transitionDur = 0.0f;
                    forceFrame = GetCurrentFrameNum();
                }
                break;
            case ANIM_FALL:
                i = IONA_FIN_FALL;
                if(GetCurrentAnimID() == ANIM_FALL) {
                    transitionDur = 0.0f;
                    forceFrame = GetCurrentFrameNum();
                }
                break;
            case IONA_SHOOT_JUMP:
                i = IONA_SHOOT_SWIM_JUMP;
                if(GetCurrentAnimID() == IONA_SHOOT_JUMP) {
                    transitionDur = 0.0f;
                    forceFrame = GetCurrentFrameNum();
                }
                break;
            case IONA_SHOOT_BACK_JUMP:
                i = IONA_SHOOT_SWIM_JUMP_BACK;
                if(GetCurrentAnimID() == IONA_SHOOT_BACK_JUMP) {
                    transitionDur = 0.0f;
                    forceFrame = GetCurrentFrameNum();
                }
                break;
            default:
                break;
            }
        }

        if(i == abs(mModel.GetCurrentAnimID())) {
            return;
        }
        if(i == IONA_TURN || i == IONA_TURN_SWIM || i == IONA_RUN_BRAKE || i == IONA_SHOOT_BRAKE) {
            mShellModel.ChangeAnimation(1, 0.0);
        }
        if(transitionDur < 0.001f) {
            mRecoiling = false;
        }
        if(i < mAnimationCache.size()) {
            ChangeAnimation(mAnimationCache[i], transitionDur);
            printf("%s (%f)\n", mModel.GetCurrentAnim()->GetName().c_str(), transitionDur);
            if(forceFrame > -1) {
                mModel.GetCurrentAnim()->ForceFrame(forceFrame);
            }
        }
    }
    void WatershedPlayer::ChangeAnimation(int i, float transitionDur) {
        if(i < 0) {
            return;
        }
        if(mUsesModel) {
            mModel.ChangeAnimation(i, transitionDur);
        }
        else {
            mCurrentAnimation = mAnimations[i];
            if(mFins) {
                mLegAnim = mFinAnims[i];
            }
            else {
                mLegAnim = mLegAnims[i];
            }
            mCurrentAnimation.Start();
            mLegAnim.Start();
        }
    }
    void WatershedPlayer::ChangeAnimation(string anim) {
        if(mUsesModel) {
            mModel.ChangeAnimation(anim);
        }
        else {
            for(int i = 0; i < mAnimations.size(); i++) {
                if(mAnimations[i].GetRef() == anim && mCurrentAnimation.GetRef() != anim) {
                    mCurrentAnimation = mAnimations[i];
                    mCurrentAnimation.Start();
                    break;
                }
            }
            if(mFins) {
                for(int i = 0; i < mFinAnims.size(); i++) {
                    if(mFinAnims[i].GetRef() == anim && mLegAnim.GetRef() != anim) {
                        mLegAnim = mFinAnims[i];
                        mLegAnim.Start();
                        break;
                    }
                }
            }
            else {
                for(int i = 0; i < mLegAnims.size(); i++) {
                    if(mLegAnims[i].GetRef() == anim && mLegAnim.GetRef() != anim) {
                        mLegAnim = mLegAnims[i];
                        mLegAnim.Start();
                        break;
                    }
                }
            }
        }
    }
    void WatershedPlayer::SetupModels() {
        mModel.SetupModel();
        mHoloRod.SetupModel();
        mHairband.SetupModel();
        mShellModel.SetupModel();
        mUsesModel = true;
        GenerateAnimCache();
    }
    bool WatershedPlayer::IsShooting() {
        return GetCurrentAnimID() >= IONA_SHOOT_GROUND && GetCurrentAnimID() <= IONA_SHOOT_SWIM_JUMP_BACK;
    }
    bool WatershedPlayer::UsesFocusAnims() {
        return mFocusMode || (mHeldObject && mObjectPulled) || mShootResetTimer;
    }
    void WatershedPlayer::GenerateAnimCache() {
        mAnimationCache.clear();
        mAnimationCache.resize(IONA_ANIMS_END);
        int i = 0;
        mAnimationCache[i++] = mModel.SetAnimID("BasePose", 0);
        mAnimationCache[i++] = mModel.SetAnimID("IdleA", ANIM_IDLE);
        mAnimationCache[i++] = mModel.SetAnimID("RunA", ANIM_WALK);
        mAnimationCache[i++] = mModel.SetAnimID("JumpA", ANIM_JUMP);
        mAnimationCache[i++] = mModel.SetAnimID("FallA", ANIM_FALL);
        mAnimationCache[i++] = mModel.SetAnimID("FallLandA", ANIM_LAND);

        mAnimationCache[i++] = mModel.SetAnimID("IdleB", IONA_SWIM_IDLE);
        mAnimationCache[i++] = mModel.SetAnimID("SwimB", IONA_SWIM);
        mAnimationCache[i++] = mModel.SetAnimID("SwimTwirlB", IONA_SWIM_TWIRL);
        mAnimationCache[i++] = mModel.SetAnimID("JumpB", IONA_FIN_JUMP);
        mAnimationCache[i++] = mModel.SetAnimID("FallB", IONA_FIN_FALL);
        mAnimationCache[i++] = mModel.SetAnimID("FallLandB", IONA_LAND_FINS);

        mAnimationCache[i++] = mModel.SetAnimID("MmaidDiveB", IONA_MERMAID_DIVE);
        mAnimationCache[i++] = mModel.SetAnimID("MmaidIdleB", IONA_MERMAID_IDLE);
        mAnimationCache[i++] = mModel.SetAnimID("MmaidSurfaceB", IONA_MERMAID_SURFACE);
        mAnimationCache[i++] = mModel.SetAnimID("MmaidSwimB", IONA_MERMAID_SWIM);

        mAnimationCache[i++] = mModel.SetAnimID("FShootA", IONA_SHOOT_GROUND);
        mAnimationCache[i++] = mModel.SetAnimID("FShootFJumpA", IONA_SHOOT_JUMP);
        mAnimationCache[i++] = mModel.SetAnimID("FShootRJumpA", IONA_SHOOT_BACK_JUMP);
        mAnimationCache[i++] = mModel.SetAnimID("DShootA", IONA_SHOOT_DOWN);
        mAnimationCache[i++] = mModel.SetAnimID("UShootA", IONA_SHOOT_UP);
        mAnimationCache[i++] = mModel.SetAnimID("FShootFRunA", IONA_RUNSHOOT_FORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("FShootRRunA", IONA_RUNSHOOT_BACK);
        mAnimationCache[i++] = mModel.SetAnimID("UShootFRunA", IONA_RUNSHOOT_UFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("DShootFRunA", IONA_RUNSHOOT_DFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("", IONA_RUNSHOOT_UBACK);
        mAnimationCache[i++] = mModel.SetAnimID("", IONA_RUNSHOOT_DBACK);

        mAnimationCache[i++] = mModel.SetAnimID("FShootB", IONA_SWIMSHOOT_IDLE);
        mAnimationCache[i++] = mModel.SetAnimID("FShootFSwimB", IONA_SWIMSHOOT_FORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("FShootRSwimB", IONA_SWIMSHOOT_BACK);
        mAnimationCache[i++] = mModel.SetAnimID("FShootUSwimB", IONA_SWIMSHOOT_UP);
        mAnimationCache[i++] = mModel.SetAnimID("FShootDSwimB", IONA_SWIMSHOOT_DOWN);

        mAnimationCache[i++] = mModel.SetAnimID("UShootUSwimB", IONA_SWIMSHOOT_UFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("DShootDSwimB", IONA_SWIMSHOOT_DFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("", IONA_SWIMSHOOT_UBACK);
        mAnimationCache[i++] = mModel.SetAnimID("", IONA_SWIMSHOOT_DBACK);

        mAnimationCache[i++] = mModel.SetAnimID("FShootFJumpB", IONA_SHOOT_SWIM_JUMP);
        mAnimationCache[i++] = mModel.SetAnimID("FShootRJumpB", IONA_SHOOT_SWIM_JUMP_BACK);
        mAnimationCache[i++] = mModel.SetAnimID("", IONA_SHOOT_RECOILA);
        mAnimationCache[i++] = mModel.SetAnimID("", IONA_SHOOT_RECOILB);

        mAnimationCache[i++] = mModel.SetAnimID("SwimTurnB", IONA_SWIM_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("SwimBrakeRecB", IONA_SWIM_BRAKE_RECOV);
        mAnimationCache[i++] = mModel.SetAnimID("DShootFRunTurnA", IONA_DSHOOT_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("UShootFRunTurnA", IONA_USHOOT_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("RunTurnA", IONA_RUN_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("RunBrakeA", IONA_IDLE_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("FShootFRunTurnA", IONA_SHOOT_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("IdleTurnA", IONA_TURN);
        mAnimationCache[i++] = mModel.SetAnimID("IdleTurnB", IONA_TURN_SWIM);
        mAnimationCache[i++] = mModel.SetAnimID("MMaidTurnB", IONA_TURN_MERMAID);
        mAnimationCache[i++] = mModel.SetAnimID("SwimBrakeB", IONA_SWIM_STOPBRAKE);

        mAnimationCache[i++] = mModel.SetAnimID("WHookA", IONA_HOOK);
        mAnimationCache[i++] = mModel.SetAnimID("WHookBreakA", IONA_HOOK_BREAK);
        mAnimationCache[i++] = mModel.SetAnimID("WHookPullA", IONA_HOOK_PULL);
        mAnimationCache[i++] = mModel.SetAnimID("WHookB", IONA_HOOK_FINS);
        mAnimationCache[i++] = mModel.SetAnimID("WHookBreakB", IONA_HOOK_BREAK_FINS);
        mAnimationCache[i++] = mModel.SetAnimID("WHookPullB", IONA_HOOK_PULL_FINS);

        mAnimationCache[i++] = mModel.SetAnimID("Hitstun1A", IONA_HITSTUN1A);
        mAnimationCache[i++] = mModel.SetAnimID("Hitstun1B", IONA_HITSTUN1B);
        mAnimationCache[i++] = mModel.SetAnimID("Hitstun2A", IONA_HITSTUN1A);
        mAnimationCache[i++] = mModel.SetAnimID("Hitstun2B", IONA_HITSTUN1B);
        mAnimationCache[i++] = mModel.SetAnimID("KnockbackA", IONA_KNOCKBACKA);
        mAnimationCache[i++] = mModel.SetAnimID("KnockbackB", IONA_KNOCKBACKA);
        mAnimationCache[i++] = mModel.SetAnimID("Knockdown1A", IONA_KNOCKDOWN1A);
        mAnimationCache[i++] = mModel.SetAnimID("Knockdown2A", IONA_KNOCKDOWN2A);
        mAnimationCache[i++] = mModel.SetAnimID("Knockdown1B", IONA_KNOCKDOWN1B);
        mAnimationCache[i++] = mModel.SetAnimID("Knockdown2B", IONA_KNOCKDOWN2B);

        mAnimationCache[i++] = mModel.SetAnimID("Getup1A", IONA_KDRECOV1A);
        mAnimationCache[i++] = mModel.SetAnimID("Getup2A", IONA_KDRECOV2A);
        mAnimationCache[i++] = mModel.SetAnimID("Getup1B", IONA_KDRECOV1B);
        mAnimationCache[i++] = mModel.SetAnimID("Getup2B", IONA_KDRECOV2B);

        mHandAnchorID = mModel.GetBoneID("Arm_Anchor-l");
        mHeadAnchorID = mModel.GetBoneID("Head_Head");
        for(auto& anim : mShellModel.GetAnimations()) {
            anim.SetLoops(0);
        }
        mShellModel.AddAnimation(*mShellModel.GetAnimation(1));
        mShellModel.GetAnimations().back().Reverse();
        mShellModel.GetAnimations().back().SetID(mShellModel.GetAnimations().size() - 1);
    }
    glm::vec3 WatershedPlayer::GetWorldPos(float xScale, float yScale) {
        return glm::vec3((GetPosition().x + mModelOffset.x) * xScale,
                         (GetPosition().y + mModelOffset.y) * yScale,
                         mModel.GetTransform().translation.z);
    }
    Rectanglef WatershedPlayer::GetGrabRect() {
        Vector2f pos;
        pos.y = mHitBox.GetCenter().y  - mHitBox.GetHeight() * 0.5f + 15;
        if(mFlipped) {
            pos.x = mHitBox.GetLeft() - mGrabRange + 5;
        }
        else {
            pos.x = mHitBox.GetRight() + 5;
        }
        Rectanglef r(pos, mHitBox.GetHeight() - 30, mGrabRange);
        return r;
    }

}
