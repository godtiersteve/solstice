#ifndef PLAYERWatershed_H_INCLUDED
#define PLAYERWatershed_H_INCLUDED

#include <memory>
#include "player/baseplayer2.h"
#include "particle/particle.h"
#include "particle/bullet.h"
#include "particle/bullettype.h"
#include "graphics/3d/skeletalmodel.h"
#include "physics/chain.h"
#include "environment/room.h"

namespace solstice {
enum WatershedPlayerAnim {IONA_SWIM_IDLE = ANIM_LAND + 1, IONA_SWIM, IONA_SWIM_TWIRL, IONA_FIN_JUMP, IONA_FIN_FALL, IONA_LAND_FINS,
                          IONA_MERMAID_DIVE, IONA_MERMAID_IDLE, IONA_MERMAID_SURFACE, IONA_MERMAID_SWIM,
                          IONA_SHOOT_GROUND, IONA_SHOOT_DOWN, IONA_SHOOT_UP,
                          IONA_SHOOT_JUMP, IONA_SHOOT_BACK_JUMP, IONA_SHOOT_JUMP_UP, IONA_SHOOT_JUMP_DOWN,
                          IONA_RUNSHOOT_FORWARD, IONA_RUNSHOOT_BACK, IONA_RUNSHOOT_UFORWARD, IONA_RUNSHOOT_DFORWARD,
                          IONA_RUNSHOOT_UBACK, IONA_RUNSHOOT_DBACK,
                          IONA_SWIMSHOOT_IDLE, IONA_SWIMSHOOT_FORWARD, IONA_SWIMSHOOT_BACK, IONA_SWIMSHOOT_UP, IONA_SWIMSHOOT_DOWN,
                          IONA_SWIMSHOOT_UFORWARD, IONA_SWIMSHOOT_DFORWARD, IONA_SWIMSHOOT_UBACK, IONA_SWIMSHOOT_DBACK,
                          IONA_SHOOT_SWIM_JUMP, IONA_SHOOT_SWIM_JUMP_BACK, IONA_SHOOT_SWIM_JUMP_UP, IONA_SHOOT_SWIM_JUMP_DOWN,
                          IONA_SHOOT_RECOIL, IONA_SHOOT_RECOIL_DOWN, IONA_SHOOT_RECOIL_UP,
                          IONA_SHOOT_RECOIL_FINS, IONA_SHOOT_RECOIL_DOWN_FINS, IONA_SHOOT_RECOIL_UP_FINS,
                          IONA_SWIM_BRAKE, IONA_SWIM_BRAKE_RECOV, IONA_DSHOOT_BRAKE, IONA_USHOOT_BRAKE,
                          IONA_RUN_BRAKE, IONA_IDLE_BRAKE, IONA_SHOOT_BRAKE, IONA_SHOOT_BRAKE_UFORWARD, IONA_SHOOT_BRAKE_DFORWARD,
                          IONA_TURN, IONA_TURN_SWIM, IONA_TURN_MERMAID, IONA_SWIM_STOPBRAKE,
                          IONA_HOOK, IONA_HOOK_BREAK, IONA_HOOK_PULL,
                          IONA_HOOK_FINS, IONA_HOOK_BREAK_FINS, IONA_HOOK_PULL_FINS,
                          IONA_HITSTUN1A, IONA_HITSTUN1B, IONA_HITSTUN2A, IONA_HITSTUN2B,
                          IONA_KNOCKBACKA, IONA_KNOCKBACKB, IONA_KNOCKDOWN1A, IONA_KNOCKDOWN2A, IONA_KNOCKDOWN1B, IONA_KNOCKDOWN2B,
                          IONA_KDRECOV1A, IONA_KDRECOV2A, IONA_KDRECOV1B, IONA_KDRECOV2B,
                          IONA_ANIMS_END
                         };
enum IonaAnimationFlags {IONANIM_SHOOT = 1, IONANIM_TURN = 2, IONANIM_HURT = 4, IONANIM_KNOCKDOWN = 8};
class WatershedPlayer : public BasePlayer2 {
private:
    std::vector<BulletType> mBulletTypes;
    int mActiveBulletType;
    std::vector<Bullet> mBullets;
    size_t mLastInactiveBullet;
    int mShootResetTimer;
    bool mFins;
    bool mShellFlipped;
    bool mSwimming, mFocusMode, mFacingLeft;
    bool mObjectPulled;
    float mSwimAccel, mSwimDecel, mSwimAngle, mSwimMaxVel;
    float mDashVel, mDiveVel, mSurfaceVel;
    Vector2f mSwimVelocity, mSwimColImpulse, mSwimDirection, mThrowStrength, mSkidDirection, mLastDirection;
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

    Animation mLegAnim;
    std::vector<Animation> mLegAnims, mFinAnims;
    void CheckAnimationsSprite();
    void CheckAnimationsModel();
    void CheckGroundAnims();
    void CheckSurfaceAnims();
    void CheckSwimAnims();
    void CheckFocusAnimationsModel();
    void CheckInput();
    void AdjustAnimations();
    void Dive();
    void Surface();
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
    void ChangeAnimID(int baseAnim, int newAnim, int& i, float& dur, int& forceFrame);
public:
    WatershedPlayer();
    WatershedPlayer(BasePlayer2 player) {*this = player;}

