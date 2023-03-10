#include "enemybase.h"

namespace solstice {
EnemyBase::EnemyBase() {
    mActiveBehavior = 0;
    mNoTorque = true;
    mObjectType = OBJECT_ENEMY;
    mActiveBehavior = mDefaultBehavior;
    mCanShoot = true;
    mControlMode = false;
    mHurling = false;
    InitAnimRefs();
}
EnemyBase::EnemyBase(PhysicsBox2f physbox) {
    mNoTorque = true;
    mActiveBehavior = 0;
    mObjectType = OBJECT_ENEMY;
    mActiveBehavior = mDefaultBehavior;
    mCanShoot = true;
    mControlMode = false;
    mHurling = false;
    InitAnimRefs();
}
EnemyBase::EnemyBase(PhysicsBox2f physbox, std::vector<std::shared_ptr<Behavior> >& behaviors) {
    mNoTorque = true;
    mBehaviors = behaviors;
    mActiveBehavior = 0;
    mObjectType = OBJECT_ENEMY;
    mActiveBehavior = mDefaultBehavior;
    mCanShoot = true;
    mControlMode = false;
    mHurling = false;
    InitAnimRefs();
}
void EnemyBase::SetModel(int id, ModelAnimationset& animSet) {
    mModelID = id;
    mAnimationSet = animSet;
}
void EnemyBase::InitAnimRefs() {
    for(size_t i = 0; i < ENEMY_ANIM_MAX; i++) {
        mAnimationRefs[i].second = -1;
    }
    mAnimationRefs[ENEMY_ANIM_IDLE].first = "Idle1";
    mAnimationRefs[ENEMY_ANIM_WALK].first = "Walk";
    mAnimationRefs[ENEMY_ANIM_JUMP].first = "Jump";
    mAnimationRefs[ENEMY_ANIM_HITSTUN].first = "Hitstun";
    mAnimationRefs[ENEMY_ANIM_HURL].first = "Hitstun";
    mAnimationRefs[ENEMY_ANIM_IDLE_SWIM].first = "Idle2";
    mAnimationRefs[ENEMY_ANIM_SWIM].first = "Swim";
    mAnimationRefs[ENEMY_ANIM_KNOCKDOWN_1].first = "Knockdown1";
    mAnimationRefs[ENEMY_ANIM_KNOCKDOWN_2].first = "Knockdown2";
    mAnimationRefs[ENEMY_ANIM_GETUP_1].first = "Getup1";
    mAnimationRefs[ENEMY_ANIM_GETUP_2].first = "Getup2";
    mAnimationRefs[ENEMY_ANIM_GRABBED].first = "Grabbed";
}
void EnemyBase::InitPhysbox(PhysicsBox2f& box) {

}
void EnemyBase::Update(double dt) {
    if(GetSubmergeRatio() <= 0) {
        ChangeState(MOVE_GROUNDED);
    }
    else if(GetSubmergeRatio() <= 1.25) {
        ChangeState(MOVE_SWIMMING_SURFACE);
    }
    else {
        ChangeState(MOVE_SWIMMING_UNDERWATER);
    }
    if(mHurling) {
        ChangeBehavior(-1);
        if(mAnimationSet.CurrentAnimFinished()) {
            //mAnimationSet.GetCurrentAnim()->Reset();
        }
        if(mGrounded || mUnderwater && mVelocity.y <= 0) {
            mHurling = false;
        }
    }
    ApplyBehavior(dt);
    CheckAnimations();
    PhysicsBox2f::Update(dt);
    if(GetVelocity().x < 0) {
        mTargetAngle = -fabs(mBasePivotAngle);
    }
    else if(GetVelocity().x > 0) {
        mTargetAngle = fabs(mBasePivotAngle);
    }
    mTargetRange.SetPosition(mHitBox.GetPosition());
    UpdateTransforms(GetPosition() + mModelOffset, mHitBox.GetAngle());
    mAnimationSet.Update(dt);
    if(mAnimationSet.GetCurrentAnim() && (mAnimationSet.GetCurrentAnim()->Finished() && mAnimationSet.GetCurrentAnimID() > -1)) {
        ChangeAnimByType(ENEMY_ANIM_IDLE);
    }
    PivotTowardsTarget();
}
void EnemyBase::Hurt(Vector2f knockback, int damage, bool hurls) {
    ApplyImpulse(knockback);
    ChangeBehavior(-1);
    if(!hurls) {
        ChangeAnimByType(ENEMY_ANIM_HITSTUN);
        mAnimationSet.GetCurrentAnim()->Reset();
    }
    else {
        ChangeAnimByType(ENEMY_ANIM_HURL);
        mAnimationSet.GetCurrentAnim()->Reset();
    }
    if(knockback.x < 0) {
        mPivotAngle = fabs(mBasePivotAngle);
    }
    else {
        mPivotAngle = -fabs(mBasePivotAngle);
    }
    mHurling = hurls;
    mTargetAngle = mPivotAngle;
}
void EnemyBase::CheckAnimations() {
    if(mAnimationSet.GetCurrentAnimID() != -1) {
        if((mAnimationSet.GetCurrentAnim()->mPriority <= 1 || mAnimationSet.CurrentAnimFinished()) && mAnimationSet.GetCurrentAnimID() > -1) {
            if(!mUnderwater) {
                if(mGrounded) {
                    if(mVelocity.x != 0) {
                        ChangeAnimByType(ENEMY_ANIM_WALK);
                    }
                    else {
                        ChangeAnimByType(ENEMY_ANIM_IDLE);
                    }
                }
                else if(GetVelocity2().y != 0.0) {
                    ChangeAnimByType(ENEMY_ANIM_JUMP);
                }
            }
            else {
                if(mVelocity.x != 0 || (!mVelocity.x && mActiveBehavior->mMotion.GetVelocity().x)) {
                    ChangeAnimByType(ENEMY_ANIM_SWIM);
                }
                else {
                    ChangeAnimByType(ENEMY_ANIM_IDLE_SWIM);
                }
            }
        }
    }
}
void EnemyBase::ChangeAnimByType(EnemyAnimationType t) {
    if(mUnderwater && t == ENEMY_ANIM_IDLE) {
        t = ENEMY_ANIM_IDLE_SWIM;
    }
    if(!mAnimationRefs[t].first.empty() && mAnimationRefs[t].second == -1) {
        mAnimationRefs[t].second = mAnimationSet.GetAnimationID(mAnimationRefs[t].first);
    }
    if(mAnimationRefs[t].second > -1) {
        mAnimationSet.ChangeAnimation(mAnimationRefs[t].second);
    }
}
void EnemyBase::ApplyBehavior(double dt) {
    if(mActiveBehavior) {
        Behavior& b = *mActiveBehavior;
        if(b.mMotion.mType == ENMOTION_STOPS) {
            Stop();
        }
        if(b.IsBehavior(BEHAVIOR_SHOOT)) {
            b.mDestination = mHitBox.GetCenter();
        }
        else {
            b.mDestination = mTarget;
        }
        ApplyState(b.mState);
        b.Update(dt, this);

        if(mActiveBehavior->Finished()) {
            mActiveBehavior->Reset();
            mLastBehavior = mActiveBehavior;
            if(mActiveBehaviorID == -1 && !mControlMode) {
                OnNotify(*this, SOL_EVENT_IDLE);
            }
            else {
                if(mNextBehavior) {
                    mActiveBehavior = mNextBehavior;
                }
                else {
                    mDefaultBehavior->Reset();
                    mActiveBehavior = mDefaultBehavior;
                }
            }
            mNextBehavior = nullptr;
        }
    }
    else {
        mActiveBehavior = mDefaultBehavior;
        mActiveBehavior->Reset();
        mActiveBehaviorID = -1;
    }
}
void EnemyBase::ApplyState(ActorState state) {
    if(mActiveBehavior) {
        if(!mActiveBehavior->mState.mAnimationRef.empty() && mActiveBehavior->mState.mAnimationID >= 0) {
            if(!mActiveBehavior->mState.mAnimationRef.empty() && mActiveBehavior->mState.mAnimationID < 0) {
                mActiveBehavior->mState.mAnimationID = mAnimationSet.GetAnimationID(mActiveBehavior->mState.mAnimationRef);
            }
            if((mAnimationSet.GetCurrentAnim()->Finished() && mAnimationSet.GetCurrentAnim()->GetID() != -mActiveBehavior->mState.mAnimationID) || mAnimationSet.GetCurrentAnim()->GetLoops() == -1) {
                mAnimationSet.ChangeAnimation(mActiveBehavior->mState.mAnimationID);
            }
        }
    }
    ApplyForce(state.mMotion.mAcceleration);
    ApplyImpulse(state.mMotion.mVelocity);
}
void EnemyBase::CheckAndResolveCollision(BackgroundTile2& ground) {
    if(ground.GetType() == TILE_PLATFORM && mNoPlatform) {
        return;
    }
    Collision c = solstice::CheckCollision(&mHitBox, &ground.GetHitBox());
    if(c && GetInvMass()) {
        mCollisionFound = true;
        CheckCollisions(c.normal);
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
            if(mActiveBehavior && mActiveBehavior->IsBehavior(ENEMYBEHAVIOR_WALK)) {
                if(((c.mtv.x < 0 && velocity.x > 0) || (c.mtv.x > 0 && velocity.x < 0)) && mActiveBehavior->mTurnsOnCollision && !mControlMode) {
                    mActiveBehavior->Reverse();
                }
            }
            ResolveCollision(c, 1.0f, vel);
        }
        else if(ground.GetType() == TILE_PLATFORM) {
            CheckPlatform(c, ground);
        }
        else if(ground.GetType() == TILE_WATER) {
            SetUnderwater(true);
        }
    }
}
void EnemyBase::ChangeBehavior(int i) {
    if(i > -1 && i < mBehaviors.size()) {
        if(!mActiveBehavior) {
            mActiveBehavior = mBehaviors[i];
        }
        if(i != mActiveBehaviorID) {
            if(mActiveBehavior->mPriority <= mBehaviors[i]->mPriority) {
                mActiveBehavior->Reset();
                mActiveBehaviorID = i;
                mActiveBehavior = mBehaviors[i];
                if(mActiveBehavior->mMotion.mStopsMomentum) {
                    StopX();
                }
            }
        }
    }
    else if(mActiveBehaviorID != -1 && (mActiveBehavior && mActiveBehavior->mPriority <= 1)) {
        mActiveBehaviorID = -1;
        mActiveBehavior = mDefaultBehavior;
        mActiveBehavior->Reset();
        if(mActiveBehavior->mMotion.mStopsMomentum) {
            StopX();
        }
    }
}
void EnemyBase::ForceBehavior(int i) {
    if(i > -1 && i < mBehaviors.size()) {
        if(mActiveBehavior && i != mActiveBehaviorID) {
            mActiveBehavior->Reset();
            mActiveBehaviorID = i;
            mActiveBehavior = mBehaviors[i];
        }
    }
    else if (i == -1) {
        mActiveBehavior = mDefaultBehavior;
    }
}
std::shared_ptr<EnemyBase> EnemyBase::Clone() {
    return std::make_shared<EnemyBase>(*this);
}
void EnemyBase::OnNotify(const GameObject& entity, SolsticeEvent event) {
    /*
    switch(event) {
    case SOL_EVENT_GRABBED:
        break;
    case SOL_EVENT_THROWN:
        break;
    case SOL_EVENT_HURLED:
        break;
    case SOL_EVENT_DAMAGED:
        break;
    case SOL_EVENT_LOWHEALTH:
        break;
    case SOL_EVENT_DESTROYED:
        break;
    case SOL_EVENT_ALLY_DESTROYED:
        break;
    case SOL_EVENT_LEADER_DESTROYED:
        break;
    case SOL_EVENT_RALLY:
        break;
    case SOL_EVENT_WATERTOUCH:
        break;
    case SDL_EVENT_WATERSPOTTED:
        break;
    case SOL_EVENT_GROUNDTOUCH:
        break;
    case SOL_EVENT_WALLTOUCH:
        break;
    case SOL_EVENT_ALLYTOUCH:
        break;
    case SOL_EVENT_ENEMYTOUCH:
        break;
    case SOL_EVENT_KNOCKDOWN:
        break;
    case SOL_EVENT_ALARM_ENABLED:
        break;
    case SOL_EVENT_ALARM_DISABLED:
        break;
    case SOL_EVENT_ALLY_NEAR:
        break;
    case SOL_EVENT_ENEMY_NEAR:
        //OnPlayerDetect(entity);
        break;
    case SOL_EVENT_ENEMY_ESCAPED:
        OnTargetEscape(entity);
        break;
    default:
        break;
    }
    */
    ChangeBehaviorFromEvent(event);
}
void EnemyBase::OnAlarmOn(const GameObject& entity)
{

}

