#ifndef PLAYERWatershed_H_INCLUDED
#define PLAYERWatershed_H_INCLUDED

#include <memory>
#include "player/baseplayer2.h"
#include "particle/particle.h"
#include "bullets/bullet.h"
#include "bullets/bullettype.h"
#include "graphics/3d/skeletalmodel.h"
#include "graphics/3d/light.h"
#include "physics/chain.h"
#include "environment/room.h"
#include "bullets/guns.h"
#include "bullets/waterhook.h"

namespace solstice {
enum WatershedPlayerCollisions {IONA_COL_NONE = 0, IONA_COL_GROUND = 1, IONA_COL_LEFT = 2, IONA_COL_RIGHT = 4, IONA_COL_TOP = 8,
                               IONA_COL_WATER = 16,
                               IONA_COL_WETSPOT = 32};
enum WatershedPlayerAnim {IONA_SWIM_IDLE = ANIM_LAND + 1, IONA_SWIM, IONA_SWIM_TWIRL, IONA_FIN_JUMP, IONA_FIN_FALL, IONA_LAND_FINS,
                          IONA_MERMAID_DIVE, IONA_MERMAID_IDLE, IONA_MERMAID_SURFACE, IONA_MERMAID_SWIM,
                          IONA_SHOOT_GROUND, IONA_SHOOT_DOWN, IONA_SHOOT_UP,
                          IONA_SHOOT_JUMP, IONA_SHOOT_BACK_JUMP, IONA_SHOOT_JUMP_UP, IONA_SHOOT_JUMP_DOWN,
                          IONA_RUNSHOOT_FORWARD, IONA_RUNSHOOT_BACK, IONA_RUNSHOOT_UFORWARD, IONA_RUNSHOOT_DFORWARD,
                          IONA_RUNSHOOT_UBACK, IONA_RUNSHOOT_DBACK,
                          IONA_SWIMSHOOT_IDLE, IONA_SWIMSHOOT_FORWARD, IONA_SWIMSHOOT_BACK, IONA_SWIMSHOOT_UP, IONA_SWIMSHOOT_DOWN,
                          IONA_SWIMSHOOT_IDLE_UP, IONA_SWIMSHOOT_IDLE_DOWN,
                          IONA_SWIMSHOOT_UFORWARD, IONA_SWIMSHOOT_DFORWARD, IONA_SWIMSHOOT_UBACK, IONA_SWIMSHOOT_DBACK,
                          IONA_SHOOT_SWIM_JUMP, IONA_SHOOT_SWIM_JUMP_BACK, IONA_SHOOT_SWIM_JUMP_UP, IONA_SHOOT_SWIM_JUMP_DOWN,
                          IONA_SHOOT_USWIM_DOWN, IONA_SHOOT_USWIM_UP, IONA_SHOOT_DSWIM_UP, IONA_SHOOT_DSWIM_DOWN,
                          IONA_SHOOT_RECOIL, IONA_SHOOT_RECOIL_DOWN, IONA_SHOOT_RECOIL_UP,
                          IONA_SHOOT_RECOIL_DOWNFORWARD, IONA_SHOOT_RECOIL_UPFORWARD, IONA_SHOOT_RECOIL_UPBACK, IONA_SHOOT_RECOIL_DOWNBACK,
                          IONA_SHOOT_RECOIL_FINS, IONA_SHOOT_RECOIL_DOWN_FINS, IONA_SHOOT_RECOIL_UP_FINS,
                          IONA_SWIM_BRAKE, IONA_SWIM_BRAKE_RECOV, IONA_DSHOOT_BRAKE, IONA_USHOOT_BRAKE,
                          IONA_RUN_BRAKE, IONA_IDLE_BRAKE, IONA_SHOOT_BRAKE, IONA_SHOOT_BRAKE_UFORWARD, IONA_SHOOT_BRAKE_DFORWARD,
                          IONA_TURN, IONA_TURN_SWIM, IONA_TURN_MERMAID, IONA_SWIM_STOPBRAKE,
                          IONA_HOOK, IONA_HOOK_BREAK, IONA_HOOK_PULL,
                          IONA_HOOK_FINS, IONA_HOOK_BREAK_FINS, IONA_HOOK_PULL_FINS,
                          IONA_HITSTUN1A, IONA_HITSTUN1B, IONA_HITSTUN2A, IONA_HITSTUN2B,
                          IONA_KNOCKBACKA, IONA_KNOCKBACKB, IONA_KNOCKDOWN1A, IONA_KNOCKDOWN2A, IONA_KNOCKDOWN1B, IONA_KNOCKDOWN2B,
                          IONA_KDRECOV1A, IONA_KDRECOV2A, IONA_KDRECOV1B, IONA_KDRECOV2B,
                          IONA_CSCHARGE_UP, IONA_CSDASH, IONA_CSIMPACT,
                          IONA_CASTING_A, IONA_CASTING_B, IONA_CASTING_AIR,
                          IONA_SPELL_A, IONA_SPELL_A2, IONA_SPELL_B, IONA_SPELL_B2, IONA_SPELL_AIR,
                          IONA_ROLL_A, IONA_ROLL_B, IONA_SEASICKLE_1A, IONA_SEASICKLE_2A, IONA_SEASICKLE_3A,
                          IONA_SEASICKLE_1B, IONA_SEASICKLE_2B, IONA_SEASICKLE_3B,
                          IONA_SEASICKLE_AIR, IONA_SEASICKLE_AIR_FINS, IONA_SEASICKLE_CHARGEA, IONA_SEASICKLE_CHARGEB, IONA_SEASICKLE_CHARGEAIR,
                          IONA_REVERSE_RUN,
                          IONA_ANIMS_END
                         };
enum IonaAnimationFlags {IONA_AFLAG_FINS = 1,
                         IONA_AFLAG_SWIMMING = 2,
                         IONA_AFLAG_SHOOTING = 4,
                         IONA_AFLAG_TURNING = 8,
                         IONA_AFLAG_HURTING = 16,
                         IONA_AFLAG_KNOCKDOWN = 32,
                         IONA_AFLAG_IDLE = 64,
                         IONA_AFLAG_MELEE = 128,
                         IONA_AFLAG_ROLLING = 256,
                         IONA_AFLAG_JUMP = 512,
                         IONA_AFLAG_HASWALKING = 1024,
                         IONA_AFLAG_NOWALKING = 2048,
                         IONA_AFLAG_CASTING = 4096,
                         IONA_AFLAG_BRAKING = 8192,
                         IONA_AFLAG_MAGIC = 16384,
                         IONA_AFLAG_IDLE_ROTATES = 32768,
                         IONA_AFLAG_MAX = INT_MAX};
enum WatershedSpellType {SPELL_PISTOL, SPELL_MISSILE, SPELL_LASER, SPELL_THROWER, SPELL_MAGIC, SPELL_MELEE, SPELL_CUSTOM};
class WatershedPlayer : public BasePlayer2 {
private:
    int mShootResetTimer;
    bool mFins;
    bool mShellFlipped;
    bool mSwimming, mFocusMode, mFacingLeft;
    bool mObjectPulled;
    float mSwimAccel, mSwimDecel, mSwimAngle, mSwimMaxVel;
    float mDashVel, mDiveVel, mSurfaceVel;
    Vector2f mSwimVelocity, mSwimColImpulse, mSwimDirection, mThrowStrength, mSkidDirection, mLastDirection;
    Vector2f mRocketJumpVelocity, mRocketJumpDecel;
    float mFocusModifier, mFocusTimer, mGroundFocusModifier;
    float mGrabRange;
    short mFlipTimer;
    bool mSkidding, mDashing, mSurfacing, mTurning, mSurfaced;
    bool mDiving, mSkidStop, mFocusAccels, mTouchingWall;
    RigidBody* mHeldObject, *mPrevHeldObject;
    int mReleasedObjFrames, mReleasedObjDelay, mGrabObjDelay;
    Vector2f mHoldPoint, mAnchorPoint, mShotOrigin;
    float mAnchorZ, mHeldObjectZ;
    float mHoldLength;
    Chain mLassoChain;
    SkeletalModel mHoloRod, mHairband, mShellModel;
    bool mRodVisible;
    bool mRecoiling;
    int mHandAnchorID, mHeadAnchorID;
    float mHurlAngle;
    Rectanglef mHurlHitbox;
    int mActiveFlags;
    Circlef mSwimHitBox;
    float mFocusMaxSpeed;
    glm::vec3 mShellPos;
    glm::mat4 mModelTrans;
    glm::vec3 mShotLightColor, mShotLightColorCur;
    float mLightBrightness, mLightBrightStep;
    Vector2f mFocusDirection;
    Vector2f mSwimForce;
    bool mCharging, mBreached;
    float mCurrentCharge, mChargeTimer;
    float mMaxMeleeCharge, mCurrentMeleeCharge;
    float mCastTimer, mMaxCastTime;
    float mHitBoxHeight;
    Vector2f mAimDirection;
    bool mMeleeTargetFound;
    Vector2f mMeleeTargetPosition;
    int mEquippedSpellType;
    bool mMeleeSlashCreated;
    WaterHook mWaterHook;
    RigidBody* mSpellTarget;
    Vector2f mMagicTarget;
    bool mTouchingWaterSurface;