    void InitSwimMovement(float maxSpeed, float accel, float decel, float focusModifier = 0.5f);
    void InitMovement(float maxSpeed, float accel, float decel);

    void FocusAccelerate();
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
    void SetActiveBullet(int i)             {i >= mBulletTypes.size() || i < 0 ? mActiveBulletType = -1 : mActiveBulletType = i;}
    void SetHoloRod(SkeletalModel m)        {mHoloRod = m;}
    void SetHairband(SkeletalModel m)       {mHairband = m;}
    void SetShellModel(SkeletalModel m)     {mShellModel = m;}
    void SetGrabRange(float r)              {mGrabRange = r;}
    void SetReleasedObjFrames(float d)      {mReleasedObjFrames = d;}
    void SetReleasedObjDelay(float d)       {mReleasedObjDelay = d;}
    void SetTouchingWall(bool t)            {mTouchingWall = t;}
    void SetGroundFocusModifier(float m)    {mGroundFocusModifier = m;}

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
    float GetGrabRange()                    {return mGrabRange;}
    float GetGroundFocusModifier()          {return mGroundFocusModifier;}
    std::vector<Animation>& GetLegAnims()   {return mLegAnims;}
    std::vector<Animation>& GetFinAnims()   {return mFinAnims;}
    float GetShootResetTimer()              {return mShootResetTimer;}

    void AddBulletType(BulletType b)        {mBulletTypes.push_back(b);}
    BulletType& GetBulletType(size_t i)     {return mBulletTypes[i];}
    BulletType* GetBulletTypePtr(size_t i)  {return i >= 0 && i < mBulletTypes.size() ? &mBulletTypes[i] : NULL;}
    std::vector<Bullet>& GetBullets()       {return mBullets;}

    Animation GetLegAnim()                  {return mLegAnim;}
    RigidBody* GetHeldObject()              {return mHeldObject;}
    QuadData GetLegQuad()                   {return mLegAnim.GetDrawQuad();}
    Vector2f GetSwimVelocity()              {return mSwimVelocity;}
    Vector2f GetThrowStrength()             {return mThrowStrength;}
    Vector2f GetSwimDirection()             {return mSwimDirection;}
    bool IsHoldingObject()                  {return mHeldObject != NULL;}
    Chain& GetLassoChain()                  {return mLassoChain;}
    Chain* GetLassoChainPtr()               {return &mLassoChain;}
    Vector2f GetSwimColImpulse()            {return mSwimColImpulse;}
    BulletType GetActiveBulletType()        {return mBulletTypes[mActiveBulletType];}
    int GetActiveBulletID()                 {return mActiveBulletType;}
    BulletType GetActiveBullet(size_t i)    {return mBulletTypes[i];}
    SkeletalModel& GetHairband()            {return mHairband;}
    SkeletalModel& GetHoloRod()             {return mHoloRod;}
    SkeletalModel& GetShellModel()          {return mShellModel;}
    SkeletalModel* GetHairbandPtr()         {return &mHairband;}
    SkeletalModel* GetHoloRodPtr()          {return &mHoloRod;}
    SkeletalModel* GetShellModelPtr()       {return &mShellModel;}
    Rectanglef& GetHitBox()                 {return mHurlAngle ? mHurlHitbox : mHitBox;}
    Rectanglef* GetHitBoxPtr()                 {return &GetHitBox();}
    Rectanglef* GetHurlHitboxPtr()          {return &mHurlHitbox;}
    Rectanglef& GetHurlHitbox()             {return mHurlHitbox;}
    bool GetTouchingWall()                  {return mTouchingWall;}
    float GetAnchorZ()                      {return mAnchorZ;}
    float GetReleasedObjFrames()            {return mReleasedObjFrames;}
    float GetReleasedObjDelay()             {return mReleasedObjDelay;}
    int GetGrabObjDelay()                   {return mGrabObjDelay;}
    Vector2f GetShotOrigin()                {return mShotOrigin;}
    size_t GetLastInactiveBullet()          {return mLastInactiveBullet;}
    bool IsShooting();
    Rectanglef GetGrabRect();
    void SetupModels();

    void Draw(ShaderProgram& shader, TextureArray& textures);
    void DrawBullets(SceneRenderer& rend);

    void AddAnimation(Animation m);

    void Update(std::vector<SDL_Event>& events, float tstep = 1.0f);
    void Update(double t);
    void Update(double t, const std::vector<RigidBody*>& grabbableObjects);
    void CheckGrabbleObjects(const std::vector<RigidBody*>& grabbableObjects);
    bool CheckCollision(WaterBody& waterBody);
    bool CheckCollision(BackgroundTile2& tile) {return BasePlayer2::CheckCollision(tile);}
    void CheckAndResolveCollision(RigidBody* b);
    void CheckAndResolveCollision(BackgroundTile2& ground) {BasePlayer2::CheckAndResolveCollision(ground);}
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
    void Shoot();
    void GenerateAnimCache();
    glm::vec3 GetWorldPos(float xScale = 1.0, float yScale = 1.0);
};
}

#endif // PLAYERWatershed_H_INCLUDED