void EnemyBase::OnAlarmOff(const GameObject& entity)
{

}

void EnemyBase::OnPlayerDetect(const GameObject& entity) {
    auto iter = std::find_if(mBehaviors.begin(), mBehaviors.end(), FindBehaviorEvent((int)SOL_EVENT_ENEMY_NEAR));
    if(iter != mBehaviors.end()) {
        mActiveBehavior = *iter;
    }
    /*
    if(entity.IsType(OBJECT_PLAYER)) {
        ChangeBehavior(1);
    }
    */
}

void EnemyBase::OnDeath(const GameObject& entity)
{

}

void EnemyBase::OnEnemyTouch(const GameObject& entity)
{

}

void EnemyBase::OnAllyTouch(const GameObject& entity)
{

}

void EnemyBase::OnHurt(const GameObject& entity)
{

}

void EnemyBase::OnLowHealth(const GameObject& entity)
{

}

void EnemyBase::OnKnockdown(const GameObject& entity)
{

}

void EnemyBase::OnWatertouch(const GameObject& entity)
{

}

void EnemyBase::OnRally(const GameObject& entity)
{

}
void EnemyBase::OnTargetEscape(const GameObject& entity) {
    if(entity.IsType(OBJECT_PLAYER)) {
        ChangeBehavior(0);
    }
}
void EnemyBase::ChangeBehaviorFromEvent(int event) {
    SolsticeEvent ev = (SolsticeEvent)event;
    if(mEventList.mBehaviorList.count(event)) {
        auto found = mEventList.mBehaviorList.equal_range(event);
        size_t i = 0;
        for(auto iter = found.first; iter != found.second; iter++, i++) {
            if(mLastBehavior != iter->second) {
                mActiveBehavior = iter->second;
            }
            mActiveBehaviorID = i;
        }
    }
    else {
        ChangeBehavior(-1);
    }
}
bool EnemyBase::ChangeToBehavior(int behaviorType, Vector2f direction) {
    for(size_t i = 0; i < mBehaviors.size(); i++) {
        if(mBehaviors[i]->IsBehavior(behaviorType)) {
            ChangeBehavior(i);
            if(mActiveBehavior) {
                mActiveBehavior->Reset();
                mActiveBehavior->mMotion.SetDirection(direction);
            }
            return true;
        }
    }
    return false;
}
GameObject* EnemyBase::GetEntity(int e) {
    switch(e) {
        case ENTITY_SHOOTING:
            return &mShootingEntity;
            break;
        default:
            return nullptr;
            break;
    }
}
}
