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
        mActionKeys.push_back(ActionKey(SDLK_d, PKEY_BUTTON2, SDL_CONTROLLER_BUTTON_L2));
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
        mHeadAnchorID = -1;
        mHandAnchorID = -1;
        mShellFlipped = false;
        mHurlAngle = 0.0f;
        mActiveFlags = 0;
        mHurling = false;
        mLightBrightness = 0.0f;
        mLightBrightStep = 0.05f;
        mShotLightColor = glm::vec3(0.0, 0.5, 1.0);
        mCharging = false;
        mChargeSpeed = 0.1;
        mCurrentCharge = 0.0;
        mChargeTimeMax = 1.5;
        mChargeTimer = 0;
        mRollSpeed = 10.0;
        mCastTimer = 0.0;
        mChargeVelocityMax = 12.5;
        mEquippedSpellType = -1;
        mBreached = false;
        mBreachStrength = 15.0;
        mMaxMeleeCharge = 0.5;
        mMaxCastTime = 1.5;
        mCurrentMeleeCharge = 0.0;
        mMeleeSlashCreated = false;
        mHitBoxHeight = 32;
        mRollHeight = 32;
        mMeleeTargetFound = false;
        mRocketJumpSpeed = 12;
        mRocketJumpDecel = 3.0;
        mSpellTargetRange = 300;

        Animation waterHookAnim, waterHookDestroy;
        waterHookAnim.SetTexHW(2048, 2048);
        float hookSize = 160;
        waterHookAnim.SetClip(hookSize);
        waterHookAnim.AutoAddFramesSheet(5, 256, 256, 2, 0);
        waterHookAnim.SetLoopFrame(4);
        waterHookAnim.SetTextureID(0);

        waterHookDestroy.SetTexHW(2048, 2048);
        waterHookDestroy.SetClip(hookSize);
        waterHookDestroy.AutoAddFramesSheet(5, 256, 256, 3, 0);
        waterHookDestroy.SetLoops(0);
        waterHookDestroy.SetTextureID(0);

        mWaterHook.Destroy();
        mWaterHook.mAnimations.push_back(waterHookAnim);
        mWaterHook.mDestroyAnimations.push_back(waterHookDestroy);
        mWaterHook.SetAcceleration(glm::vec3(1.0, 0.0, 0.0));
        mWaterHook.SetVelocity(glm::vec3(5.0, 15.0, 0.0));
        mWaterHook.SetMaxSpeed(glm::vec3(15.0, 15.0, 0.0));
        mWaterHook.mFlipSprite = false;
        mWaterHook.mRotatesSprite = true;
        mWaterHook.SetFallSpeed(0.5);
        mWaterHook.SetHitboxHW(50, 140);

        float slashSize = 256;
        mMeleeBulletSpawner.mBaseBullets.push_back(MeleeBullet());
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.SetTexHW(2048, 2048);
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.AutoAddFramesSheet(6, 512, 512, 0, 0);

        mMeleeBulletSpawner.mBaseBullets.push_back(MeleeBullet());
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.SetTexHW(2048, 2048);
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.AutoAddFramesSheet(7, 512, 512, 2, 0);

        mMeleeBulletSpawner.mBaseBullets.push_back(MeleeBullet());
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.SetTexHW(2048, 2048);
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.AutoAddFramesSheet(7, 512, 512, 0, 0);

        mMeleeBulletSpawner.mBaseBullets.push_back(MeleeBullet());
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.SetTexHW(2048, 2048);
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.AutoAddFramesSheet(6, 512, 512, 2, 0);

        mMeleeBulletSpawner.mBaseBullets.push_back(MeleeBullet());
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.SetTexHW(2048, 2048);
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.AutoAddFramesSheet(6, 512, 512, 0, 0);

        mMeleeBulletSpawner.mBaseBullets.push_back(MeleeBullet());
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.SetTexHW(2048, 2048);
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation.AutoAddFramesSheet(7, 512, 512, 2, 0);
        mMeleeBulletSpawner.mBaseBullets.back().mMeleeBulletType = MELEEBULLET_FINISHER;

        mMeleeBulletSpawner.mBaseBullets[0].mMeleeAnimation.SetTextureID(2);
        mMeleeBulletSpawner.mBaseBullets[1].mMeleeAnimation.SetTextureID(2);
        mMeleeBulletSpawner.mBaseBullets[2].mMeleeAnimation.SetTextureID(3);
        mMeleeBulletSpawner.mBaseBullets[3].mMeleeAnimation.SetTextureID(3);
        mMeleeBulletSpawner.mBaseBullets[4].mMeleeAnimation.SetTextureID(4);
        mMeleeBulletSpawner.mBaseBullets[5].mMeleeAnimation.SetTextureID(4);
        mMeleeBulletSpawner.mBaseBullets[6].mMeleeAnimation.SetTextureID(6);
        double dt = 1.0 / 60.0;
        for(size_t i = 0; i < mMeleeBulletSpawner.mBaseBullets.size(); i++) {
            mMeleeBulletSpawner.mBaseBullets[i].mMeleeAnimation.Reset();
            mMeleeBulletSpawner.mBaseBullets[i].mMeleeAnimation.Start();
            mMeleeBulletSpawner.mBaseBullets[i].mMeleeAnimation.SetLoops(0);
            mMeleeBulletSpawner.mBaseBullets[i].mMeleeAnimation.SetDuration(dt * 3);
            mMeleeBulletSpawner.mBaseBullets[i].mMeleeAnimation.GetFrame(1).SetDuration(dt * 4.5);
            mMeleeBulletSpawner.mBaseBullets[i].mMeleeAnimation.SetDrawFrom(DRAW_CENTER);
            mMeleeBulletSpawner.mBaseBullets[i].mMeleeAnimation.SetClip(slashSize);
            mMeleeBulletSpawner.mBaseBullets[i].SetHitboxHW(150, 150);
            mMeleeBulletSpawner.mBaseBullets[i].mKnockback = Vector2f(2, 2);
        }
        mMeleeBulletSpawner.mBaseBullets[2].mKnockback = Vector2f(15, 5);

        solstice::FlurryBullet flurry;

        Animation flurryAnimation, flurryAnimation2, flurryAnimation3, flurryAnimation4;
        flurryAnimation.SetTexHW(2048, 2048);
        flurryAnimation2.SetTexHW(2048, 2048);
        flurryAnimation3.SetTexHW(2048, 2048);
        flurryAnimation4.SetTexHW(2048, 2048);
        flurryAnimation.AutoAddFramesSheet(5, 512, 512, 0, 0);
        flurryAnimation2.AutoAddFramesSheet(5, 512, 512, 1, 1);
        flurryAnimation3.AutoAddFramesSheet(5, 512, 512, 2, 2);
        flurryAnimation4.AutoAddFramesSheet(5, 512, 512, 0, 0);

        flurryAnimation.Reset();
        flurryAnimation.Start();
        flurryAnimation.SetLoops(0);
        flurryAnimation.SetDuration(dt * 3);
        flurryAnimation.SetDrawFrom(DRAW_CENTER);
        flurryAnimation.SetClip(slashSize);

        flurryAnimation2.Reset();
        flurryAnimation2.Start();
        flurryAnimation2.SetLoops(0);
        flurryAnimation2.SetDuration(dt * 3);
        flurryAnimation2.SetDrawFrom(DRAW_CENTER);
        flurryAnimation2.SetClip(slashSize);

        flurryAnimation3.Reset();
        flurryAnimation3.Start();
        flurryAnimation3.SetLoops(0);
        flurryAnimation3.SetDuration(dt * 3);
        flurryAnimation3.SetDrawFrom(DRAW_CENTER);
        flurryAnimation3.SetClip(slashSize);

        flurryAnimation4.Reset();
        flurryAnimation4.Start();
        flurryAnimation4.SetLoops(0);
        flurryAnimation4.SetDuration(dt * 3);
        flurryAnimation4.SetDrawFrom(DRAW_CENTER);
        flurryAnimation4.SetClip(slashSize);

        flurryAnimation.SetTextureID(4);
        flurryAnimation2.SetTextureID(4);
        flurryAnimation3.SetTextureID(4);
        flurryAnimation4.SetTextureID(4);

        MeleeBullet flurryFinisher;
        flurryFinisher.mKnockback = Vector2f(15, 150);
        flurryFinisher.SetHitboxHW(512, 512);
        flurryFinisher.mMeleeAnimation.SetTexHW(2048, 2048);
        flurryFinisher.mMeleeAnimation.AutoAddFramesSheet(7, 512, 512, 2, 0);
        flurryFinisher.mMeleeAnimation.SetTextureID(6);
        flurryFinisher.mMeleeAnimation.Reset();
        flurryFinisher.mMeleeAnimation.Start();
        flurryFinisher.mMeleeAnimation.SetLoops(0);
        flurryFinisher.mMeleeAnimation.SetDuration(dt * 3);
        flurryFinisher.mMeleeAnimation.GetFrame(1).SetDuration(dt * 4.5);
        flurryFinisher.mMeleeAnimation.SetDrawFrom(DRAW_CENTER);
        flurryFinisher.mMeleeAnimation.SetClip(slashSize);

        //flurry.mAnimations.push_back(mMeleeBulletSpawner.mBaseBullets.back().mMeleeAnimation);
        flurry.mAnimations.push_back(flurryAnimation);
        flurry.mAnimations.push_back(flurryAnimation2);
        flurry.mAnimations.push_back(flurryAnimation3);
        flurry.mAnimations.push_back(flurryAnimation4);
        flurry.mFinisher = flurryFinisher;
        for(size_t i = 0; i < flurry.mAnimations.size(); i++) {
            flurry.mAnimations[i].SetLoops(0);
            flurry.mAnimations[i].SetDuration(dt * 2);
        }
        mMeleeBulletSpawner.mFinisherBase = flurry;
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
        mHeldObject = NULL;
        mSpecialAnim = false;
        mCancelableSpecial = true;
        mObjectPulled = false;
        mHurling = false;
        mActiveFlags = 0;
        mWaterPistol->Reset();
        mMissile->Reset();
        mLaser->Reset();
        mMagic.Reset();
        mMeleeBulletSpawner.Reset();
        mMeleeFinisher.Reset();
    }
    void WatershedPlayer::Walk() {
        if(mSwimming || mHurling) {
            return;
        }
        float accel = mWalkAccel;
        if(GetCurrentAnimID() == IONA_ROLL_A || GetCurrentAnimID() == IONA_ROLL_B) {
            accel = 0;
        }
        if(mFocusMode && !mFocusAccels) {
            accel = -1.0f;
        }
        Vector2f velocity = GetPosition() - mLastPos;
        float walkSpeed = mWalkSpeed;
        if(mFocusMode) {
            walkSpeed*= mGroundFocusModifier;
        }
        bool leftHeld = mLeftHeld;
        bool rightHeld = mRightHeld;
        if(GetCurrentAnimID() == IONA_CSCHARGE_UP || GetCurrentAnimID() == IONA_CSIMPACT || GetCurrentAnimID() == IONA_CSDASH) {
            leftHeld = false;
            rightHeld = false;
        }
        if(leftHeld) {
            if(accel > -1.0f) {
                if((mGrounded || mAirAccels) && velocity.x > -walkSpeed) {
                    if(mSkidStop) {
                        if(!AnimHasFlag(IONA_AFLAG_BRAKING) || (AnimHasFlag(IONA_AFLAG_BRAKING && mVelocity.x > 0.0))) {
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
            if(!mFocusMode && mGrounded && velocity.x >= walkSpeed * 0.80) {
                Brake();
                mFacingLeft = true;
                mLastPos.x = GetPosition().x - walkSpeed * 0.9f;
            }
        }
        else if(rightHeld) {
            if(accel > -1.0f) {
                if((mGrounded || mAirAccels)  && velocity.x < walkSpeed) {
                    if(mSkidStop) {
                        if(!AnimHasFlag(IONA_AFLAG_BRAKING) || (AnimHasFlag(IONA_AFLAG_BRAKING && mVelocity.x < 0.0))) {
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
            if(!mFocusMode && mGrounded && velocity.x <= -walkSpeed * 0.80) {
                Brake();
                mFacingLeft = false;
                mLastPos.x = GetPosition().x + walkSpeed * 0.90f;
            }
        }
        if((!leftHeld && !rightHeld) && (GetCurrentAnimID() != IONA_HOOK || (GetCurrentAnimID() == IONA_HOOK && mGrounded))) {
            if(accel > 0.0) {
                if(velocity.x > 0) {
                    ApplyForceX(-accel);
                }
                else if(velocity.x < 0) {
                    ApplyForceX(accel);
                }
            }
            else if(GetCurrentAnimID() != IONA_ROLL_A && GetCurrentAnimID() != IONA_ROLL_B) {
                mLastPos.x = GetPosition().x;
            }
        }
    }
    void WatershedPlayer::ApplyImpulse(Vector2f impulse) {
        RigidBody::ApplyImpulse(impulse);
        mSwimForce+= impulse;
    }
    void WatershedPlayer::ApplyImpulseX(float x) {
        RigidBody::ApplyImpulseX(x);
        mSwimForce.x+= x;
    }
    void WatershedPlayer::ApplyImpulseY(float y) {
        RigidBody::ApplyImpulseY(y);
        mSwimForce.y+=y;
    }
    void WatershedPlayer::ApplyForce(Vector2f impulse) {
        mAppliedForce+= impulse;
        mSwimForce+= impulse;
    }
    void WatershedPlayer::ApplyForceX(float x) {
        mAppliedForce.x+=x;
        mSwimForce.x+=x;
    }
    void WatershedPlayer::ApplyForceY(float y) {
        mAppliedForce.y+=y;
        mSwimForce.y+= y;
    }
    void WatershedPlayer::Accelerate() {
        if(mHurling) {
            return;
        }
        if(mSwimming && !mDiving) {
            float maxVel = mSwimMaxVel;
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
            if(GetCurrentAnimID() == IONA_CSCHARGE_UP) {
                mSwimDirection = 0;
            }
            if(!mSkidding) {
                mSwimDirection = mSwimDirection.GetUnit();
                if(mFocusMode || IsShooting()) {
                    mSwimVelocity = mSwimDirection * mFocusModifier * mSwimMaxVel;
                    mSwimVelocity+= mRocketJumpVelocity;
                }
                else {
                    mSwimVelocity+= mRocketJumpVelocity;
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
                    if(mSwimVelocity.x > maxVel) {
                        mSwimVelocity.x = maxVel;
                    }
                    if(mSwimVelocity.x < -maxVel) {
                        mSwimVelocity.x = -maxVel;
                    }
                    if(mSwimVelocity.y > maxVel) {
                        mSwimVelocity.y = maxVel;
                    }
                    if(mSwimVelocity.y < -maxVel) {
                        mSwimVelocity.y = -maxVel;
                    }
                    float swimVelSqr = maxVel * maxVel;
                    Vector2f swimVelUnit = mSwimVelocity.GetUnit();
                    if(mSwimVelocity.Magnitude2() > swimVelSqr) {
                        mSwimVelocity = swimVelUnit * maxVel;
                    }
                    float diff = mSwimDirection.Dot(swimVelUnit);
                    if((diff < -0.8 && mSwimVelocity.Magnitude2() >= swimVelSqr * 0.7f)) {
                        Brake();
                    }
                }
                if(mRocketJumpVelocity.x > 0) {
                    mRocketJumpVelocity.x-= mRocketJumpDecel.x;
                }
                else if(mRocketJumpVelocity.x < 0) {
                    mRocketJumpVelocity.x+= mRocketJumpDecel.x;
                }
                if(mRocketJumpVelocity.y > 0) {
                    mRocketJumpVelocity.y-= mRocketJumpDecel.y;
                }
                else if(mRocketJumpVelocity.y < 0) {
                    mRocketJumpVelocity.y+= mRocketJumpDecel.y;
                }
                if(fabs(mRocketJumpVelocity.x) < mRocketJumpDecel.x) {
                    mRocketJumpVelocity.x = 0;
                }
                if(fabs(mRocketJumpVelocity.y) < mRocketJumpDecel.y) {
                    mRocketJumpVelocity.y = 0;
                }
            }
            else {
                Skid();
            }
            if(mSwimVelocity.Magnitude2() > maxVel * maxVel) {
                mSwimVelocity = mSwimVelocity.GetUnit() * maxVel;
            }
            mSwimVelocity+= mSwimForce;
            //mSwimForce = 0;
            double decel = 0.5;
            if(GetCurrentAnimID() == IONA_CSDASH) {
                decel = 0.25;
            }
            float decelX = std::min(decel, fabs(mSwimForce.x));
            float decelY = std::min(decel, fabs(mSwimForce.y));
            if(mSwimForce.x > 0) {
                mSwimForce.x-= decelX;
            }
            if(mSwimForce.x < 0) {
                mSwimForce.x+= decelX;
            }
            if(mSwimForce.y > 0) {
                mSwimForce.y-= decelY;
            }
            if(mSwimForce.y < 0) {
                mSwimForce.y+= decelY;
            }
            mVelocity = mSwimVelocity;
            mSwimColImpulse = 0;
        }
    }
    void WatershedPlayer::Skid() {
        if(!mSkidding || mCharging) {
            return;
        }
        mFlipTimer = 0;
        if(!UsesFocusAnims()) {
            Vector2f decel = mSwimDecel * mSkidDirection;
            if(mSwimVelocity.x < 0) {
                mSwimVelocity.x = mSwimVelocity.x + mSwimDecel;
            }
            if(mSwimVelocity.x > 0) {
                mSwimVelocity.x = mSwimVelocity.x - mSwimDecel;
            }
            if(mSwimVelocity.y < 0) {
                mSwimVelocity.y = mSwimVelocity.y + mSwimDecel;
            }
            if(mSwimVelocity.y > 0) {
                mSwimVelocity.y = mSwimVelocity.y - mSwimDecel;
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
            if((mSwimming && !mHurling && mVelocity.y >= 1.0f) || GetCurrentAnimID() == IONA_SWIM) {
                mLastPos.y = GetPosition().y;
            }
        }
        else if(!mSwimming) {
            if(mUnderwater && GetCurrentAnimID() != IONA_CSCHARGE_UP) {
                if(mDownHeld && depth < 2.0f) {
                    mDiving = true;
                    mNoFloat = true;
                    mNoGravity = true;
                    mLastPos.y = GetPosition().y + 5.0;
                }
                else if(!mDownHeld) {
                    mDiving = false;
                    if(GetCurrentAnimID() == IONA_MERMAID_DIVE) {
                        mSpecialAnim = false;
                        StopY();
                    }
                }
                if(depth > 2.0 && mDownHeld) {
                    mDiving = false;
                    mSwimming = true;
                }
            }
            if(mFocusMode && mDownHeld) {
                mSwimVelocity.y = -mSwimMaxVel * mFocusModifier;
                mLastPos.y = GetPosition().y + mSwimMaxVel * mFocusModifier;
                mDiving = false;
                mNoFloat = true;
            }
        }
        if(depth >= 1.5f && !mDownHeld) {
            mSwimming = true;
            mNoFloat = true;
            mNoGravity = true;
            mSurfacing = false;
            mDiving = false;
        }
        if(depth >= 2.5) {
            mSurfaced = false;
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
        if(!mUnderwater || mFocusMode) {
            return;
        }
        if(GetCurrentAnimID() == IONA_CSDASH && mTouchingWaterSurface) {
            Breach();
        }
        else {
            float depth = mHitBox.GetHeight() * 1.5;
            if(mSwimming) {
                if(mSubmergeDepth <= depth && mVelocity.Magnitude2() == 0) {
                    mSurfacing = true;
                }
                if(mSubmergeDepth > depth) {
                    mDiving = false;
                }
            }
            else {
                if(mVelocity.y >= -1.0 && mSubmergeDepth > mHitBox.GetHeight() * 0.5) {
                    mSurfacing = true;
                }
                else {
                    if(mSurfacing) {
                        StopY();
                    }
                }
            }
            if(GetKeyDown(PKEY_DOWN) || (mSurfacing && mSubmergeDepth <= mHitBox.GetHeight() * 0.5)) {
                mSurfacing = false;
            }
            if(mSurfacing && mVelocity.y < 3.0) {
                ApplyImpulseY(1.0);
            }
            if(mSubmergeDepth > depth * 2.0f) {
                mSwimming = true;
                mSurfacing = false;
                mDiving = false;
            }
        }
    }
    void WatershedPlayer::Breach() {
        if(!mBreached) {
            Vector2f speed;
            speed.x = cos(mSwimAngle) * 12;
            speed.y = sin(mSwimAngle) * 12;
            ApplyImpulse(speed);
            mBreached = true;
        }
    }
    void WatershedPlayer::CheckFlip() {
        if(!mFocusMode && mShootResetTimer && !AnimHasFlag(IONA_AFLAG_BRAKING)) {
            if(mLeftHeld || (mFacingLeft && !mRightHeld)) {
                mFlipped = true;
            }
            else if(mRightHeld) {
                mFlipped = false;
            }
        }
        else {
            if(!mFocusMode) {
                if(!mSwimming && !AnimHasFlag(IONA_AFLAG_BRAKING) && !AnimHasFlag(IONA_AFLAG_TURNING)) {
                    if(mFacingLeft) {
                        mFlipped = true;
                    }
                    else {
                        mFlipped = false;
                    }
                }
                if(mSwimming && mFlipTimer >= 30 && !mFocusMode) {
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

            if(mSwimming && (fabs(mVelocity.x) > 4 || mVelocity.Magnitude2() <= 0.01)) {
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
        if(!mUnderwater && !mSpecialAnim && !mFocusMode && !mLeftHeld && !mRightHeld) {
            if(mFacingLeft) {
                mFlipped = true;
            }
            else {
                mFlipped = false;
            }
        }
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
        if(mShellModel.GetCurrentAnim() && mShellModel.GetCurrentAnim()->Finished()) {
            mShellModel.ChangeAnimation(0, 0.0);
        }
    }
    void WatershedPlayer::Update(double t, const std::vector<RigidBody*>& grabbableObjects) {
        Update(t);
        mWaterHook.Update(t);
        CheckGrabbleObjects(grabbableObjects);
        FindSpellTarget(grabbableObjects);
        if(mSwimming) {
            float angle = mSwimAngle;
            mSwimHitBox.SetPosition(mHitBox.GetCenter());
        }
    }
    void WatershedPlayer::Update(double t) {
        mStep = t;
        mVelocity = GetPosition() - mLastPos;
        mGroundedCheck++;
        mFocusTimer+= mStep;
        if(IsChargingMagic() && mCastTimer < mMaxCastTime) {
            mCastTimer+= t;
        }
        if(!GetKeyDown(PKEY_BUTTON1)) {
            if(mFocusTimer >= 0.5f) {
                if(mFocusMode && mSwimming && mFacingLeft) {
                    mSwimAngle = PI;
                    mFlipped = true;
                }
                SetFocus(false);
            }
        }
        if(mFocusMode) {
            mFlipTimer = 0;
        }
        if(mActionKeys[PKEY_SHOOT].keyState == KEY_DOWNCHECKED && mEquippedSpellType == SPELL_MELEE) {
            mCurrentMeleeCharge+= t;
        }
        if(mCurrentMeleeCharge > mMaxMeleeCharge) {
            mCurrentMeleeCharge = mMaxMeleeCharge;
        }
        if(mEquippedSpellType != SPELL_MELEE && !GetKeyDown(PKEY_SHOOT)) {
            mCurrentMeleeCharge = 0.0;
        }
        if(GetCurrentAnimID() != IONA_LAND_FINS || (GetCurrentAnimID() == IONA_LAND_FINS && GetCurrentFrameNum() >= 9)) {
            CheckInput();
        }
        CheckJump();
        CheckPlatform();
        Accelerate();
        Dive();
        Walk();
        float fallSpeed = mGravityAccel;
        if(GetCurrentAnim().HasFlag(IONA_AFLAG_MELEE) && !mUnderwater) {
            fallSpeed *= 0.65;
        }
        Fall(fallSpeed);
        if(mGrounded && fabs(mVelocity.x) > 3.0) {
            mLastPos.y-= mSlopeForce;
        }
        Float();
        Surface();
        AdjustSwimVel();
        CheckFlip();
        UpdateShootMode();
        Attack();
        CheckFins();
        Charge(t);
        CheckAnimations();
        if((GetCurrentAnimID() == IONA_SEASICKLE_AIR || GetCurrentAnimID() == IONA_SEASICKLE_AIR_FINS) && mMeleeSlashCreated && GetCurrentFrameNum() == 7) {
            mMeleeSlashCreated = false;
        }
        Recover();
        VerletNoVelocity(t);
        mSubmergeDepth = 0;
        ClearColliders();
        UpdateHeldObject();
        UpdateRod(t);
        UpdateBullets(t);
        mSlopeForce = 0.0f;
        mHurlHitbox.SetPosition(mHitBox.GetPosition());
        if(!mUnderwater) {
            ChangeState(MOVE_GROUNDED);
            mBreached = false;
        }
        else if(!mSwimming) {
            ChangeState(MOVE_SWIMMING_SURFACE);
        }
        else {
            ChangeState(MOVE_SWIMMING_UNDERWATER);
        }
        if(GetCurrentAnim().HasFlag(IONA_AFLAG_ROLLING) && mHitBox.GetHeight() > mRollHeight) {
            mHitBox.SetHeight(mRollHeight);
        }
        else if(mHitBox.GetHeight() < mHitBoxHeight){
            mHitBox.SetHeight(mHitBoxHeight);
        }
        if(!IsChargingMagic()) {
            mCastTimer = 0;
        }
        mTouchingWaterSurface = false;

    }
    void WatershedPlayer::Update(std::vector<SDL_Event>& events, float tstep) {
        mVelocity = GetPosition() - mLastPos;
        mStep = tstep;
        UpdateKeys(events);
        Update(tstep);
    }
    void WatershedPlayer::UpdateBullets(double dt) {
        if(mWaterPistol) {
            mWaterPistol->Update(dt);
        }
        if(mLaser) {
            Vector2f shotOrigin = mShotOrigin;
            if(mAimDirection.x) {
                if(mFlipped) {
                    shotOrigin.x-= 50;
                }
                else {
                    shotOrigin.x+= 50;
                }
            }
            else if(mAimDirection.y > 0) {
                shotOrigin.y+= 50;
            }
            else if(mAimDirection.y < 0) {
                shotOrigin.y-= 50;
            }
            mLaser->SetPositions(shotOrigin);
            if(mEquippedSpellType != SPELL_LASER || !IsShooting()) {
                mLaser->Kill();
            }
            else if(mEquippedSpellType == SPELL_LASER) {
                mLaser->Start();
            }
            mLaser->AngleBullets(atan2(mAimDirection.y, mAimDirection.x));
            mLaser->Update(dt);
        }
        if(mMissile) {
            mMissile->Update(dt);
            auto bullets = mMissile->GetBullets();
            for(auto it = bullets.begin(); it != bullets.end(); it++) {
                Missile* missile = static_cast<Missile*>(it->get());
                Circlef blastCircle = missile->GetBlastCircle();
                if(missile->Exploding() && solstice::CheckCollision(&this->GetHitBox(), &blastCircle)) {
                    Vector2f force;
                    force.y = mRocketJumpSpeed;
                    if(mSwimming) {
                        Stop();
                        if(missile->GetVelocity().y > 0 || mSwimHitBox.GetCenter().y < missile->GetBlastCircle().GetCenter().y - 15) {
                            force.y = -mRocketJumpSpeed;
                        }
                        else if(missile->GetVelocity().y < 0) {
                            force.y = mRocketJumpSpeed;
                        }
                        if(missile->GetVelocity().x > 0) {
                            force.y*= 0.75;
                            force.x = -mRocketJumpSpeed;
                        }
                        else if(missile->GetVelocity().x < 0) {
                            force.y*= 0.75;
                            force.x = mRocketJumpSpeed;
                        }
                        ApplyImpulse(force * 0.4f);
                    }
                    else {
                        StopY();
                        if(missile->GetVelocity().y > 0) {
                            force.y = -mRocketJumpSpeed;
                        }
                        else if(missile->GetVelocity().y < 0) {
                            force.y = mRocketJumpSpeed;
                        }
                        if(missile->GetVelocity().x > 0) {
                            force.y*= 0.75;
                            force.x = -mRocketJumpSpeed;
                        }
                        else if(missile->GetVelocity().x < 0) {
                            force.y*= 0.75;
                            force.x = mRocketJumpSpeed;
                        }
                        force.y*= 2.0;
                        ApplyImpulse(force);
                    }
                    mShootResetTimer = 0;
                }
            }
        }
        if(mFoamThrower) {
            if(mEquippedSpellType != SPELL_THROWER || !IsShooting()) {
                mFoamThrower->Kill();
            }
            mFoamThrower->Update(dt);
        }
        mMeleeBulletSpawner.Update(dt);
        mMagic.Update(dt);
        if(mMeleeBulletSpawner.GetFinisherCollision() && !mMeleeBulletSpawner.HasFinisher()) {
            mMeleeBulletSpawner.DoFinisher();
            mCancelableSpecial = true;
            ChangeAnimIDSpecial(IONA_SPELL_A, false);
        }
        if(mMeleeBulletSpawner.HasFinisher() && (mActionKeys[PKEY_SHOOT].keyState == KEY_DOWN || mActionKeys[PKEY_SHOOT].keyState == KEY_DOWNCHECKED ))
        {
            mMeleeBulletSpawner.ResetFinisherReps();
        }
        if(mMeleeBulletSpawner.GetFinisherReps() > 10 && GetCurrentAnimID() == IONA_SPELL_A) {
            mSpecialAnim = false;
            mCancelableSpecial = false;
            ChangeAnimIDSpecial(IONA_SPELL_A2, false);
        }
        if((GetCurrentAnimID() == IONA_SPELL_A2 || GetCurrentAnimID() == IONA_SPELL_B2) && GetCurrentFrameNum() >= 10) {
            if(mMeleeBulletSpawner.HasFinisher()) {
                mMeleeBulletSpawner.EndFinisher();
            }
        }
    }
    void WatershedPlayer::FindSpellTarget(const std::vector<RigidBody*>& grabbableObjects) {
        if(mEquippedSpellType == SPELL_MAGIC || mEquippedSpellType == SPELL_MELEE) {
            double closest = mSpellTargetRange * mSpellTargetRange + 30;
            if(mEquippedSpellType == SPELL_MELEE) {
                closest*= 0.5;
            }
            mMagicTarget = mHitBox.GetPosition() + Vector2f(100.0f, 0);
            for(size_t i = 0; i < grabbableObjects.size(); i++) {
                if((mFlipped && grabbableObjects[i]->GetPosition().x < mHitBox.GetCenter().y) ||
                   (!mFlipped && grabbableObjects[i]->GetPosition().x > mHitBox.GetCenter().y))
                {
                    double dist = grabbableObjects[i]->GetPosition().Distance(mHitBox.GetCenter());
                    if(dist < closest) {
                        closest = dist;
                        mSpellTarget = grabbableObjects[i];
                        mMagicTarget = mSpellTarget->GetHitBox().GetCenter();
                    }
                }
            }
        }
        if(mEquippedSpellType == SPELL_MAGIC) {
            mMagicTarget.y+= 150;
        }
    }
    void WatershedPlayer::CheckGrabbleObjects(const std::vector<RigidBody*>& grabbableObjects) {
        if(GetKeyDown(PKEY_ATTACK)) {
            if(mHeldObject && (GetCurrentAnimID() < IONA_HOOK || GetCurrentAnimID() > IONA_HOOK_PULL_FINS)) {
                if(mDownHeld) {
                    DropHeldObject();
                }
                else {
                    if(mVelocity.x) {
                        if(mUpHeld) {
                            mModel.SwapBoneGroup(mAnimationCache[IONA_SHOOT_RECOIL_UP], 0);
                        }
                        else if(mDownHeld) {
                            mModel.SwapBoneGroup(mAnimationCache[IONA_SHOOT_RECOIL_DOWN], 0);
                        }
                        else {
                            mModel.SwapBoneGroup(mAnimationCache[IONA_SHOOT_RECOIL], 0);
                        }
                        mRecoiling = true;
                    }
                    else {
                        if(mUpHeld) {
                            ChangeAnimIDSpecial(IONA_SHOOT_RECOIL_UP, true, 0.0f);
                        }
                        else if(mDownHeld) {
                            ChangeAnimIDSpecial(IONA_SHOOT_RECOIL_DOWN, true, 0.0f);
                        }
                        else {
                            ChangeAnimIDSpecial(IONA_SHOOT_RECOIL, true, 0.0f);
                        }
                        mRecoiling = true;
                    }
                    ThrowObject();
                }
            }
            else {
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
                if(mFlipped) {
                    mSwimAngle = PI;
                }
                else {
                    mSwimAngle = 0;
                }
            }
            mActionKeys[PKEY_ATTACK].keyState = KEY_DOWNCHECKED;
            mWaterHook.SetPosition(mHitBox.GetPosition());
        }
        if(GetCurrentAnimID() == IONA_HOOK || GetCurrentAnimID() == IONA_HOOK_FINS) {
            if(GetCurrentFrameNum() == 14 && !mWaterHook.GetAlive()) {
                Vector2f aimDirection;
                Vector2f shotOrigin;
                shotOrigin.y = 80;
                glm::vec3 accel = mWaterHook.GetAcceleration();
                if(mFlipped) {
                    aimDirection.x = -3;
                    shotOrigin.x-= 120;
                    accel.x = -fabs(accel.x);
                }
                else {
                    aimDirection.x = 3;
                    shotOrigin.x+= 120;
                    accel.x = fabs(accel.x);
                }
                if(GetKeyDown(PKEY_DOWN)) {
                    aimDirection.y = 0.0;
                    mWaterHook.SetFallSpeed(0.0);
                }
                else {
                    aimDirection.y = 8.0;
                    mWaterHook.SetFallSpeed(0.5);
                }
                mWaterHook.Update(0);
                mWaterHook.Shoot(aimDirection, shotOrigin + mHitBox.GetPosition());
                mWaterHook.SetAcceleration(accel);
            }
        }
        if(!mHeldObject && mWaterHook.GetAlive()) {
            Vector2f center = GetHitBox().GetCenter();
            RigidBody* closestBody = NULL;
            float closestDistance = FLT_MAX;
            for(auto& body : grabbableObjects) {
                Rectanglef grabBox = mWaterHook.GetHitBox();
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
        if(mWaterHook.GetAlive()) {
            float dist = 650 * 650;
            if(GetCurrentAnimID() == IONA_HOOK_BREAK || GetCurrentAnimID() == IONA_HOOK_BREAK_FINS || (mWaterHook.GetHitBoxPos().Distance(mHitBox.GetCenter()) > dist)) {
                mWaterHook.Destroy();
            }
        }
    }
    void WatershedPlayer::CheckFins() {
        float subDepth = GetSubmergeRatio();
        if(!mHurling && ((mUnderwater && subDepth >= 0.3) || mSwimming)) {
            SetFins(true);
        }
        if(!mHurling && !mUnderwater && mGrounded && GetCurrentAnimID() != IONA_LAND_FINS) {
            SetFins(false);
        }
    }
    void WatershedPlayer::AdjustSwimVel() {
        if(mSwimming) {
            mLastPos = GetPosition() - mVelocity;
        }
        else {
            mVelocity = GetPosition() - mLastPos;
            mSwimVelocity = 0.0f;
            mSwimForce = 0.0;
        }
        if(mFocusMode || mSurfacing || (!mSwimming && GetCurrentAnimID() != IONA_CSDASH)) {
            mSwimAngle = 0.0f;
        }
    }
    void WatershedPlayer::AdjustSwimAngle() {
        if(mSwimVelocity.Magnitude2() > 0.1) {
            float angleComp = RADIAN * 5;
            if(mTouchingWall || mGrounded) {
                angleComp = RADIAN * 3;
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
        else {
            if(AnimHasFlag(IONA_AFLAG_IDLE_ROTATES)) {
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
    }
    void WatershedPlayer::Hurt(float damage, Vector2f kb, bool knockback) {
        SetFocus(false);
        mHealth-= damage;
        if(mHealth < 0) {
            mHealth = 0;
        }
        mLastPos = GetPosition();
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
        mCancelableSpecial = true;
        ApplyImpulse(kb);
        if(!mSwimming) {
            if(knockback) {
                ChangeAnimIDSpecial(IONA_KNOCKBACKA, false, 0.0f);
            }
            else {
                int anim = Randi(0, 1);
                if(GetCurrentAnimID() != IONA_HITSTUN1A && GetCurrentAnimID() != IONA_HITSTUN1B) {
                    if(anim) {
                        ChangeAnimIDSpecial(IONA_HITSTUN1A, false, 0.0f);
                    }
                    else {
                        ChangeAnimIDSpecial(IONA_HITSTUN2A, false, 0.0f);
                    }
                }
                else {
                    if(GetCurrentAnimID() == IONA_HITSTUN1A && !anim) {
                        ChangeAnimIDSpecial(IONA_HITSTUN2A, false, 0.0f);
                    }
                    else if(GetCurrentAnimID() == IONA_HITSTUN2A && anim) {
                        ChangeAnimIDSpecial(IONA_HITSTUN1A, false, 0.0f);
                    }
                    GetCurrentAnim().Reset();
                    GetCurrentAnim().ForceFrame(6);
                }
            }
        }
        else {
            if(knockback) {
                ChangeAnimIDSpecial(IONA_KNOCKBACKB, false, 0.0f);
            }
            else {
                mSwimVelocity+= kb;
                int anim = Randi(0, 1);
                if(GetCurrentAnimID() != IONA_HITSTUN1B && GetCurrentAnimID() != IONA_HITSTUN2B) {
                    if(anim) {
                        ChangeAnimIDSpecial(IONA_HITSTUN1B, false, 0.0f);
                    }
                    else {
                        ChangeAnimIDSpecial(IONA_HITSTUN2B, false, 0.0f);
                    }
                }
                else {
                    if(GetCurrentAnimID() == IONA_HITSTUN1B && !anim) {
                        ChangeAnimIDSpecial(IONA_HITSTUN2B, false, 0.0f);
                    }
                    else if(GetCurrentAnimID() == IONA_HITSTUN2B && anim) {
                        ChangeAnimIDSpecial(IONA_HITSTUN1B, false, 0.0f);
                    }
                    GetCurrentAnim().Reset();
                    GetCurrentAnim().ForceFrame(6);
                }
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
        if(GetCurrentAnimFinished()) {
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
        mRocketJumpVelocity = 0.0f;
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
    void WatershedPlayer::CheckAndResolveCollision(BackgroundTile2& ground) {
        if(!mSwimming) {
            BasePlayer2::CheckAndResolveCollision(ground);
        }
        else {
            if(ground.GetType() != TILE_GROUND) {
                return;
            }
            //Collision c = CircleRectCollision(mSwimHitBox, ground.GetHitBox());
            //Collision c = solstice::CheckCollision(&mSwimHitBox, ground.GetHitBox2().GetHitBox(0).get());
            Collision c = solstice::CheckCollision(&GetHitBox(), ground.GetHitBox2().GetHitBox(0).get());
            if(!c) {
                return;
            }
            if(c.hasCollision && GetInvMass()) {
                mCollisionFound = true;
                CheckCollisions(c.mtv);
                if(ground.GetType() == TILE_GROUND) {
                    Vector2f velocity = GetPosition() - GetLastPosition();
                    Vector2f normalVel = c.normal.Dot(velocity) * c.normal;
                    Vector2f tangentVel = velocity - normalVel;
                    float groundFriction = ground.GetFriction() * fabs(c.normal.y);
                    if(c.normal.x && c.normal.y > 0.25f) {
                        groundFriction = 1.0f;
                    }
                    float friction = std::max(mFriction, groundFriction);
                    float elasticity = std::max(mElasticity, ground.GetElasticity());
                    Vector2f vel = tangentVel * -friction + normalVel * -(1 + elasticity);
                    ResolveCollision(c, 1.0f, vel);
                }
            }
        }
    }
    bool WatershedPlayer::CheckCollision(WaterBody& waterBody) {
        float depth = waterBody.CheckCollision(GetHitBox());
        if(depth > 0.0f) {
            bool onSurface = waterBody.CheckSurfaceCollision(GetHitBox());
            if(onSurface) {
                mTouchingWaterSurface = true;
            }
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
    void WatershedPlayer::SetGrounded(bool g) {
        mGrounded = g;
        if(g) {
            if(!mUnderwater) {
                if(!mFins && fabs(mVelocity.x) < 0.01f && mVelocity.y <= -1.0f) {
                    Land();
                }
            }
        }
    }
    void WatershedPlayer::Land() {
        if(AnimationHasFlag(IONA_AFLAG_CASTING)) {
            TransitionToAnimation(IONA_CASTING_A, 0.0, GetCurrentFrameNum(), 6);
        }
        else if(AnimationHasFlag(IONA_AFLAG_MAGIC)) {
            TransitionToAnimation(IONA_SPELL_A, 0.0, GetCurrentFrameNum(), GetCurrentFrameNum() + 1);
        }
        else {
            if(GetCurrentAnim().HasFlag(IONA_AFLAG_MELEE)) {
                mCancelableSpecial = true;
            }
            ChangeAnimIDSpecial(ANIM_LAND, true, 0.0f);
        }
    }
    void WatershedPlayer::ResolveCollision(Collision& c, float mass, Vector2f vel) {
        if(!mSwimming) {
            if(c.mtv.y > 0.0 && c.normal.x) {
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
                if(!mFins && fabs(mVelocity.x) < 0.01f && mVelocity.y <= -1.0f) {
                    Land();
                }
                if(mFins && mVelocity.y < 0.0f) {
                    if(!mHurling) {
                        mCancelableSpecial = true;
                        ChangeAnimIDSpecial(IONA_LAND_FINS, false, 0.0f);
                    }
                    else {
                        mCancelableSpecial = true;
                        ChangeAnimIDSpecial(IONA_KDRECOV2A, true, 0.0f);
                    }
                    SetFocus(false);
                    mActionKeys[PKEY_BUTTON1].keyState = KEY_DOWNCHECKED;
                    StopX();
                }
                if(mVelocity.y <= 1.5f && c.normal.y >= 0.2f) {
                    SetGrounded(true);
                }
                if(mGrounded && mVelocity.y <= 0 && c.normal.y > 0.01 && c.normal.y < 0.5f) {
                    mSlopeForce = mWalkSpeed * 2;
                }
            }
        }
        else {
            mSwimVelocity+= vel * mass;
            if(mHurling) {
                mSwimVelocity = 0.0f;
                mRocketJumpVelocity = 0.0f;
                mLastPos = GetPosition();
                ForceRecover();
            }
            AddXY(c.mtv * mass);
        }
        if(c.mtv.x) {
            mTouchingWall = true;
        }
        if(c) {
            if(GetCurrentAnimID() == IONA_CSDASH) {
                StopCharging();
                ChangeAnimIDSpecial(IONA_CSIMPACT, false, 0.0f);
            }
        }
    }
    void WatershedPlayer::ResolveCollision(RigidBody* b, Collision& c, Vector2f vel) {
        CheckCollisions(c.mtv);
        b->CheckCollisions(-c.mtv);
        if(c.normal.y >= 0.5) {
            mGrounded = true;
            if(fabs(mVelocity.x) < 0.01f) {
                if(!mFins && mVelocity.y <= -2.0f) {
                    Land();
                }
                if(mFins) {
                    mCancelableSpecial = true;
                    ChangeAnimIDSpecial(IONA_LAND_FINS, false, 0.0f);
                    mActionKeys[PKEY_BUTTON1].keyState = KEY_DOWNCHECKED;
                    StopX();
                }
            }
        }
        if(!mSwimming) {
            BasePlayer2::ResolveCollision(b, c, vel);
            if(c.normal.y > 0) {
                if(!mFins && fabs(mVelocity.x) < 0.01f && mVelocity.y <= -3.0f) {
                    ChangeAnimIDSpecial(ANIM_LAND, true, 0.0f);
                }
                if(mFins && mVelocity.y < 0.0f) {
                    if(!mHurling) {
                        ChangeAnimIDSpecial(IONA_LAND_FINS, false, 0.0f);
                    }
                    else {
                        mCancelableSpecial = true;
                        ChangeAnimIDSpecial(IONA_KDRECOV2A, true, 0.0f);
                        StopCharging();
                    }
                    mActionKeys[PKEY_BUTTON1].keyState = KEY_DOWNCHECKED;
                    StopX();
                }
            }
        }
        else {
            float massA = GetInvMass() / (GetInvMass() + b->GetInvMass());
            float massB = b->GetInvMass() / (GetInvMass() + b->GetInvMass());
            mSwimVelocity+= vel * massA;
            AddXY(c.mtv * massA);
            b->GetLastPosition()+= vel * massB;
            if(mHurling) {
                mSwimVelocity = 0.0f;
                mRocketJumpVelocity = 0.0f;
                mLastPos = GetPosition();
                ForceRecover();
            }
            if(GetCurrentAnimID() == IONA_CSDASH) {
                mCancelableSpecial = true;
                ChangeAnimIDSpecial(IONA_CSIMPACT, false);
                mCharging = false;
            }
            b->AddXY(c.mtv * massB);
            mTouchingWall = true;
        }
    }
    void WatershedPlayer::CheckBulletCollisions(LevelRoom& room) {
        std::vector<BackgroundTile2>& tiles = room.GetGroundTiles();
        std::vector<RigidBody*>& bodies = room.GetBodyPointers();
        for(size_t i = 0; i < tiles.size(); i++) {
            if(mWaterPistol) {
                mWaterPistol->CheckAndResolveCollision(tiles[i]);
            }
            if(mLaser) {
                mLaser->CheckAndResolveCollision(tiles[i]);
            }
            if(mMissile) {
                mMissile->CheckAndResolveCollision(tiles[i]);
            }
            if(mFoamThrower) {
                mFoamThrower->CheckAndResolveCollision(tiles[i]);
            }
            mMagic.CheckAndResolveCollision(tiles[i]);
            mWaterHook.CheckAndResolveCollision(tiles[i]);
        }
        for(size_t i = 0; i < bodies.size(); i++) {
            if(mWaterPistol) {
                mWaterPistol->CheckAndResolveCollision(bodies[i]);
            }
            if(mLaser) {
                mLaser->CheckAndResolveCollision(bodies[i]);
            }
            if(mMissile) {
                mMissile->CheckAndResolveCollision(bodies[i]);
            }
            if(mFoamThrower) {
                mFoamThrower->CheckAndResolveCollision(bodies[i]);
            }
            mMeleeBulletSpawner.CheckAndResolveCollision(bodies[i]);
            mMagic.CheckAndResolveCollision(bodies[i]);
        }
    }
    void  WatershedPlayer::CheckPlatform(Collision& c, BackgroundTile2& bg) {
        bool land = false;
        Vector2f velocity = GetPosition() - GetLastPosition();
        Rectanglef rect = bg.GetHitBox2().GetRect(0);
        if(bg.GetSprings()) {
            if(velocity.y <= 0) {
                if(!bg.Springing()) {
                    if(GetHitBox().GetBottom() >= rect.GetBottom()) {
                        SetPosition(Vector2f(GetPosition().x, rect.GetTop()));
                        GetLastPosition().y = GetPosition().y;
                        SetGrounded(true);
                        land = true;
                    }
                }
                else if(GetHitBox().GetBottom() - velocity.y >= rect.GetBottom()) {
                    GetLastPosition().y = GetPosition().y;
                    SetGrounded(true);
                    land = true;
                    SetPosition(Vector2f(GetPosition().x, rect.GetTop()));
                    AddY(c.mtv.y);
                }
            }
        }
        else if(GetHitBox().GetBottom() >= bg.GetHitBox().GetBottom() && velocity.y < 0.0f) {
            GetLastPosition().y = GetPosition().y;
            SetGrounded(true);
            land = true;
            SetPosition(Vector2f(GetPosition().x, bg.GetHitBox().GetTop()));
        }
        if(mGrounded && mAppliedForce.y < 0) {
            mAppliedForce.y = 0;
        }
        if(land) {
            if(!mFins) {
                Land();
            }
            if(mFins) {
                ChangeAnimIDSpecial(IONA_LAND_FINS, false, 0.0f);
                mActionKeys[PKEY_BUTTON1].keyState = KEY_DOWNCHECKED;
                StopX();
            }
        }
    }
    void WatershedPlayer::ToggleSpellRight() {
        mEquippedSpellType++;
        if(mEquippedSpellType > SPELL_CUSTOM - 1) {
            mEquippedSpellType = 0;
        }
    }
    void WatershedPlayer::ToggleSpellLeft() {
        mEquippedSpellType--;
        if(mEquippedSpellType < 0) {
            mEquippedSpellType = SPELL_CUSTOM - 1;
        }
    }
    void WatershedPlayer::FindNextBulletIndex() {
    }
    void WatershedPlayer::CreateBullet(int type) {
        FindNextBulletIndex();
        switch(type) {
            case SPELL_PISTOL:
                if(mWaterPistol) {
                    //mActiveBullets[mNextBulletIndex] = std::make_shared<BulletBase>(*mPistol);
                    mWaterPistol->mAimDirection = mAimDirection;
                    mWaterPistol->CreateBullet();
                }
                break;
            case SPELL_MISSILE:
                if(mMissile) {
                    mMissile->mAimDirection = mAimDirection;
                    mMissile->CreateBullet();
                }
                break;
            case SPELL_LASER:
                if(mLaser) {
                    mLaser->mAimDirection = mAimDirection;
                    mLaser->CreateBullet();
                }
                break;
            case SPELL_THROWER:
                if(mFoamThrower) {
                    //mFoamThrower->CreateBullet();
                }
                break;
            case SPELL_MAGIC:
                /*
                if(mMagic) {
                    mActiveBullets[mNextBulletIndex] = std::make_shared<BulletBase>(*mMagic);
                }
                */
                break;
            case SPELL_MELEE:
                /*
                if(mMeleeSlashes) {
                    mActiveBullets[mNextBulletIndex] = std::make_shared<BulletBase>(*mMeleeSlashes);
                }
                */
                break;
            case SPELL_CUSTOM:
                break;
            default:
                break;
        }
    }
    void WatershedPlayer::Attack() {
        if(GetKeyDown(PKEY_SHOOT)) {
            switch(mEquippedSpellType) {
            case SPELL_PISTOL:
                Shoot();
                break;
            case SPELL_MAGIC:
                mActionKeys[PKEY_SHOOT].keyState = KEY_DOWNCHECKED;
                CastMagic();
                break;
            case SPELL_MISSILE:
                Shoot();
                break;
            case SPELL_LASER:
                Shoot();
                break;
            case SPELL_MELEE:
                mActionKeys[PKEY_SHOOT].keyState = KEY_DOWNCHECKED;
                mCurrentMeleeCharge = 0.0;
                MeleeCombo();
                break;
            case SPELL_THROWER:
                Shoot();
                break;
            case SPELL_CUSTOM:
            default:
                break;
            }
        }
        else if(mActionKeys[PKEY_SHOOT].keyState == KEY_RELEASED && mCurrentMeleeCharge >= mMaxMeleeCharge) {
            if(mUnderwater) {
                ChangeAnimIDSpecial(IONA_SEASICKLE_CHARGEB, false);
            }
            else if(mGrounded) {
                ChangeAnimIDSpecial(IONA_SEASICKLE_CHARGEA, false);
            }
            else {
                ChangeAnimIDSpecial(IONA_SEASICKLE_CHARGEAIR, false);
                StopY();
            }
            mCurrentMeleeCharge = 0.0;
        }
        else if(mActionKeys[PKEY_SHOOT].keyState == KEY_RELEASED) {
            EndMagicCast();
        }
        if(GetCurrentAnimID() == IONA_SEASICKLE_CHARGEAIR) {
            StopY();
            ApplyImpulseY(-3);
        }
    }
    void WatershedPlayer::UpdateShootMode() {
        if(mShootResetTimer > 0) {
            mShootResetTimer--;
        }
        if(mLightBrightness >= 0.0) {
            mLightBrightness-= mLightBrightStep;
            mShotLightColorCur = mShotLightColor * mLightBrightness;
        }
        if(IsShooting() && !mShootResetTimer) {
            mSpecialAnim = false;
            mShootResetTimer = 0;
            RemoveFlags(IONA_AFLAG_SHOOTING);
        }
        else if(IsShooting() && mSwimming && !AnimHasFlag(IONA_AFLAG_BRAKING)) {
            if(mLeftHeld) {
                mFlipped = true;
                mSwimAngle = PI;
            }
            if(mRightHeld) {
                mFlipped = false;
                mSwimAngle = 0;
            }
        }
        if(!mFocusMode) {
            if((mFacingLeft && !mRightHeld) || mLeftHeld) {
                mAimDirection.x = -1.0;
                mAimDirection.y = 0.0;
            }
            else {
                mAimDirection.x = 1.0;
                mAimDirection.y = 0.0;
            }
            if(mUpHeld) {
                mAimDirection.y = 1.0;
                mAimDirection.x = 0.0f;
            }
            if(mDownHeld) {
                mAimDirection.y = -1.0;
                mAimDirection.x = 0.0f;
            }
            if(mHeldObject) {
                mAimDirection.y = 0;
            }
        }
        else {
            mAimDirection = mFocusDirection;
        }
    }
    void WatershedPlayer::Shoot() {
        float norm = 2.4 * 2.0 / 1280.0;
        if((mCancelableSpecial || !mSpecialAnim)) {
            AddFlags(IONA_AFLAG_SHOOTING);
            mShotOrigin = GetShellPosition(norm);
            Vector2f startVel;
            startVel.x = mVelocity.x;
            std::shared_ptr<BulletSpawner> bullet;
            if(mFoamThrower) {
                mFoamThrower->mActivated = false;
            }
            switch(mEquippedSpellType) {
                case SPELL_PISTOL:
                    bullet = mWaterPistol;
                    break;
                case SPELL_LASER:
                    bullet = mLaser;
                    break;
                case SPELL_THROWER:
                    bullet = mFoamThrower;
                    mFoamThrower->mActivated = true;
                    break;
                case SPELL_MISSILE:
                    bullet = mMissile;
                    break;
                default:
                    bullet = nullptr;
                    break;
            }
            if(mEquippedSpellType != SPELL_THROWER) {
                if(bullet && bullet->CreateBullet(mAimDirection.GetUnit(), mShotOrigin)) {
                    mLightBrightness = 1.0f;
                    mShotLightColor = bullet->mFlashColor;
                    if(AnimHasFlag(IONA_AFLAG_BRAKING) && GetCurrentAnimFinished()) {
                        EndBrake();
                    }
                    if(mSkidding) {
                        mSkidding = false;
                        mSpecialAnim = false;
                    }
                    mShootResetTimer = 15;
                    if(mSwimming) {
                        if(GetCurrentAnimID() == IONA_SWIM_TWIRL) {
                            mSwimAngle = atan2(mAimDirection.y, mAimDirection.x);
                        }
                        else if(!AnimHasFlag(IONA_AFLAG_TURNING)) {
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
            else {
                bullet->mAimDirection = mAimDirection.GetUnit();
                bullet->mSpawnPosition = mShotOrigin;
                mFoamThrower->mActivated = true;
                mLightBrightness = 1.0f;
                mShotLightColor = bullet->mFlashColor;
                if(AnimHasFlag(IONA_AFLAG_BRAKING) && GetCurrentAnimFinished()) {
                    EndBrake();
                }
                if(mSkidding) {
                    mSkidding = false;
                    mSpecialAnim = false;
                }
                mShootResetTimer = 15;
                if(mSwimming) {
                    if(GetCurrentAnimID() == IONA_SWIM_TWIRL) {
                        mSwimAngle = atan2(mAimDirection.y, mAimDirection.x);
                    }
                    else if(!AnimHasFlag(IONA_AFLAG_TURNING)) {
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
        if(mShootResetTimer) {
            if(mSkidding) {
                mSkidding = false;
                ChangeAnimID(IONA_SWIM_IDLE);
            }
        }
    }
    void WatershedPlayer::ShootBullet(Bullet b) {

    }
    void WatershedPlayer::Brake() {
        if(AnimHasFlag(IONA_AFLAG_BRAKING)) {
            return;
        }
        if(mSwimming) {
            if(IsShooting()) {
                ChangeAnimIDSpecial(IONA_SWIM_STOPBRAKE, true, 0.0f);
            }
            else if(!mHeldObject) {
                ChangeAnimIDSpecial(IONA_SWIM_BRAKE, true, 0.0f);

            }
            else if(AnimHasFlag(IONA_AFLAG_TURNING)) {
                ChangeAnimIDSpecial(IONA_TURN_SWIM, true, 0.0f);
            }
            mFlipTimer = 0;
        }
        else if(!mUnderwater) {
            if(IsShooting()) {
                if(mUpHeld) {
                    ChangeAnimIDSpecial(IONA_SHOOT_BRAKE_UFORWARD, true, 0.0f);
                }
                else if(mDownHeld) {
                    ChangeAnimIDSpecial(IONA_SHOOT_BRAKE_DFORWARD, true, 0.0f);
                }
                else {
                    ChangeAnimIDSpecial(IONA_SHOOT_BRAKE, true, 0.0f);
                }
                mShootResetTimer+= 20;
            }
            else if (mGrounded) {
                if(mLeftHeld || mRightHeld) {
                    ChangeAnimIDSpecial(IONA_RUN_BRAKE, true, 0.0f);
                }
                else {
                    ChangeAnimIDSpecial(IONA_IDLE_BRAKE, true, 0.0f);
                }
            }
        }
    }
    void WatershedPlayer::EndBrake() {
        if(AnimHasFlag(IONA_AFLAG_BRAKING)) {
            if(!mFins) {
                if(mVelocity.x) {
                    int transFrame = 0;
                    float transSpeed = 0.1f;
                    if(IsShooting()) {
                        if(mUpHeld) {
                            ChangeAnimID(IONA_RUNSHOOT_UFORWARD, 0);
                        }
                        else if(mDownHeld) {
                            ChangeAnimID(IONA_RUNSHOOT_DFORWARD, 0);
                        }
                        else {
                            ChangeAnimID(IONA_RUNSHOOT_FORWARD, 0);
                        }
                    }
                    else {
                        ChangeAnimID(ANIM_WALK, 0);
                    }
                }
                else {
                    if(IsShooting()) {
                        ChangeAnimID(IONA_SHOOT_GROUND, 0.0);
                    }
                    else {
                        ChangeAnimID(ANIM_IDLE);
                    }
                }
                mFlipped = !mFlipped;
            }
            else {
                mSkidding = false;
                if(mSwimming) {
                    mFlipped = !mFlipped;
                    mFlipTimer = 0.0f;
                    bool holdingDirection = mLeftHeld || mRightHeld || mDownHeld || mUpHeld;
                    if(mFlipped) {
                        if(!holdingDirection) {
                            mSwimAngle = PI;
                        }
                        mFacingLeft = true;
                    }
                    else {
                        if(!holdingDirection) {
                            mSwimAngle = 0.0f;
                        }
                        mFacingLeft = false;
                    }
                    if(mLeftHeld || mRightHeld || mUpHeld || mDownHeld) {
                        ChangeAnimID(IONA_SWIM, 0.0);
                        GetCurrentAnim().ForceFrame(11);
                        mSwimAngle = atan2(mSwimDirection.y, mSwimDirection.x);
                    }
                    else {
                        ChangeAnimID(IONA_SWIM_IDLE, 0.0);
                    }
                }
            }
        }
        else if(mSwimming) {

        }
    }
    void WatershedPlayer::Turn() {
        if(mFocusMode || (IsShooting() && fabs(mVelocity.x) < 1.5)) {
            return;
        }
        if(!AnimHasFlag(IONA_AFLAG_BRAKING)) {
            if((mLeftHeld && !mFlipped) || (mRightHeld && mFlipped)) {
                if(mSwimming) {
                    ChangeAnimIDSpecial(IONA_TURN_SWIM, true, 0.0f);
                    if(mLeftHeld && !mFlipped) {
                        mSwimAngle = 0;
                    }
                    if(mRightHeld && mFlipped) {
                        mSwimAngle = PI;
                    }
                }
                else if(mFins && mSurfaced) {
                    ChangeAnimIDSpecial(IONA_TURN_MERMAID, true, 0.0f);
                }
                else if(mGrounded) {
                    ChangeAnimIDSpecial(IONA_TURN, true, 0.0f);
                }
            }
        }
        else if(mSwimming) {
            if(mRightHeld && !mFacingLeft && GetCurrentFrameNum() > 2) {
                mSwimAngle = PI;
                mFlipped = true;
                mFlipTimer = 0.0;
                ChangeAnimIDSpecial(IONA_TURN_SWIM, true, 0.0f);
            }
            else if(mLeftHeld && mFacingLeft && GetCurrentFrameNum() > 2) {
                mSwimAngle = 0;
                mFlipped = false;
                mFlipTimer = 0.0;
                ChangeAnimIDSpecial(IONA_TURN_SWIM, true, 0.0f);
            }
        }
    }
    void WatershedPlayer::EndTurn() {
        if(AnimHasFlag(IONA_AFLAG_TURNING) && !AnimHasFlag(IONA_AFLAG_BRAKING)) {
            if(mSwimming) {
                mFlipped = !mFlipped;
                if(mVelocity.Magnitude2() > 0.1f) {
                    mSwimAngle = atan2(mSwimDirection.y, mSwimDirection.x);
                    mFlipTimer = 0.0f;
                    TransitionToAnimation(IONA_SWIM, 0.1, 0, 4);
                }
                else {
                    ChangeAnimID(IONA_SWIM_IDLE, 0.0f);
                    mFlipTimer = 0.0f;
                    if(mFacingLeft) {
                        mSwimAngle = PI;
                    }
                    else {
                        mSwimAngle = 0.0f;
                    }
                }
                mSkidding = false;
            }
            else if(mFins) {
                mFlipped = !mFlipped;
                mFlipTimer = 0.0f;
                if(mVelocity.x) {
                    TransitionToAnimation(IONA_MERMAID_SWIM, 0.1f, 0, 0);
                }
                else {
                    ChangeAnimID(IONA_MERMAID_IDLE, 0.0f);
                }
            }
            else {
                mFlipped = !mFlipped;
                mFlipTimer = 0.0f;
                if(mVelocity.x && !IsShooting()) {
                    TransitionToAnimation(ANIM_WALK, 0.1f, 0, 0);
                }
                else {
                    ChangeAnimID(ANIM_IDLE, 0.0f);
                }
            }
        }
    }
    void WatershedPlayer::AdjustVelocity() {
        if(!mSwimming) {
            float maxVelocity = mWalkSpeed;
            if(GetCurrentAnimID() == IONA_ROLL_A) {
                maxVelocity = mRollSpeed;
            }
            mVelocity = GetPosition() - GetLastPosition();
            if(mRightHeld && mVelocity.x > maxVelocity) {
                mVelocity.x = maxVelocity;
            }
            if(mLeftHeld && mVelocity.x < -maxVelocity) {
                mVelocity.x = -maxVelocity;
            }
            if(fabs(mVelocity.x) < maxVelocity || fabs(mVelocity.x) > mMaxVelocity.x) {
                mLastPos.x = GetPosition().x;
            }
            if(fabs(mVelocity.y) > mMaxVelocity.y || fabs(mVelocity.y) < 0.5f) {
                mLastPos.y = GetPosition().y;
            }
            mSwimVelocity = 0;
            mRocketJumpVelocity = 0.0f;
        }
        else {
            Vector2f maxVel = mSwimVelocity.GetUnit() * mSwimMaxVel;
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
        if(mFocusMode) {
            if(mFacingLeft) {
                mFlipped = true;
            }
            else {
                mFlipped = false;
            }
        }
        glm::vec3 modelScale = mModel.GetTransform().GetScale();
        bool rotateZ = !mSwimming;
        if(GetCurrentAnimID() == IONA_CSDASH || GetCurrentAnimID() == IONA_CSIMPACT) {
            rotateZ = false;
        }
        if(rotateZ || mFocusMode) {
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
        float xAngle = mModel.GetTransform().GetRotation().x;
        float yAngle = 0.0f;
        if((mSwimming && !mFocusMode && !mHurling) || (GetCurrentAnim() == IONA_CSDASH || GetCurrentAnim() == IONA_CSIMPACT)) {
            yAngle = -mSwimAngle;
        }
        if((GetCurrentAnimID() == IONA_KNOCKDOWN2A || GetCurrentAnimID() == IONA_KNOCKDOWN2B) && !mSwimming) {
            yAngle+= PI * 0.5;
        }
        else {
            yAngle+= mHurlAngle;
        }
        mModel.SetAngle(xAngle, yAngle, 0.0);
        mModel.SetScale(modelScale);
        mModel.SetTranslation(pos);

        mModel.Update(t);
        Bone& handBone = mModel.GetCurrentAnim()->GetCurrentFrame().bones[mHandAnchorID];
        Bone& headBone = mModel.GetCurrentAnim()->GetCurrentFrame().bones[mHeadAnchorID];
        if(mRodVisible) {
            int k = mHoloRod.GetBoneID("Rod_Base");
            Bone& rodBone = mHoloRod.GetCurrentAnim()->GetCurrentFrame().bones[k];
            mHoloRod.GetCurrentAnim()->SetParentMatrix(GetShellTransform() * rodBone.offsetMatrix);
            mHoloRod.Update(t);
        }
        else {
            glm::mat4 boneMat = mShellModel.GetCurrentAnim()->GetCurrentFrame().bones[0].offsetMatrix;
            mShellModel.GetCurrentAnim()->SetParentMatrix(GetShellTransform() * boneMat);
            mShellModel.Update(t);
            glm::mat4 shellMat = GetShellTransform();
            mShellPos = mShellModel.GetVertexPosition(0, 0, 0, shellMat, mModelTrans);
        }
        //mHairband.GetCurrentAnim()->SetParentMatrix(GetShellTransform() * headBone.offsetMatrix);
        mHairband.GetCurrentAnim()->SetParentMatrix(headBone.finalTransformation * headBone.offsetMatrix);
        mHairband.Update(t);
    }
    glm::mat4 WatershedPlayer::GetShellTransform() {
        glm::mat4 boneMat = mShellModel.GetCurrentAnim()->GetCurrentFrame().bones[0].offsetMatrix;
        return mModel.GetCurrentAnim()->GetCurrentFrame().bones[mHandAnchorID].finalTransformation;
    }
    Vector2f WatershedPlayer::GetShellPosition(float norm) {
        return Vector2f(mShellPos.x / norm, mShellPos.y / norm);
    }
    void WatershedPlayer::Draw(ShaderProgram& shader, TextureArray& textures) {
        if(mUsesModel) {
            mModelTrans = mModel.GetTransform().GetTransform();
            mModel.Draw(shader, textures, mModelTrans);
            mHairband.Draw(shader, textures, mModelTrans);
            if(mRodVisible) {
                mHoloRod.Draw(shader, textures, mModelTrans);
            }
            else {
                mShellModel.Draw(shader, textures, mModelTrans);
            }
        }
    }
    int WatershedPlayer::GetCurrentBullet() {
        switch(mEquippedSpellType) {
        case 0:
            return 0;
        case 1:
            return 0;
        case 2:
            return 0;
        case 3:
            return 0;
        case 4:
            return 0;
        default:
            return 0;
        }
    }
    void WatershedPlayer::DrawBullets(SceneRenderer& rend) {
        if(mWaterPistol) {
            mWaterPistol->Draw(rend);
        }
        if(mLaser) {
            mLaser->Draw(rend);
        }
        if(mMissile) {
            mMissile->Draw(rend);
        }
        if(mFoamThrower) {
            mFoamThrower->Draw(rend);
        }
        mMagic.Draw(rend);
        mMeleeBulletSpawner.Draw(rend);
        mWaterHook.Draw(rend);
    }
    void WatershedPlayer::DrawBulletHitBoxes(SceneRenderer& rend, Color color) {
        if(mWaterPistol) {
            mWaterPistol->DrawHitBoxes(rend, Color(1.0, 0.0, 0.0, 0.25));
        }
        if(mLaser) {
            mLaser->DrawHitBoxes(rend, Color(1.0, 0.0, 0.0, 0.25));
        }
        if(mMissile) {
            mMissile->DrawHitBoxes(rend, Color(1.0, 0.0, 0.0, 0.25));
        }
        if(mFoamThrower) {
            mFoamThrower->DrawHitBoxes(rend, Color(1.0, 0.0, 0.0, 0.25));
        }
        mMeleeBulletSpawner.DrawHitBoxes(rend, Color(1.0, 0.0, 0.0, 0.25));
        mWaterHook.DrawHitBoxes(rend, Color(1.0, 0.0, 0.0, 0.25));
    }
    std::string WatershedPlayer::GetSpellString() {
        switch(mEquippedSpellType) {
            case SPELL_PISTOL:
                return "Water Pistol";
            case SPELL_MISSILE:
                return "Missile";
            case SPELL_LASER:
                return "Laser";
            case SPELL_THROWER:
                return "Foamthrower";
            case SPELL_MAGIC:
                return "Magic";
            case SPELL_MELEE:
                return "Melee";
            case SPELL_CUSTOM:
                return "Custom spell";
            default:
                return "None";
        }
    }
    void WatershedPlayer::CheckFocusAnimationsModel() {
        if(!UsesFocusAnims() || (AnimHasFlag(IONA_AFLAG_BRAKING) || AnimHasFlag(IONA_AFLAG_TURNING))) {
            return;
        }
        bool facingForward = true;
        if(mFocusMode) {
            if((mFacingLeft && mVelocity.x <= 0) || (!mFacingLeft && mVelocity.x >= 0)) {
                facingForward = true;
            }
            else {
                facingForward = false;
            }
        }
        bool upAim = false;
        bool downAim = false;
        if(mFocusMode) {
            if(mFocusDirection.y > 0) {
                upAim = true;
                downAim = false;
            }
            else if(mFocusDirection.y < 0) {
                downAim = true;
                upAim = false;
            }
        }
        else {
            if(mUpHeld) {
                upAim = true;
                downAim = false;
            }
            else if(mDownHeld) {
                upAim = false;
                downAim = true;
            }
        }
        if(mUnderwater && GetSubmergeDepth() >= 0.3) {
            if(fabs(mVelocity.x) > 0.01 && !mSurfacing) {
                float transitionSpeed = 0.025f;
                if(!facingForward && mFocusMode) {
                    ChangeAnimID(IONA_SWIMSHOOT_BACK, transitionSpeed);
                }
                else if(downAim && facingForward) {
                    ChangeAnimID(IONA_SWIMSHOOT_DFORWARD, transitionSpeed);
                }
                else if(upAim && facingForward) {
                    ChangeAnimID(IONA_SWIMSHOOT_UFORWARD, transitionSpeed);
                }
                else {
                    ChangeAnimID(IONA_SWIMSHOOT_FORWARD, transitionSpeed);
                }
            }
            else {
                if(downAim) {
                    if(mVelocity.y < 0) {
                       ChangeAnimID(IONA_SHOOT_DSWIM_DOWN);
                    }
                    else if(mVelocity.y > 0) {
                       ChangeAnimID(IONA_SHOOT_USWIM_DOWN);
                    }
                    else {
                        ChangeAnimID(IONA_SWIMSHOOT_IDLE_DOWN);
                    }
                }
                else if(upAim) {
                    if(mVelocity.y > 0) {
                        ChangeAnimID(IONA_SHOOT_USWIM_UP);
                    }
                    else if(mVelocity.y < 0) {
                       ChangeAnimID(IONA_SHOOT_DSWIM_UP);
                    }
                    else {
                        ChangeAnimID(IONA_SWIMSHOOT_IDLE_UP);
                    }
                }
                else {
                    if(mVelocity.y > 0) {
                        ChangeAnimID(IONA_SWIMSHOOT_UP);
                    }
                    else if(mVelocity.y < 0) {
                        ChangeAnimID(IONA_SWIMSHOOT_DOWN);
                    }
                    else {
                        ChangeAnimID(IONA_SWIMSHOOT_IDLE);
                    }
                }
                mHitBox.SetAngle(0.0f);
            }
        }
        else if(!AnimHasFlag(IONA_AFLAG_BRAKING) && !AnimHasFlag(IONA_AFLAG_TURNING) && (GetCurrentAnimID() != ANIM_LAND && !GetCurrentAnimFinished())) {
            if(mGrounded) {
                int forceFrame = 0;
                float transSpeed = 0.075;
                int lastID = GetCurrentAnimID();
                int curFrame = GetCurrentFrameNum();
                if(fabs(mVelocity.x) >= 1.0) {
                    forceFrame = GetCurrentFrameNum() + 1;
                    transSpeed = 0.025;
                }
                if(mFocusMode && !facingForward) {
                    if(downAim) {
                        TransitionToAnimation(IONA_RUNSHOOT_DBACK, transSpeed, curFrame, forceFrame);
                    }
                    else if(upAim) {
                        TransitionToAnimation(IONA_RUNSHOOT_UBACK, transSpeed, curFrame, forceFrame);
                    }
                    else {
                        ChangeAnimID(IONA_RUNSHOOT_BACK, transSpeed);
                    }
                }
                else {
                    if(mVelocity.x) {
                        if(downAim) {
                            TransitionToAnimation(IONA_RUNSHOOT_DFORWARD, transSpeed, curFrame, forceFrame);
                        }
                        else if(upAim) {
                            TransitionToAnimation(IONA_RUNSHOOT_UFORWARD, transSpeed, curFrame, forceFrame);
                        }
                        else {
                            TransitionToAnimation(IONA_RUNSHOOT_FORWARD, transSpeed, curFrame, forceFrame);
                        }
                    }
                    else {
                        if(downAim) {
                            ChangeAnimID(IONA_SHOOT_DOWN, transSpeed);
                        }
                        else if(upAim) {
                            ChangeAnimID(IONA_SHOOT_UP, transSpeed);
                        }
                        else {
                            ChangeAnimID(IONA_SHOOT_GROUND, transSpeed);
                        }
                    }
                }
            }
            else if(mVelocity.y > 0.0f || mVelocity.y <= -1.0f) {
                float transSpeed = 0.05f;
                if(!facingForward && mFocusMode) {
                    ChangeAnimID(IONA_SHOOT_BACK_JUMP, transSpeed);
                }
                else if(downAim) {
                    ChangeAnimID(IONA_SHOOT_JUMP_DOWN, transSpeed);
                }
                else if(upAim) {
                    ChangeAnimID(IONA_SHOOT_JUMP_UP, transSpeed);
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
        if(mRecoiling && (GetCurrentAnim().SubAnimFinished() || GetCurrentAnimFinished())) {
            mRecoiling = false;
        }
        if(mSpecialAnim) {
            if(GetCurrentAnim().HasFlag(IONA_AFLAG_MELEE)) {
                CreateMeleeSlashes();
            }
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
            if(GetCurrentAnimID() == IONA_IDLE_BRAKE) {
                if((mLeftHeld && mVelocity.x < 0.0f && !mFlipped) || (mRightHeld && mVelocity.x > 0.0f && mFlipped)) {
                    Brake();
                }
                else if((mLeftHeld && mVelocity.x < 0.0f && mFlipped) || (mRightHeld && mVelocity.x > 0.0f && !mFlipped)) {
                    mSpecialAnim = false;
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
            if(GetCurrentAnimID() == IONA_CSDASH && mVelocity.Magnitude2() < mSwimMaxVel * mSwimMaxVel) {
                mCancelableSpecial = true;
                mCharging = false;
                //ChangeAnimID(IONA_SWIM);
                TransitionToAnimation(IONA_SWIM, 0.05, GetCurrentFrameNum(), 10);
                if(GetKeyDown(PKEY_JUMP)) {
                    mActionKeys[PKEY_JUMP].keyState = KEY_DOWNCHECKED;
                }
            }
            if(GetCurrentAnimFinished()) {
                mSpecialAnim = false;
                mCancelableSpecial = true;
                mHurling = false;
                if(AnimHasFlag(IONA_AFLAG_TURNING)) {
                    EndTurn();
                }
                if(AnimHasFlag(IONA_AFLAG_BRAKING)) {
                    EndBrake();
                }
                if(GetCurrentAnimID() == IONA_LAND_FINS) {
                    ChangeAnimID(ANIM_IDLE);
                }
                else if(GetCurrentAnimID() == IONA_CSIMPACT) {
                    StopCharging();
                }
                else if(GetCurrentAnimID() == IONA_SEASICKLE_1A || GetCurrentAnimID() == IONA_SEASICKLE_2A ||
                        GetCurrentAnimID() == IONA_SEASICKLE_3A || GetCurrentAnimID() == IONA_SEASICKLE_CHARGEA) {
                    ChangeAnimID(ANIM_IDLE, 0.1);
                }
                else if(GetCurrentAnimID() == IONA_SEASICKLE_1B || GetCurrentAnimID() == IONA_SEASICKLE_2B ||
                        GetCurrentAnimID() == IONA_SEASICKLE_3B || GetCurrentAnimID() == IONA_SEASICKLE_CHARGEB) {
                    if(mSwimming) {
                        ChangeAnimID(IONA_SWIM_IDLE, 0.1);
                    }
                    else {
                        ChangeAnimID(IONA_MERMAID_IDLE, 0.1);
                    }
                }
                else if(GetCurrentAnimID() == IONA_SEASICKLE_CHARGEAIR || GetCurrentAnimID() == IONA_SEASICKLE_AIR) {
                    ChangeAnimID(ANIM_FALL, 0.1);
                }
                if(GetCurrentAnimID() == IONA_SPELL_A && mMeleeBulletSpawner.HasFinisher()) {
                    GetCurrentAnim().Reset();
                    GetCurrentAnim().ForceFrame(GetCurrentAnim().GetLoopFrame());
                    mSpecialAnim = true;
                    mCancelableSpecial = false;
                }
                if(GetCurrentAnimID() == IONA_CSCHARGE_UP) {
                    ChangeAnimIDSpecial(IONA_CSDASH, false);
                    Vector2f force(mChargeVelocityMax * cos(mSwimAngle), mChargeVelocityMax * sin(mSwimAngle));
                    ApplyImpulse(force);
                }
                RemoveFlags(GetCurrentAnim().GetFlags());
            }
            if(GetCurrentAnimID() == ANIM_IDLE || GetCurrentAnimID() == IONA_SWIM_IDLE) {
                mSpecialAnim = false;
                mCancelableSpecial = true;
            }
            if(GetCurrentAnimID() == IONA_CASTING_A && (mRightHeld || mLeftHeld)) {
                ChangeAnimID(ANIM_WALK, 0.05);
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
    void WatershedPlayer::CheckCastAnims() {
    }
    void WatershedPlayer::CheckSurfaceAnims() {
        if(GetSubmergeRatio() < 0.3) {
            return;
        }
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
                        if(GetCurrentAnimID() == IONA_TURN_MERMAID) {
                            EndTurn();
                        }
                        else {
                            ChangeAnimID(IONA_MERMAID_SWIM);
                    }
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
        }
        if(mSkidding && GetCurrentAnim() != IONA_SWIM_STOPBRAKE) {
            Brake();
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
                if(GetCurrentAnimID() == IONA_SWIMSHOOT_BACK || GetCurrentAnimID() == IONA_SWIMSHOOT_IDLE || GetCurrentAnim() == IONA_SWIM_BRAKE) {
                    Turn();
                }
                if(GetCurrentAnimID() == IONA_SWIM_BRAKE && !mSpecialAnim) {
                    EndBrake();
                }
                else if(mModel.GetCurrentAnimID() != IONA_SWIM_BRAKE && !GetKeyDown(PKEY_BUTTON1) && GetCurrentAnimID() != IONA_TURN_SWIM && !mShootResetTimer) {
                    float angleComp = RADIAN * 5;
                    if(mTouchingWall) {
                        angleComp = RADIAN * 3;
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
                    if(AnimHasFlag(IONA_AFLAG_TURNING)) {
                        EndTurn();
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
                    if(AnimHasFlag(IONA_AFLAG_TURNING)) {
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
        if(!UsesFocusAnims()) {
            if(!mSpecialAnim) {
                if(AnimHasFlag(IONA_AFLAG_FINS) && !AnimHasFlag(IONA_AFLAG_SWIMMING)) {
                    if(mVelocity.y > 0.0f) {
                        ChangeAnimID(ANIM_JUMP);
                    }
                    else {
                        if(!mFins) {
                            ChangeAnimID(ANIM_FALL);
                        }
                        else {
                            ChangeAnimID(IONA_FIN_FALL);
                        }
                    }
                }
                if(mGrounded && mVelocity.x != 0) {
                    float transSpeed = 0.03f;
                    if(AnimHasFlag(IONA_AFLAG_TURNING)) {
                        EndBrake();
                    }
                    else if(!IsShooting()) {
                        ChangeAnimID(ANIM_WALK, transSpeed);
                    }
                    if(!mLeftHeld && !mRightHeld && fabs(mVelocity.x) >= mWalkSpeed * 0.9f) {
                        Brake();
                    }
                }
                if(!mGrounded) {
                    if(mVelocity.y <= 0) {
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
                    if(AnimHasFlag(IONA_AFLAG_TURNING)) {
                        transSpeed = 0.0f;
                        mFlipped = !mFlipped;
                    }
                    else if(GetCurrentAnimID() == IONA_HOOK_PULL || AnimHasFlag(IONA_AFLAG_BRAKING)) {
                        transSpeed = 0.0f;
                    }
                    if(AnimHasFlag(IONA_AFLAG_BRAKING)) {
                        TransitionToAnimation(ANIM_IDLE, transSpeed, 0, 0);
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
                    SetFins(false);
                }
            }
        }
    }
    void WatershedPlayer::SetFocus(bool focus) {
        if(mFocusMode == focus) {
            if(!mFocusMode) {
                mFocusTimer = 0.0;
            }
            return;
        }
        else {
            mFocusMode = focus;
            if(focus) {
                if(mUpHeld) {
                    mFocusDirection.y = 1;
                    mFocusDirection.x = 0;
                }
                else if(mDownHeld) {
                    mFocusDirection.y = -1;
                    mFocusDirection.x = 0;
                }
                else {
                    mFocusDirection.y = 0;
                    if(mFacingLeft) {
                        mFocusDirection.x = -1;
                    }
                    else {
                        mFocusDirection.x = 1;
                    }
                }
                if(AnimHasFlag(IONA_AFLAG_BRAKING)) {
                    EndBrake();
                }
                else if(AnimHasFlag(IONA_AFLAG_TURNING)) {
                    EndTurn();
                }
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
            else {
                mFocusDirection = 0;
                mFocusTimer = 0.0f;
            }
        }
    }
    void WatershedPlayer::SetDirectionFlags() {
        BasePlayer2::SetDirectionFlags();
        if(mLeftHeld && mRightHeld && mFacingLeft) {
            mRightHeld = false;
        }
        else if(mLeftHeld && mRightHeld && !mFacingLeft) {
            mLeftHeld = false;
        }
    }
    void WatershedPlayer::CheckInput() {
        //if(mIgnoreInput || (mSpecialAnim && !mCancelableSpecial)) {
        if(mIgnoreInput) {
            return;
        }
        if( ((!mSpecialAnim || (mSpecialAnim && mCancelableSpecial)) && (!AnimationHasFlag(IONA_AFLAG_NOWALKING)))
           || AnimationHasFlag(IONA_AFLAG_HASWALKING)) {
            SetDirectionFlags();
        }
        if((!mFocusMode && GetCurrentAnimID() != IONA_SWIM_BRAKE) || (GetCurrentAnimID() == IONA_SWIM_BRAKE && !mVelocity.Magnitude2())) {
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
                    SetFocus(false);
                }
            }
            else if(!mSpecialAnim || (mSpecialAnim && mCancelableSpecial)) {
                SetFocus(true);
                mShootResetTimer = 0;
            }
            if(GetKeyDown(PKEY_BUTTON2)) {
                mActionKeys[PKEY_BUTTON2].keyState = KEY_DOWNCHECKED;
                if((mGrounded || mUnderwater) && (!mSpecialAnim || (mSpecialAnim && mCancelableSpecial))) {
                    if(GetCurrentAnimID() != IONA_ROLL_A && GetCurrentAnimID() != IONA_ROLL_B) {
                        if(mGrounded) {
                            ChangeAnimIDSpecial(IONA_ROLL_A, false, 0.0);
                        }
                        else {
                            ChangeAnimIDSpecial(IONA_ROLL_B, false, 0.0);
                        }
                        float velocity = mRollSpeed;
                        if(fabs(mVelocity.x) < 1.0) {
                            velocity*= 1.5;
                        }
                        if(mFlipped) {
                            ApplyImpulseX(-velocity);
                        }
                        else {
                            ApplyImpulseX(velocity);
                        }
                    }
                }
            }
            if(mUnderwater) {
                if(GetKeyDown(PKEY_JUMP) && !mFocusMode && (mCancelableSpecial || !mSpecialAnim) && !mUpHeld) {
                    if(mFacingLeft) {
                        mSwimAngle = PI;
                    }
                }
            }
        }
        else {
            if(GetKeyDown(PKEY_BUTTON1) && ((mSpecialAnim && mCancelableSpecial) || (!mSpecialAnim))) {
                if(mSwimVelocity.Magnitude2() > mFocusModifier * mSwimMaxVel * mSwimMaxVel) {
                    mSwimVelocity = mSwimVelocity.GetUnit() * mSwimMaxVel * mFocusModifier;
                }
                else {
                    SetFocus(true);
                    mShootResetTimer = 0;
                    if(mSkidding) {
                        mSkidding = false;
                        mSpecialAnim = false;
                    }
                }
            }
            else {
                if(mFocusTimer >= 0.5f) {
                    SetFocus(false);
                }
            }
            if(GetKeyDown(PKEY_JUMP) && !mFocusMode && (mCancelableSpecial || !mSpecialAnim)) {
                Vector2f direction;
                if(mUpHeld) {
                    direction.y = 1;
                    mFlipped = false;
                    mFacingLeft = false;
                }
                else if(mDownHeld) {
                    direction.y = -1;
                    mFlipped = true;
                    mFacingLeft = true;
                }
                if(mRightHeld) {
                    direction.x = 1;
                    mFlipped = false;
                    mFacingLeft = false;
                }
                else if(mLeftHeld) {
                    direction.x = -1;
                    mFlipped = true;
                    mFacingLeft = true;
                }
                direction = direction.GetUnit();
                mSwimAngle = atan2(direction.y, direction.x);
                ChangeAnimIDSpecial(IONA_CSCHARGE_UP, true);
            }
            else if(!GetKeyDown(PKEY_JUMP) && GetCurrentAnimID() == IONA_CSCHARGE_UP) {
                mSpecialAnim = false;
            }
            if(GetKeyDown(PKEY_BUTTON3)) {
                ChangeAnimIDSpecial(IONA_ROLL_A, false, 0.0);
                if(mFlipped) {
                    mSwimVelocity.x-=10;
                }
                else {
                    mSwimVelocity.x+=10;
                }
            }
        }
        if(mUnderwater && (GetCurrentAnimID() == IONA_CSCHARGE_UP || GetCurrentAnimID() == IONA_CSDASH)) {
            float step = RADIAN;
            if(GetCurrentAnim() == IONA_CSCHARGE_UP) {
                step*= 2;
            }
            if(mUpHeld) {
                if(mSwimAngle < PI * 0.5 && mSwimAngle > -PI * 0.5) {
                    mSwimAngle+= step;
                }
                else {
                    mSwimAngle-= step;
                }
            }
            else if(mDownHeld) {
                if(mSwimAngle > PI * 0.5 || mSwimAngle < -PI * 0.5) {
                    mSwimAngle+= step;
                }
                else {
                    mSwimAngle-= step;
                }
            }
            if(mRightHeld) {
                if(mSwimAngle > 0) {
                    mSwimAngle-=step;
                }
                else if(mSwimAngle < 0) {
                    mSwimAngle+= step;
                }
            }
            else if(mLeftHeld) {
                if(mSwimAngle > PI && mSwimAccel > -PI) {
                    mSwimAngle-= step;
                }
                if(mSwimAngle < PI && mSwimAngle < -PI) {
                    mSwimAngle+= step;
                }
            }

        }
        if(mVelocity.Magnitude2() <= 1.0f && !mFocusMode) {
            Turn();
        }
    }
    void WatershedPlayer::Charge(double dt) {
    }
    void WatershedPlayer::StopCharging() {
        if(GetCurrentAnimID() == IONA_CSCHARGE_UP || GetCurrentAnimID() == IONA_CSDASH) {
            mCharging = false;
            mCancelableSpecial = true;
            mCurrentCharge = 0;
            mChargeTimer = 0;
            mSpecialAnim = false;
        }
    }
    void WatershedPlayer::CastMagic() {
        if(!mSpecialAnim || (mSpecialAnim && mCancelableSpecial)) {
            if(AnimationHasFlag(IONA_AFLAG_MAGIC)) {
                if(mSwimming) {
                    mSwimVelocity.x*= 0.8;
                    if(fabs(mSwimVelocity.x) < 0.5) {
                        mSwimVelocity.x = 0;
                    }
                }
                else {
                    mVelocity.x*= 0.8;
                    if(fabs(mVelocity.x) < 0.5) {
                        mVelocity.x = 0;
                    }
                    if(!mGrounded && !mUnderwater && mVelocity.y < 0) {
                        mVelocity.y*=0.9;
                    }
                }
            }
            if(mUnderwater) {
                if(!AnimationHasFlag(IONA_AFLAG_MAGIC)) {
                    ChangeAnimIDSpecial(IONA_CASTING_B, true);
                }
                else if(mCastTimer >= mMaxCastTime && mActionKeys[PKEY_SHOOT].keyState == KEY_RELEASED) {
                    if(mSwimming) {
                        EndMagicCast();
                    }
                    else {
                        ChangeAnimation(IONA_MERMAID_IDLE, 0.1);
                    }
                }
                else {
                    ChangeAnimIDSpecial(IONA_SPELL_B, true);
                }
                if(GetCurrentAnimID() == IONA_CASTING_B) {
                    TransitionToAnimation(IONA_CASTING_A, 0.025, GetCurrentAnim().GetCurFrameNum(), 5);
                }
            }
            else if (mGrounded) {
                if(!AnimationHasFlag(IONA_AFLAG_MAGIC)) {
                    ChangeAnimIDSpecial(IONA_CASTING_A, true);
                }
                if(AnimationHasFlag(IONA_AFLAG_CASTING)) {
                    if(mCastTimer >= mMaxCastTime && mActionKeys[PKEY_SHOOT].keyState == KEY_RELEASED) {
                        EndMagicCast();
                    }
                    else {
                        ChangeAnimIDSpecial(IONA_SPELL_A, true);
                    }
                }
            }
            else {
                if(!AnimationHasFlag(IONA_AFLAG_MAGIC)) {
                    ChangeAnimIDSpecial(IONA_CASTING_AIR, true);
                }
                else if(mCastTimer >= mMaxCastTime) {
                    EndMagicCast();
                }
                else {
                    ChangeAnimIDSpecial(IONA_SPELL_AIR, true);
                }
            }
        }
    }
    void WatershedPlayer::EndMagicCast() {
        int spellToCast = -1;
        if(mCastTimer >= mMaxCastTime) {
            spellToCast = MAG_LVL_3;
        }
        else if(mCastTimer >= mMaxCastTime * 0.66) {
            spellToCast = MAG_LVL_2;
        }
        else if(mCastTimer >= mMaxCastTime * 0.33) {
            spellToCast = MAG_LVL_1;
        }
        if(spellToCast >= 0) {
            mCastTimer = 0;
            mSpecialAnim = false;
            if(mFins) {
                ChangeAnimIDSpecial(IONA_SPELL_B, false);
            }
            else if(mGrounded) {
                ChangeAnimIDSpecial(IONA_SPELL_A, false);
            }
            else {
                ChangeAnimIDSpecial(IONA_SPELL_AIR, false);
            }
            if(mFlipped) {
                //mMagicTarget = mHitBox.GetPosition() + Vector2f(-90.0, 250.0);
            }
            else {
                //mMagicTarget = mHitBox.GetPosition() + Vector2f(90.0, 250.0);
            }
            mMagic.mSpawnPosition = mMagicTarget;
            mMagic.CastSpell(MagicCasterLevel(spellToCast));
        }
    }
    void WatershedPlayer::InitHitBox(Vector2f pos, float h, float w, int drawFrom) {
        mHitBox.Init(pos, h, w, (DrawPoint)drawFrom);
        mHitBoxHeight = h;
        mRollHeight = h * 0.5;
    }
    void WatershedPlayer::CreateMeleeSlashes() {
        mMeleeBulletSpawner.mSpawnPosition = mHitBox.GetCenter();
        if(mMeleeBulletSpawner.HasFinisher()) {
            mMeleeBulletSpawner.SetPositions(mMagicTarget);
        }
        else {
            Vector2f offset;
            offset.y = 30;
            if(mFlipped) {
                offset.x = -30;
            }
            else {
                offset.x = 30;
            }
            mMeleeBulletSpawner.SetPositions(mHitBox.GetCenter() + offset);
        }
        if(!mMeleeSlashCreated) {
            switch(GetCurrentAnimID()) {
            case IONA_SEASICKLE_1A:
            case IONA_SEASICKLE_1B:
                if(GetCurrentFrameNum() == 3) {
                    mMeleeBulletSpawner.CreateSlash(0, mFlipped);
                    mMeleeSlashCreated = true;
                }
                break;
            case IONA_SEASICKLE_2A:
            case IONA_SEASICKLE_2B:
                if(GetCurrentFrameNum() == 5) {
                    mMeleeBulletSpawner.CreateSlash(1, mFlipped);
                    mMeleeSlashCreated = true;
                }
                break;
            case IONA_SEASICKLE_3A:
            case IONA_SEASICKLE_3B:
                if(GetCurrentFrameNum() == 6) {
                    mMeleeBulletSpawner.CreateSlash(2, mFlipped);
                    mMeleeSlashCreated = true;
                }
                break;
            case IONA_SEASICKLE_AIR:
            case IONA_SEASICKLE_AIR_FINS:
                if(GetCurrentFrameNum() == 6) {
                    mMeleeBulletSpawner.CreateSlash(3, mFlipped);
                    mMeleeSlashCreated = true;
                }
                else if(GetCurrentFrameNum() == 12) {
                    mMeleeBulletSpawner.CreateSlash(4, mFlipped);
                    mMeleeSlashCreated = true;
                }
                break;
            case IONA_SEASICKLE_CHARGEA:
            case IONA_SEASICKLE_CHARGEB:
            case IONA_SEASICKLE_CHARGEAIR:
                if(GetCurrentFrameNum() == 6) {
                    mMeleeBulletSpawner.CreateSlash(5, mFlipped);
                    mMeleeSlashCreated = true;
                }
                break;
            default:
                break;
            }
        }
    }
    void WatershedPlayer::MeleeCombo() {
        mMeleeBulletSpawner.mSpawnPosition = mHitBox.GetCenter();
        if(!mSpecialAnim || (mSpecialAnim && mCancelableSpecial)) {
            if(fabs(mVelocity.x) > 0.1) {
                mVelocity.x*= 0.8;
            }
            else {
                mVelocity.x = 0;
            }
            if(mSwimming) {
                mSwimVelocity.x*= 0.8;
                mSwimVelocity.y*= 0.8;
            }
        }
        if(mMeleeBulletSpawner.HasFinisher()) {
            return;
        }
        if(mUnderwater) {
            if(abs(GetCurrentAnimID()) != IONA_SEASICKLE_1B) {
                if(GetCurrentAnimID() != IONA_SEASICKLE_2B && GetCurrentAnimID() != IONA_SEASICKLE_3B) {
                    ChangeAnimIDSpecial(IONA_SEASICKLE_1B, false);
                }
                else if(GetCurrentAnimID() == IONA_SEASICKLE_2B && GetCurrentAnim().GetCurFrameNum() >= 7) {
                    mCancelableSpecial = true;
                    ChangeAnimIDSpecial(IONA_SEASICKLE_3B, false);
                }
            }
            else if(GetCurrentAnim().GetCurFrameNum() >= 8) {
                mCancelableSpecial = true;
                ChangeAnimIDSpecial(IONA_SEASICKLE_2A, false);
            }
        }
        else if(mGrounded) {
            if(abs(GetCurrentAnimID()) != IONA_SEASICKLE_1A) {
                if(GetCurrentAnimID() != IONA_SEASICKLE_2A && GetCurrentAnimID() != IONA_SEASICKLE_3A) {
                    ChangeAnimIDSpecial(IONA_SEASICKLE_1A, false);
                }
                else if(GetCurrentAnimID() == IONA_SEASICKLE_2A && GetCurrentAnim().GetCurFrameNum() >= 7) {
                    mCancelableSpecial = true;
                    ChangeAnimIDSpecial(IONA_SEASICKLE_3A, false);
                }
            }
            else if(GetCurrentAnim().GetCurFrameNum() >= 8) {
                mCancelableSpecial = true;
                ChangeAnimIDSpecial(IONA_SEASICKLE_2A, false);
            }
        }
        else {
            if(mFins) {
                ChangeAnimIDSpecial(IONA_SEASICKLE_AIR_FINS, false);
            }
            else {
                ChangeAnimIDSpecial(IONA_SEASICKLE_AIR, false);
            }
        }
    }
    void WatershedPlayer::MeleeFinisher() {
    }
    void WatershedPlayer::SetFins(bool fins) {
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
        if(!obj) {
            return;
        }
        if(obj->IsType(OBJECT_BARREL)) {
            Barrel* b = static_cast<Barrel*>(obj);
            b->KnockOver();
        }
        mHeldObject = obj;
        mHeldObject->SetAngle(0.0f);
        mHeldObject->KillRotVel();
        mHeldObject->Stop();
        mHeldObject->SetNoFloat(true);
        mLassoChain.Attach(obj);
        mWaterHook.Destroy();
    }
    void WatershedPlayer::ThrowObject() {
        if(mHeldObject && mObjectPulled) {
            mHeldObject->Stop();
            Vector2f thr = mThrowStrength;
            if(mSwimming) {
                thr.y = 0;
            }
            if(!mFlipped) {
                mHeldObject->ApplyImpulse(mThrowStrength);
                mLassoChain.GetBack().ApplyImpulse(thr * 4.0f);
                mHeldObject->ApplyRotImpulse(RADIAN * 8);
            }
            else {
                thr.x = -thr.x;
                mHeldObject->ApplyImpulse(thr);
                mLassoChain.GetBack().ApplyImpulse(thr * 4.0f);
                mHeldObject->ApplyRotImpulse(-RADIAN * 8);
            }
            mSpecialAnim = true;
            mShootResetTimer = 10;
            ReleaseHeldObject();
        }
    }
    void WatershedPlayer::ReleaseHeldObject() {
        if(mHeldObject) {
            mHeldObject->ApplyImpulse(mVelocity);
            mHeldObject->SetZOffset(0.0f);
            mHeldObject->SetNoPlatform(false);
            mHeldObject->SetNoFloat(false);
            mObjectPulled = false;
            mPrevHeldObject = mHeldObject;
        }
        mHeldObject = NULL;
        mLassoChain.Attach(NULL);
        mReleasedObjFrames = 0;
        mGrabObjDelay = 0;
    }
    void WatershedPlayer::UpdateHeldObject() {
        if(mHeldObject) {
            if(AnimHasFlag(IONA_AFLAG_TURNING)) {
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
        if((mSpecialAnim && !mCancelableSpecial) || mModel.GetAnimations().empty()) {
            return;
        }
        int forceFrame = -1;
        if(mFins) {
            switch(i) {
            case ANIM_JUMP:
                i = IONA_FIN_JUMP;
                if(GetCurrentAnimID() == ANIM_JUMP) {
                    forceFrame = GetCurrentFrameNum();
                    transitionDur = 0.0;
                }
                break;
            case ANIM_FALL:
                i = IONA_FIN_FALL;
                if(GetCurrentAnimID() == ANIM_FALL) {
                    forceFrame = GetCurrentFrameNum();
                    transitionDur = 0.0;
                }
                break;
            case IONA_SHOOT_JUMP:
                i = IONA_SHOOT_SWIM_JUMP;
                if(GetCurrentAnimID() == IONA_SHOOT_JUMP) {
                    forceFrame = GetCurrentFrameNum();
                    transitionDur = 0.0;
                }
                break;
            case IONA_SHOOT_BACK_JUMP:
                    i = IONA_SHOOT_SWIM_JUMP_BACK;
                    if(GetCurrentAnimID() == IONA_SHOOT_BACK_JUMP) {
                        forceFrame = GetCurrentFrameNum();
                        transitionDur = 0.0;
                    }
                    break;
            case IONA_SHOOT_JUMP_UP:
                i = IONA_SHOOT_SWIM_JUMP_UP;
                if(GetCurrentAnimID() == IONA_SHOOT_SWIM_JUMP_UP) {
                    forceFrame = GetCurrentFrameNum();
                    transitionDur = 0.0;
                }
                break;
            case IONA_SHOOT_JUMP_DOWN:
                    i = IONA_SHOOT_SWIM_JUMP_DOWN;
                    if(GetCurrentAnimID() == IONA_SHOOT_JUMP_DOWN) {
                        forceFrame = GetCurrentFrameNum();
                        transitionDur = 0.0;
                    }
                    break;
            default:
                break;
            }

        }
        if(IsChargingMagic()) {
            switch(i) {
                case ANIM_JUMP:
                case ANIM_FALL:
                    i = IONA_CASTING_AIR;
                    if(GetCurrentAnimID() == ANIM_JUMP || GetCurrentAnimID() == ANIM_FALL) {
                        transitionDur = 0.0;
                    }
                    break;
                case ANIM_IDLE:
                    i = IONA_CASTING_A;
                    if(GetCurrentAnimID() == ANIM_IDLE) {
                        transitionDur = 0.0;
                    }
                    break;
                case IONA_FIN_FALL:
                case IONA_FIN_JUMP:
                case IONA_SWIM_IDLE:
                case IONA_MERMAID_IDLE:
                    i = IONA_CASTING_B;
                    break;
                default:
                    break;
            }
        }
        if(GetCurrentAnimID() != i) {
            mMeleeSlashCreated = false;
        }
        else {
            return;
        }
        if(mFins && mModel.GetAnimation(mAnimationCache[i])->GetName().back() == 'A') {
            std::string name = mModel.GetAnimation(mAnimationCache[i])->GetName();
            name.back() = 'B';
            i = mModel.GetAnimationID(name);
            ChangeAnimation(i, transitionDur);
            if(forceFrame > -1) {
                mModel.GetCurrentAnim()->ForceFrame(forceFrame);
            }
            return;

        }
        else if(!mFins && mModel.GetAnimation(mAnimationCache[i])->GetName().back() == 'B') {
            std::string name = mModel.GetAnimation(mAnimationCache[i])->GetName();
            name.back() = 'A';
            i = mModel.GetAnimationID(name);
            ChangeAnimation(i, transitionDur);
            if(forceFrame > -1) {
                mModel.GetCurrentAnim()->ForceFrame(forceFrame);
            }
            return;
        }
        else if(i != abs(mModel.GetCurrentAnimID())) {
            if(AnimHasFlag(IONA_AFLAG_TURNING && IsShooting())) {
                mShellModel.ChangeAnimation(1, 0.0);
            }
            if(transitionDur < 0.001f) {
                mRecoiling = false;
            }
            if(i < mAnimationCache.size()) {
                ChangeAnimation(mAnimationCache[i], transitionDur);
                if(forceFrame > -1) {
                    mModel.GetCurrentAnim()->ForceFrame(forceFrame);
                }
            }
        }
    }
    void WatershedPlayer::ChangeAnimation(int i, float transitionDur) {
        if(i < 0) {
            return;
        }
        mModel.ChangeAnimation(i, transitionDur);
    }
    void WatershedPlayer::ChangeAnimation(string anim) {
        mModel.ChangeAnimation(anim);
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
        return (mActiveFlags & IONA_AFLAG_SHOOTING) || mShootResetTimer;
    }
    bool WatershedPlayer::UsesFocusAnims() {
        return (mFocusMode || (mHeldObject && mObjectPulled) || IsShooting()) && !(mSpecialAnim && !mCancelableSpecial);
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
        mAnimationCache[i++] = mModel.SetAnimID("DShootA", IONA_SHOOT_DOWN);
        mAnimationCache[i++] = mModel.SetAnimID("UShootA", IONA_SHOOT_UP);
        mAnimationCache[i++] = mModel.SetAnimID("FShootFJumpA", IONA_SHOOT_JUMP);
        mAnimationCache[i++] = mModel.SetAnimID("FShootRJumpA", IONA_SHOOT_BACK_JUMP);
        mAnimationCache[i++] = mModel.SetAnimID("UShootFJumpA", IONA_SHOOT_JUMP_UP);
        mAnimationCache[i++] = mModel.SetAnimID("DShootFJumpA", IONA_SHOOT_JUMP_DOWN);

        mAnimationCache[i++] = mModel.SetAnimID("FShootFRunA", IONA_RUNSHOOT_FORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("FShootRRunA", IONA_RUNSHOOT_BACK);
        mAnimationCache[i++] = mModel.SetAnimID("UShootFRunA", IONA_RUNSHOOT_UFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("DShootFRunA", IONA_RUNSHOOT_DFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("UShootRRunA", IONA_RUNSHOOT_UBACK);
        mAnimationCache[i++] = mModel.SetAnimID("DShootRRunA", IONA_RUNSHOOT_DBACK);

        mAnimationCache[i++] = mModel.SetAnimID("FShootB", IONA_SWIMSHOOT_IDLE);
        mAnimationCache[i++] = mModel.SetAnimID("FShootFSwimB", IONA_SWIMSHOOT_FORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("FShootRSwimB", IONA_SWIMSHOOT_BACK);
        mAnimationCache[i++] = mModel.SetAnimID("FShootUSwimB", IONA_SWIMSHOOT_UP);
        mAnimationCache[i++] = mModel.SetAnimID("FShootDSwimB", IONA_SWIMSHOOT_DOWN);
        mAnimationCache[i++] = mModel.SetAnimID("UShootB", IONA_SWIMSHOOT_IDLE_UP);
        mAnimationCache[i++] = mModel.SetAnimID("DShootB", IONA_SWIMSHOOT_IDLE_DOWN);

        mAnimationCache[i++] = mModel.SetAnimID("UShootFSwimB", IONA_SWIMSHOOT_UFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("DShootFSwimB", IONA_SWIMSHOOT_DFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("UShootRSwimB", IONA_SWIMSHOOT_UBACK);
        mAnimationCache[i++] = mModel.SetAnimID("DShootRSwimB", IONA_SWIMSHOOT_DBACK);

        mAnimationCache[i++] = mModel.SetAnimID("FShootFJumpB", IONA_SHOOT_SWIM_JUMP);
        mAnimationCache[i++] = mModel.SetAnimID("FShootRJumpB", IONA_SHOOT_SWIM_JUMP_BACK);
        mAnimationCache[i++] = mModel.SetAnimID("UShootFJumpB", IONA_SHOOT_SWIM_JUMP_UP);
        mAnimationCache[i++] = mModel.SetAnimID("DShootFJumpB", IONA_SHOOT_SWIM_JUMP_DOWN);

        mAnimationCache[i++] = mModel.SetAnimID("DShootUSwimB", IONA_SHOOT_USWIM_DOWN);
        mAnimationCache[i++] = mModel.SetAnimID("UShootUSwimB", IONA_SHOOT_USWIM_UP);
        mAnimationCache[i++] = mModel.SetAnimID("DShootDSwimB", IONA_SHOOT_DSWIM_UP);
        mAnimationCache[i++] = mModel.SetAnimID("DShootDSwimB", IONA_SHOOT_DSWIM_DOWN);

        mAnimationCache[i++] = mModel.SetAnimID("FShootFRecoilA", IONA_SHOOT_RECOIL);
        mAnimationCache[i++] = mModel.SetAnimID("DShootDRecoilA", IONA_SHOOT_RECOIL_DOWN);
        mAnimationCache[i++] = mModel.SetAnimID("UShootURecoilA", IONA_SHOOT_RECOIL_UP);
        mAnimationCache[i++] = mModel.SetAnimID("UShootURecoilA", IONA_SHOOT_RECOIL_DOWNFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("UShootURecoilA", IONA_SHOOT_RECOIL_UPFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("UShootURecoilA", IONA_SHOOT_RECOIL_UPBACK);
        mAnimationCache[i++] = mModel.SetAnimID("UShootURecoilA", IONA_SHOOT_RECOIL_DOWNBACK);
        mAnimationCache[i++] = mModel.SetAnimID("FShootFRecoilB", IONA_SHOOT_RECOIL_FINS);
        mAnimationCache[i++] = mModel.SetAnimID("DShootDRecoilB", IONA_SHOOT_RECOIL_DOWN_FINS);
        mAnimationCache[i++] = mModel.SetAnimID("UShootURecoilB", IONA_SHOOT_RECOIL_UP_FINS);

        mAnimationCache[i++] = mModel.SetAnimID("SwimTurnB", IONA_SWIM_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("SwimBrakeRecB", IONA_SWIM_BRAKE_RECOV);
        mAnimationCache[i++] = mModel.SetAnimID("DShootFRunTurnA", IONA_DSHOOT_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("UShootFRunTurnA", IONA_USHOOT_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("RunTurnA", IONA_RUN_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("RunBrakeA", IONA_IDLE_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("FShootFRunTurnA", IONA_SHOOT_BRAKE);
        mAnimationCache[i++] = mModel.SetAnimID("UShootFRunTurnA", IONA_SHOOT_BRAKE_UFORWARD);
        mAnimationCache[i++] = mModel.SetAnimID("DShootFRunTurnA", IONA_SHOOT_BRAKE_DFORWARD);
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
        mAnimationCache[i++] = mModel.SetAnimID("Hitstun2A", IONA_HITSTUN2A);
        mAnimationCache[i++] = mModel.SetAnimID("Hitstun2B", IONA_HITSTUN2B);
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

        mAnimationCache[i++] = mModel.SetAnimID("CSCChargeUpB", IONA_CSCHARGE_UP);
        mAnimationCache[i++] = mModel.SetAnimID("CSCDashB", IONA_CSDASH);
        mAnimationCache[i++] = mModel.SetAnimID("CSCImpactB", IONA_CSIMPACT);

        mAnimationCache[i++] = mModel.SetAnimID("CastingA", IONA_CASTING_A);
        mAnimationCache[i++] = mModel.SetAnimID("CastingB", IONA_CASTING_B);
        mAnimationCache[i++] = mModel.SetAnimID("AirCastingA", IONA_CASTING_AIR);
        mAnimationCache[i++] = mModel.SetAnimID("SpellAttack1A", IONA_SPELL_A);
        mAnimationCache[i++] = mModel.SetAnimID("SpellAttack2A", IONA_SPELL_A2);
        mAnimationCache[i++] = mModel.SetAnimID("SpellAttack1B", IONA_SPELL_B);
        mAnimationCache[i++] = mModel.SetAnimID("SpellAttack2B", IONA_SPELL_B2);
        mAnimationCache[i++] = mModel.SetAnimID("AirSpellAttack1A", IONA_SPELL_AIR);

        mAnimationCache[i++] = mModel.SetAnimID("FRollA", IONA_ROLL_A);
        mAnimationCache[i++] = mModel.SetAnimID("FRollB", IONA_ROLL_B);

        mAnimationCache[i++] = mModel.SetAnimID("SeaSickle1A", IONA_SEASICKLE_1A);
        mAnimationCache[i++] = mModel.SetAnimID("SeaSickle2A", IONA_SEASICKLE_2A);
        mAnimationCache[i++] = mModel.SetAnimID("SeaSickle3A", IONA_SEASICKLE_3A);
        mAnimationCache[i++] = mModel.SetAnimID("SeaSickle1B", IONA_SEASICKLE_1B);
        mAnimationCache[i++] = mModel.SetAnimID("SeaSickle2B", IONA_SEASICKLE_2B);
        mAnimationCache[i++] = mModel.SetAnimID("SeaSickle3B", IONA_SEASICKLE_3B);

        mAnimationCache[i++] = mModel.SetAnimID("AirSickleA", IONA_SEASICKLE_AIR);
        mAnimationCache[i++] = mModel.SetAnimID("AirSickleB", IONA_SEASICKLE_AIR_FINS);
        mAnimationCache[i++] = mModel.SetAnimID("ChargeSickle1A", IONA_SEASICKLE_CHARGEA);
        mAnimationCache[i++] = mModel.SetAnimID("ChargeSickle1B", IONA_SEASICKLE_CHARGEB);
        mAnimationCache[i++] = mModel.SetAnimID("AirChargeSickle1A", IONA_SEASICKLE_CHARGEAIR);

        mAnimationCache[i++] = mModel.SetAnimID("ReverseRunaA", IONA_REVERSE_RUN);

        for(size_t i = 0; i < mModel.GetAnimations().size(); i++) {
            std::string name = mModel.GetAnimation(i)->GetName();
            if(name.back() == 'B') {
                mModel.GetAnimation(i)->AddFlags(IONA_AFLAG_FINS);
            }
            if(name.find("Shoot") != string::npos) {
                mModel.GetAnimation(i)->AddFlags(IONA_AFLAG_SHOOTING);
            }
        }

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
                         mModel.GetTransform().GetTranslation().z);
    }
    bool WatershedPlayer::IsChargingMagic() {
        if(mEquippedSpellType == SPELL_MAGIC && (mActionKeys[PKEY_SHOOT].keyState == KEY_DOWN || mActionKeys[PKEY_SHOOT].keyState == KEY_DOWNCHECKED)) {
            return true;
        }
        return false;
    }
}
