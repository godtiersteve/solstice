#ifndef BASEPLAYER2_H_INCLUDED
#define BASEPLAYER2_H_INCLUDED

#include "physics/physicsrect.h"
#include "animation/animation.h"
#include "graphics/3d/skeletalmodel.h"
#include "actionkey.h"
#include "sdl.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace solstice {
enum PlayerAnimation {ANIM_IDLE = 1, ANIM_WALK, ANIM_JUMP, ANIM_FALL, ANIM_LAND};
class BasePlayer2 : public PhysicsBox2f {
protected:
    unsigned short mHitStun, mHitStunCounter;
    bool mHurling;
    float mRightHeldFrames, mLeftHeldFrames;
    bool mUpHeld, mDownHeld, mRightHeld, mLeftHeld;
    float mJumpStrength, mHoldJumpStrength, mWaterJumpStrength;
    bool mJumped, mJumping;
    SkeletalModel mModel;
    vector<Animation> mAnimations;
    Animation mCurrentAnimation;
    Vector2f mSpawnPosition, mModelOffset;
    vector<ActionKey> mActionKeys;
    vector<int> mAnimationCache;
    float mWalkSpeed, mWalkAccel;
    bool mLandStop, mAirAccels;
    bool mHoldJump;
    bool mFlipped;
    float mHoldJumpFrames, mHoldJumpTimer;
    bool mOnPlatform;
    bool mUsesModel;
    float mSlopeForce;
    bool mSpecialAnim, mCancelableSpecial;
    float mHealth, mMagic, mMaxMagic, mMaxHealth;
    float mKnockdownTime, mMaxKnockdownTime;
    bool mKnockedDown;

    //TODO : Implement the movement limiting stuff
    bool mIgnoreInput;
    short mPaused;

    void Jump(float extraStrength = 0.0f);
    virtual void CheckAnimations();
    virtual void CheckAnimationsModel();
    virtual void CheckAnimationsSprite();
    virtual void ForceAnimID(int i, int frame);
    void Float();
    void CheckInput();
    void CheckPlatform();
    virtual void ResolveCollision(Collision& c, float mass, Vector2f vel);
    void ResolveCollision(RigidBody* b, Collision& c, Vector2f vel);
public:
    BasePlayer2();
    BasePlayer2(Rectanglef hitbox);
    void Init();

    void SetHitStun(unsigned short hitstun)         {mHitStun = hitstun;}
    void SetJumpStrength(float strength)            {mJumpStrength = strength;}
    void SetHoldJumpStrength(float strength)        {mHoldJumpStrength = strength;}
    void SetJumpStrength(float jump, float hold)    {mJumpStrength = jump; mHoldJumpStrength = hold;}
    void AddAnimation(Animation anim)               {mAnimations.push_back(anim);}
    void AddKey(ActionKey k)                        {mActionKeys.push_back(k);}
    void SetSpawnPosition(Vector2f pos)             {mSpawnPosition = pos;}
    void SetLandStop(bool landStop)                 {mLandStop = landStop;}
    void SetAirAccels(bool airAccels)               {mAirAccels = airAccels;}
    void SetWalkSpeed(float speed)                  {mWalkSpeed = speed;}
    void SetWalkSpeed(float speed, float accel)     {mWalkSpeed = speed; mWalkAccel = accel;}
    void SetWalkAcceleration(float accel)           {mWalkAccel = accel;}
    void SetHoldJump(float strength, int duration);
    void SetWaterJumpStrength(float strength)       {mWaterJumpStrength = strength;}
    void SetIgnoreInput(bool in)                    {mIgnoreInput = in;}
    void SetModelOffset(Vector2f off)               {mModelOffset = off;}
    void SetHealth(float h)                         {mHealth = h;}
    void SetMaxHealth(float m)                      {mMaxHealth = m;}
    void SetMagic(float m)                          {mMagic = m;}
    void SetMaxMagic(float m)                       {mMaxMagic = m;}
    void SetHurling(bool h)                         {mHurling = true;}
    void SetKnockdownTime(float t)                  {mKnockdownTime = t;}
    void SetMaxKnockdownTime(float t)               {mMaxKnockdownTime = t;}
    void SetKnockedDown(bool k)                     {mKnockedDown = k;}
    void AdjustVelocity();
    void AdjustVelocity(Vector2f& vel);
    void Walk();