    void CheckAnimationsModel();
    void CheckGroundAnims();
    void CheckSurfaceAnims();
    void CheckSwimAnims();
    void CheckCastAnims();
    void CheckFocusAnimationsModel();
    void CheckInput();
    void AdjustAnimations();
    void Dive();
    void Surface();
    void Breach();
    void CheckFlip();
    void CheckJump();
    void SetFins(bool fins);
    void Walk();
    void Skid();
    void ResolveCollision(Collision& c, float mass, Vector2f vel);
    void ResolveCollision(RigidBody* b, Collision& c, Vector2f vel);
    void CheckPlatform(Collision& c, BackgroundTile2& bg);
    void CheckPlatform() {BasePlayer2::CheckPlatform();}
    void ReleaseHeldObject();
    void ShootBullet(Bullet b);
    bool UsesFocusAnims();
    inline void CheckFins();
    inline void AdjustSwimVel();
    inline void AdjustSwimAngle();
    void SetDirectionFlags();
    void Brake();
    void EndBrake();
    void Turn();
    void EndTurn();
    void Charge(double dt);
    void StopCharging();
    void MeleeCombo();
    void CastMagic();
    void EndMagicCast();
    void Land();
    void CreateBullet(int type);
    void FindNextBulletIndex();
    void UpdateShootMode();
    void CreateMeleeSlashes();
    void MeleeFinisher();
    void FindSpellTarget(const std::vector<RigidBody*>& grabbableObjects);

public:
    float mChargeSpeed, mChargeTimeMax, mChargeVelocityMax, mBreachStrength;
    float mRollSpeed, mRollHeight;
    std::shared_ptr<BulletSpawner> mWaterPistol, mMissile, mLaser;
    std::shared_ptr<BulletSprayer> mFoamThrower;
    MagicCaster mMagic;
    BulletMeleeSpawner mMeleeBulletSpawner;
    FlurryBullet mMeleeFinisher;
    float mRocketJumpSpeed;
    float mSpellTargetRange;
    //std::vector<std::shared_ptr<MagicBullet> > mMagic;
    //std::vector<std::shared_ptr<MeleeBullet> > mMeleeSlashes;

