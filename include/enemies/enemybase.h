#ifndef ENEMYBASE_H_INCLUDED
#define ENEMYBASE_H_INCLUDED

#include "physics/physicsrect.h"
#include "behavior/behavior.h"
#include "behavior/trackingbehavior.h"
#include "enemypath.h"
#include "graphics/3d/skeletalmodel.h"
#include "graphics/3d/modelanimset.h"
#include "graphics/renderableobject.h"
#include "engine/healthentity.h"
#include "engine/shootingentity.h"
#include "events/eventobserver.h"
#include "behavior/behavior_eventlist.h"
#include "behavior/hurlingbehavior.h"

#include <memory>
#include <utility>

namespace solstice {
enum EnemyBehaviorType {ENEMYBEHAVIOR_IDLE, ENEMYBEHAVIOR_WALK, ENEMYBEHAVIOR_JUMP,
                        ENEMYBEHAVIOR_HURT, ENEMYBEHAVIOR_KNOCKDOWN,
                        ENEMYBEHAVIOR_ATTACK, ENEMYBEHAVIOR_SHOOT, ENEMYBEHAVIOR_TRACKING,
                        ENEMYBEHAVIOR_FLEEING, ENEMYBEHAVIOR_MAX};
enum EnemyAnimationType {
    ENEMY_ANIM_IDLE, ENEMY_ANIM_WALK, ENEMY_ANIM_JUMP, ENEMY_ANIM_HITSTUN, ENEMY_ANIM_HURL,
    ENEMY_ANIM_IDLE_SWIM, ENEMY_ANIM_SWIM, ENEMY_ANIM_KNOCKDOWN_1, ENEMY_ANIM_KNOCKDOWN_2,
    ENEMY_ANIM_GETUP_1, ENEMY_ANIM_GETUP_2, ENEMY_ANIM_GRABBED,
    ENEMY_ANIM_MAX
};
class EnemyObserver : public GameObjectEventObserver {
public:
    virtual void OnAlarmOn(const GameObject& entity) = 0;
    virtual void OnAlarmOff(const GameObject& entity) = 0;
    virtual void OnPlayerDetect(const GameObject& entity) = 0;
    virtual void OnDeath(const GameObject& entity) = 0;
    virtual void OnEnemyTouch(const GameObject& entity) = 0;
    virtual void OnAllyTouch(const GameObject& entity) = 0;
    virtual void OnHurt(const GameObject& entity) = 0;
    virtual void OnLowHealth(const GameObject& entity) = 0;
    virtual void OnKnockdown(const GameObject& entity) = 0;
    virtual void OnWatertouch(const GameObject& entity) = 0;
    virtual void OnRally(const GameObject& entity) = 0;
    virtual void OnTargetEscape(const GameObject& entity) = 0;
};
class EnemyBase : public PhysicsBox2f, public EnemyObserver, public RenderableObject {
private:
    std::shared_ptr<Behavior> mActiveBehavior, mNextBehavior, mLastBehavior;
    Circlef mTargetRange;
    int mDirection;
    int mActiveBehaviorID;
    BehaviorEventList mEventList;
    int mHitstunDur;
    bool mHurling;
protected:
    virtual void ChangeBehaviorFromEvent(int event);
    virtual void CheckAnimations();
    virtual void ApplyBehavior(double dt);
    virtual void InitAnimRefs();
public:
    std::pair<std::string, int> mAnimationRefs[ENEMY_ANIM_MAX];
    std::shared_ptr<Behavior> mDefaultBehaviors[10];
    Vector2f mTarget;
    HealthEntity mHealthBar;
    std::vector<std::shared_ptr<Behavior> > mBehaviors;
    std::shared_ptr<Behavior> mDefaultBehavior;
    ShootingEntity mShootingEntity;
    bool mControlMode;
    EnemyPath mPath;
    EnemyBase();
    EnemyBase(PhysicsBox2f physbox, std::vector<std::shared_ptr<Behavior> >& behaviors);
    EnemyBase(PhysicsBox2f physbox);
    void SetModel(int id, ModelAnimationset& animSet);
    void InitPhysbox(PhysicsBox2f& box);
    void Update(double dt);
    void SetTargetRange(float r)            {mTargetRange.SetSize(r);}
    void ChangeBehavior(int i);
    void ChangeBehavior2(int event, std::unordered_map<int, std::shared_ptr<Behavior> >::iterator it);
    void ForceBehavior(int i);
    bool ChangeToBehavior(int behaviorType, Vector2f direction);
    void PerformBehavior(Behavior* b);

    virtual std::shared_ptr<EnemyBase> Clone();
    const std::shared_ptr<Behavior>& GetActiveBehavior() {return mActiveBehavior;}

    Circlef& GetTargetRange()           {return mTargetRange;}
    int GetActiveBehaviorID()           {return mActiveBehaviorID;}

    void CheckAndResolveCollision(BackgroundTile2& ground);
    void OnNotify(const GameObject& entity, SolsticeEvent event);

    virtual void OnAlarmOn(const GameObject& entity);
    virtual void OnAlarmOff(const GameObject& entity);
    virtual void OnPlayerDetect(const GameObject& entity);
    virtual void OnDeath(const GameObject& entity);
    virtual void OnEnemyTouch(const GameObject& entity);
    virtual void OnAllyTouch(const GameObject& entity);
    virtual void OnHurt(const GameObject& entity);
    virtual void OnLowHealth(const GameObject& entity);
    virtual void OnKnockdown(const GameObject& entity);
    virtual void OnWatertouch(const GameObject& entity);
    virtual void OnRally(const GameObject& entity);
    virtual void OnTargetEscape(const GameObject& entity);
    virtual void ChangeAnimByType(EnemyAnimationType t);
    //virtual void ForceAnimByType(EnemyAnimationType t);
    void SetupEventList()   {mEventList.AddBehaviors(mBehaviors);}
    GameObject* GetEntity(int e);
    bool HasEntity(int e) {return GetEntity(e);}
    virtual void ApplyState(ActorState state);
    int GetCurFrameNum() {return mAnimationSet.GetCurrentAnim() ? mAnimationSet.GetCurrentAnim()->GetCurFrameNum() : -1;}
    void SetNoTorque(bool t) {mNoTorque = true;}
    virtual void Hurt(Vector2f knocback, int damage, bool hurls = false);
};
}


#endif // ENEMYBASE_H_INCLUDED