    unsigned short GetHitStun()                 {return mHitStun;}
    unsigned short GetHitStunCounter()          {return mHitStunCounter;}
    float GetJumpStrength()                     {return mJumpStrength;}
    float GetHoldJumpStrength()                 {return mHoldJumpStrength;}
    unsigned short GetHoldJumpTimer()           {return mHoldJumpTimer;}
    Animation& GetAnimation(size_t i)           {return mAnimations[i];}
    Animation& GetAnimation(string anim);
    std::vector<Animation>& GetAnimations()     {return mAnimations;}
    Vector2f GetSpawnPosition()                 {return mSpawnPosition;}
    ActionKey& GetActionKey(size_t i)           {return mActionKeys[i];}
    ActionKey& GetActionKey(PlayerKey key);
    bool GetLandStop()                          {return mLandStop;}
    bool GetAirAccels()                         {return mAirAccels;}
    bool GetHoldJump()                          {return mHoldJump;}
    unsigned short GetHoldJumpFrames()          {return mHoldJumpFrames;}
    float GetWalkSpeed()                        {return mWalkSpeed;}
    float GetWalkAcceleration()                 {return mWalkAccel;}
    float GetWaterJumpStrength()                {return mWaterJumpStrength;}
    bool GetJumped()                            {return mJumped;}
    bool GetFlipped()                           {return mFlipped;}
    bool GetIgnoreInput()                       {return mIgnoreInput;}
    Vector2f& GetModelOffset()                  {return mModelOffset;}
    bool GetSpecialAnim()                       {return mSpecialAnim;}
    float GetHealth()                           {return mHealth;}
    float GetMagic()                            {return mMagic;}
    float GetMaxMagic()                         {return mMaxMagic;}
    float GetMaxHealth()                        {return mMaxHealth;}
    bool GetHurling()                           {return mHurling;}
    float GetKnockdownTime()                    {return mKnockdownTime;}
    float GetMaxKnockdownTime()                 {return mMaxKnockdownTime;}
    bool GetKnockedDown()                       {return mKnockedDown;}

    void Update(std::vector<SDL_Event>& events, float tstep = 1.0f);
    void UpdateKeys(SDL_Event& event);
    void UpdateKeys(vector<SDL_Event>& events);
    void UpdateKeys(SDL_Event& event, SDL_GameController* controller);
    void CheckKeyDown(SDL_Keycode code);
    void CheckKeyUp(SDL_Keycode code);
    void UpdateAnimation();
    virtual void ChangeAnimID(int i, float transitionDur = 0.05f);
    virtual void ChangeAnimation(int i, float transitionDur = 0.05f);
    virtual void ChangeAnimation(string anim);
    virtual void TransitionToAnimation(int i, float transitionSpeed, int srcFrame, int destFrame);
    unsigned int GetCurrentFrameNum();
    virtual void Respawn();
    bool GetKeyDown(PlayerKey key);
    bool GetKeyDown(string key);
    bool NoDirections();
    KeyState GetKeyState(PlayerKey key);
    QuadData& GetDrawQuad()                 {return mCurrentAnimation.GetDrawQuad();}
    std::vector<int> GetAnimationCache()    {return mAnimationCache;}
    virtual void GenerateAnimCache();
    void UpdateModel(float t, Vector2f scale);
    SkeletalModel& GetModel()               {return mModel;}
    ModelAnimation& GetCurrentAnim()        {return *mModel.GetCurrentAnim();}
    SkeletalModel* GetModelPtr()            {return &mModel;}
    void SetupModel();
    float GetUsesModel()                    {return mUsesModel;}
    void SetUsesModel(bool u)               {mUsesModel = u;}
    void SetModel(SkeletalModel m)          {mModel = m;}
    void SortAnimations(std::function<void(std::vector<Animation>&)> animSort) {animSort(mAnimations);}
    void SortAnimations(std::function<void(std::vector<ModelAnimation>&)> animSort) {animSort(mModel.GetAnimations());}
    int GetCurrentAnimID();
    bool GetCurrentAnimFinished();
    bool AnyButtonPressed();
    void CheckAndResolveCollision(BackgroundTile2& ground);
    void CheckAndResolveCollision(RigidBody* b) {RigidBody::CheckAndResolveCollision(b);}
    //void CheckAndResolveCollision(BackgroundTile2& ground);
};
    void DefaultAnimSort(std::vector<Animation>& anims);
}


#endif // BASEPLAYER2_H_INCLUDED