    WatershedPlayer();
    WatershedPlayer(BasePlayer2 player) {*this = player;}

    void InitSwimMovement(float maxSpeed, float accel, float decel, float focusModifier = 0.5f);
    void InitMovement(float maxSpeed, float accel, float decel);

    void Accelerate();
    void AdjustVelocity();
    void AdjustVelocity(Vector2f& vel);
    void Respawn();
    void ToggleSpellRight();
    void ToggleSpellLeft();

    void SetSwimMaxVel(float vel)           {mSwimMaxVel = vel;}
    void SetSwimAccel(float accel)          {mSwimAccel = accel;}
    void SetSwimDecel(float decel)          {mSwimDecel = decel;}
    void SetFocusModifier(float m)          {mFocusModifier = m;}
    void SetThrowStrength(Vector2f v)       {mThrowStrength = v;}
    void SetDiveVel(float d)                {mDiveVel = d;}
    void SetSurfaceVel(float s)             {mSurfaceVel = s;}
    void SetSkidStop(bool s)                {mSkidStop = s;}
    void SetFocusAccels(bool f)             {mFocusAccels = f;}
    void SetShotOrigin(Vector2f o)          {mShotOrigin = o;}
    void SetSwimColImpulse(Vector2f v)      {mSwimColImpulse = v;}
    void SetHoloRod(SkeletalModel m)        {mHoloRod = m;}
    void SetHairband(SkeletalModel m)       {mHairband = m;}
    void SetShellModel(SkeletalModel m)     {mShellModel = m;}
    void SetGrabRange(float r)              {mGrabRange = r;}
    void SetReleasedObjFrames(float d)      {mReleasedObjFrames = d;}
    void SetReleasedObjDelay(float d)       {mReleasedObjDelay = d;}
    void SetTouchingWall(bool t)            {mTouchingWall = t;}
    void SetGroundFocusModifier(float m)    {mGroundFocusModifier = m;}
    void SetFocusMaxSpeed(float m)          {mFocusMaxSpeed = m;}
    void SetSwimHitBox(Circlef s)           {mSwimHitBox = s;}
    void SetShotLightColor(glm::vec3 col)   {mShotLightColor = col;}
    void SetShotLightStep(float s)          {mLightBrightStep = s;}

    bool GetFocusAccels()                   {return mFocusAccels;}
    bool GetSkidStop()                      {return mSkidStop;}
    float GetDiveVel()                      {return mDiveVel;}
    float GetSurfaceVel()                   {return mSurfaceVel;}
    bool GetSurfaced()                      {return mSurfaced;}
    float GetSwimMaxVel()                   {return mSwimMaxVel;}
    float GetSwimAccel()                    {return mSwimAccel;}
    float GetSwimDecel()                    {return mSwimDecel;}
    float GetFocusModifier()                {return mFocusModifier;}
    float GetSwimAngle()                    {return mSwimAngle;}
    short GetFlipTimer()                    {return mFlipTimer;}
    bool GetDiving()                        {return mDiving;}
    bool GetSwimming()                      {return mSwimming;}
    bool GetFocusMode()                     {return mFocusMode;}
    bool GetFacingLeft()                    {return mFacingLeft;}
    float GetGroundFocusModifier()          {return mGroundFocusModifier;}
    float GetShootResetTimer()              {return mShootResetTimer;}
    float GetFocusMaxSpeed()                {return mFocusMaxSpeed;}
    float GetChargeTimer()                  {return mChargeTimer;}
    float GetMeleeChargeMax()               {return mMaxMeleeCharge;}
    float GeCurrentMeleeCharge()            {return mCurrentMeleeCharge;}
    float GetMeleeChargePercent()           {return mCurrentMeleeCharge / mMaxMeleeCharge;}
    float GetChargePercent()                {return mCastTimer / mMaxCastTime;}
    float GetCastTimer()                    {return mCastTimer;}
    void ApplyImpulse(Vector2f impulse);
    void ApplyImpulseX(float x);
    void ApplyImpulseY(float y);
    void ApplyForce(Vector2f impulse);
    void ApplyForceX(float x);
    void ApplyForceY(float y);

    Circlef& GetSwimHitBox()                {return mSwimHitBox;}
    glm::vec3 GetShellPos()                 {return mShellPos;}
    glm::mat4 GetModelTrans()               {return mModelTrans;}
    glm::vec3 GetShotLightColor()           {return mShotLightColor;}
    glm::vec3 GetShotLightColorCur()        {return mShotLightColorCur;}
    float GetLightBrightness()              {return mLightBrightness;}
    float GetLightBrightStep()              {return mLightBrightStep;}
    Vector2f GetFocusDirection()            {return mFocusDirection;}
    void SetFocusDirection(Vector2f vt)     {mFocusDirection = vt;}

    void SetFocus(bool focus);

    RigidBody* GetHeldObject()              {return mHeldObject;}
    Vector2f GetSwimVelocity()              {return mSwimVelocity;}
    Vector2f GetThrowStrength()             {return mThrowStrength;}
    Vector2f GetSwimDirection()             {return mSwimDirection;}
    bool IsHoldingObject()                  {return mHeldObject != NULL;}
    Chain& GetLassoChain()                  {return mLassoChain;}
    Chain* GetLassoChainPtr()               {return &mLassoChain;}
    Vector2f GetSwimColImpulse()            {return mSwimColImpulse;}
    SkeletalModel& GetHairband()            {return mHairband;}
    SkeletalModel& GetHoloRod()             {return mHoloRod;}
    SkeletalModel& GetShellModel()          {return mShellModel;}
    SkeletalModel* GetHairbandPtr()         {return &mHairband;}
    SkeletalModel* GetHoloRodPtr()          {return &mHoloRod;}
    SkeletalModel* GetShellModelPtr()       {return &mShellModel;}
    Rectanglef& GetHitBox()                 {return mHurlAngle ? mHurlHitbox : mHitBox;}
    Rectanglef* GetHitBoxPtr()              {return &GetHitBox();}
    Rectanglef* GetHurlHitboxPtr()          {return &mHurlHitbox;}
    Rectanglef& GetHurlHitbox()             {return mHurlHitbox;}
    bool GetTouchingWall()                  {return mTouchingWall;}
    float GetAnchorZ()                      {return mAnchorZ;}
    float GetReleasedObjFrames()            {return mReleasedObjFrames;}
    float GetReleasedObjDelay()             {return mReleasedObjDelay;}
    int GetGrabObjDelay()                   {return mGrabObjDelay;}
    Vector2f GetShotOrigin()                {return mShotOrigin;}
    glm::mat4 GetShellTransform();
    Vector2f GetShellPosition(float norm);
    Vector2f GetAimDirection()              {return mAimDirection;}
    Vector2f GetMagicTarget()               {return mMagicTarget;}
    int GetEquippedSpellType()              {return mEquippedSpellType;}
    bool IsHookActive()                     {return mWaterHook.GetAlive();}
    bool IsChargingMagic();

    inline void SetFlags(int flags)                    {mActiveFlags = flags;}
    inline void AddFlags(int flags)                    {mActiveFlags |= flags;}
    inline void RemoveFlags(int flags)                 {mActiveFlags &= ~flags;}
    inline int GetFlags()                              {return mActiveFlags;}
    inline bool HasFlag(int flag)                      {return mActiveFlags & flag;}

    std::string GetSpellString();

    bool IsShooting();
    void SetupModels();

    void Draw(ShaderProgram& shader, TextureArray& textures);
    void DrawBullets(SceneRenderer& rend);
    void DrawBulletHitBoxes(SceneRenderer& rend, Color color);

    void AddAnimation(Animation m);

    void Update(std::vector<SDL_Event>& events, float tstep = 1.0f);
    void Update(double t);
    void Update(double t, const std::vector<RigidBody*>& grabbableObjects);
    void UpdateBullets(double dt);
    void CheckGrabbleObjects(const std::vector<RigidBody*>& grabbableObjects);
    bool CheckCollision(WaterBody& waterBody);
    void CheckAndResolveCollision(RigidBody* b);
    void CheckAndResolveCollision(BackgroundTile2& ground);
    void CheckBulletCollisions(LevelRoom& room);
    void Hurt(float damage, Vector2f kb, bool knockback);
    void Recover();
    void ForceRecover();
    void UpdateRod(double d);
    void UpdateAnimation(float tstep = 1.0f);
    void UpdateModel(float t, Vector2f scale);
    void DropHeldObject(bool brkAnim = false);
    void GrabObject(RigidBody* obj);
    void ThrowObject();
    void UpdateHeldObject();
    void SetHeldObject(RigidBody* heldObject, float maxLength);
    void SetHoldPoint(Vector2f pt)  {mHoldPoint = pt; mLassoChain.SetAnchorPoint(mHitBox.GetCenter() + pt);}
    Vector2f GetHoldPoint()         {return mHoldPoint;}
    void ChangeAnimation(int i, float transitionDur = 0.25f);
    void ChangeAnimation(string anim);
    void ChangeAnimID(int i, float transitionDur = 0.1f);
    void ChangeAnimIDSpecial(int i, bool cancelable, float transitionDur = 0.1f);
    void SetGrounded(bool g);
    void Shoot();
    void GenerateAnimCache();
    void Attack();
    int GetCurrentBullet();
    void InitHitBox(Vector2f pos, float h, float w, int drawFrom);
    glm::vec3 GetWorldPos(float xScale = 1.0, float yScale = 1.0);
};
}

#endif // PLAYERWatershed_H_INCLUDED
