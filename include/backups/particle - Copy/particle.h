#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "graphics/graphicglobals.h"
#include "graphics/vector2.h"
#include "graphics/quaddata.h"
#include "animation/animation.h"
#include "particleanim.h"
#include "kaguya/kaguya.hpp"
#include <functional>

namespace solstice {
enum ParticleDestroy{PDESTROY_NONE = 0,
                    PDESTROY_GROUND = 1, PDESTROY_WALL = 2,
                    PDESTROY_BACKGROUND = PDESTROY_GROUND | PDESTROY_WALL,
                    PDESTROY_ENEMY = 4, PDESTROY_BULLET = 8,
                    PDESTROY_PLAYER = 16,
                    PDESTROY_OBJECT = 32, PDESTROY_WATER = 64};
struct ParticleState {
    glm::vec3 position;
    glm::vec3 velocity;
    float rotateAngle, angle, angleVel;
    float size;
    float alpha;
    ParticleState() {
        position = velocity = glm::vec3(0.0f);
        size = 1.0f;
        alpha = 1.0f;
        angle = 0.0f;
        rotateAngle = 0.0f;
        angleVel = 0.0f;
    }
};
struct ParticlePath {
    glm::vec3 pos, accel, decel, vel;
    float size, sizeStep, alpha, alphaStep, damping;
    ParticlePath() {
        size = 1.0f;
        sizeStep = 0.0f;
        alpha = 1.0f;
        alphaStep = 1.0f;
        damping = 1.0f;
    }
};
void SetupParticleBinds(kaguya::State& state);
class Particle {
protected:
    ParticleState mInitialState;
    std::vector<glm::vec2> mFrames;
    int mCurAnimFrame, mCurAnimFrameStep;
    int mAnimColumn, mAnimRow;
    glm::vec2 mFrameClipSize;
    int mNumFrames;
    ParticleAnimation mAnimation;

    GLenum mBlendSrc, mBlendDest;
    glm::vec3 mPosition;
    glm::vec3 mVelocity, mAcceleration, mDeceleration;
    glm::vec3 mStopVelocity;
    glm::vec3 mMaxSpeed;
    float mRotateAngle, mAngle, mAngleVel;
    float mSize, mScaleStep, mScaleMax;
    glm::vec2 mTextureOffset;
    int mCurFrame, mDestroyFrame, mResetFrame, mStartFrame;
    int mDestroyedBy, mCollidesWidth;
    float mFallSpeed;
    float mAlpha, mFadeStep;
    bool mDestroyed, mDecelsToStop;
    bool mXRotates, mYRotates, mZRotates;
    float mDamping;
    void CheckVelocity();
public:
    Particle();
    Particle(glm::vec3 pos, glm::vec3 startSpeed, glm::vec3 accel);
    Particle(glm::vec3 pos, glm::vec3 startSpeed, float angle, float accel);
    Particle(const ParticleState& state);
    Particle& operator=(const ParticleState& state);
    void Update(float t = 1.0f);
    void Init(glm::vec3 pos, glm::vec3 startSpeed, glm::vec3 accel);
    void Init(glm::vec3 pos, glm::vec3 startSpeed, float angle, float accel);
    void SetAttributes(glm::vec3 pos, glm::vec3 startSpeed, float angle, float accel);
    void SetAttributes(glm::vec3 pos, glm::vec3 startSpeed, glm::vec3 accel);
    void SetAttributes(ParticlePath p);
    void SetAttributes(ParticlePath p, float angle);

    void SetAnimRowCol(int row, int col)                {mAnimRow = row; mAnimColumn = col;}
    void SetAnimRow(int r)                              {mAnimRow = r;}
    void SetAnimCol(int c)                              {mAnimColumn = c;}
    void SetClipSize(glm::vec2 clip)                    {mFrameClipSize = clip;}
    void SetNumFrames(int i)                            {mNumFrames = i;}
    void SetAnimation(ParticleAnimation anim)           {mAnimation = anim;}
    void AddAnimationFrame(ParticleAnimFrame f)         {mAnimation.AddFrame(f);}

    std::vector<glm::vec2>& GetFrames()                 {return mFrames;}
    void AddFrame(glm::vec2 frame)                      {mFrames.push_back(frame);}
    glm::vec2& GetFrame(size_t i)                       {return mFrames[i];}
    glm::vec3 GetPosition()                             {return mPosition;}
    glm::vec3 GetVelocity()                             {return mVelocity;}
    glm::vec3 GetAcceleration()                         {return mAcceleration;}
    glm::vec3 GetMaxSpeed()                             {return mMaxSpeed;}
    glm::vec3 GetDeceleration()                         {return mDeceleration;}
    ParticleState& GetInitialState()                    {return mInitialState;}
    void SetInitialState(ParticleState s)               {mInitialState = s;}
    int GetDestroyedBy()                                {return mDestroyedBy;}
    float GetRotateAngle()                              {return mRotateAngle;}
    GLenum GetBlendSrc()                                {return mBlendSrc;}
    GLenum GetBlendDest()                               {return mBlendDest;}
    int GetCurFrame()                                   {return mCurFrame;}
    int GetDestroyFrame()                               {return mDestroyFrame;}
    float GetSize()                                     {return mSize;}
    float GetFallSpeed()                                {return mFallSpeed;}
    float GetAlpha()                                    {return mAlpha;}
    float GetFadeStep()                                 {return mFadeStep;}
    float GetScaleStep()                                {return mScaleStep;}
    float GetScaleMax()                                 {return mScaleMax;}
    float GetDamping()                                  {return mDamping;}
    bool GetDecelsToStop()                              {return mDecelsToStop;}
    glm::vec3 GetStopVelocity()                         {return mStopVelocity;}
    float GetAngleVel()                                 {return mAngleVel;}
    glm::vec2 GetTextureOffsets()                       {return mTextureOffset;}
    int GetCurrentAnimFrame()                           {return mCurAnimFrame;}
    int GetResetFrame()                                 {return mResetFrame;}
    int GetNumFrames()                                  {return mNumFrames;}
    int GetAnimRow()                                    {return mAnimRow;}
    int GetAnimCol()                                    {return mAnimColumn;}
    glm::vec2 GetClipSize()                             {return mFrameClipSize;}
    int GetCollidesWidth()                              {return mCollidesWidth;}
    float GetAngle()                                    {return mAngle;}

    void SetPosition(glm::vec3 pos);
    void SetVelocity(glm::vec3 vel)                     {mVelocity = vel;}
    void SetInitialVelocity(glm::vec3 vel)              {mInitialState.velocity = vel;}
    void SetAcceleration(glm::vec3 accel)               {mAcceleration = accel;}
    void SetDecelsToStop(bool d)                        {mDecelsToStop = d;}
    void SetMaxSpeed(glm::vec3 spd)                     {mMaxSpeed = spd;}
    void SetRotateAngle(float angle)                    {mRotateAngle = angle; mInitialState.rotateAngle = angle;}
    void SetDestroyedBy(int des)                        {mDestroyedBy = des;}
    void SetBlendSrc(GLenum src)                        {mBlendSrc = src;}
    void SetBlendDest(GLenum dest)                      {mBlendDest = dest;}
    void SetDestroyFrame(int i)                         {mDestroyFrame = i;}
    void SetResetFrame(int i)                           {mResetFrame = i;}
    void SetSize(float s)                               {mSize = s; if(s > mScaleMax) {mScaleMax = s;}}
    void SetFallSpeed(float s)                          {mFallSpeed = s;}
    void SetAlpha(float a)                              {mAlpha = a; mInitialState.alpha = a;}
    void SetFadeStep(float s)                           {mFadeStep = s;}
    void SetScaleStep(float s)                          {mScaleStep = s;}
    void SetScaleMax(float s)                           {mScaleMax = s;}
    void SetStopVelocity(glm::vec3 v)                   {mStopVelocity = v;}
    void SetDamping(float d)                            {mDamping = d;}
    void SetAngleVel(float a)                           {mAngleVel = a; mInitialState.angleVel = a;}
    void SetTextureOffset(glm::vec2 off)                {mTextureOffset = off;}
    void SetXRotates(bool r)                            {mXRotates = r;}
    void SetYRotates(bool r)                            {mYRotates = r;}
    void SetZRotates(bool r)                            {mZRotates = r;}
    bool GetXRotates()                                  {return mXRotates;}
    bool GetYRotates()                                  {return mYRotates;}
    bool GetZRotates()                                  {return mZRotates;}
    void SetCurrentFrame(int c)                         {mCurFrame = c;}
    void SetRotates(bool x, bool y, bool z)             {mXRotates = x; mYRotates = y; mZRotates = z;}
    void SetStartFrame(int f)                           {mStartFrame = f;}
    int GetStartFrame()                                 {return mStartFrame;}
    void SetAngle(float a)                              {mAngle = a;}
    void SetCollidesWith(int i)                         {mCollidesWidth = i;}
    void SaveInitialState();
    void SetDeceleration(glm::vec3 dec);
    void AddVelocity(glm::vec3 vel)                     {mVelocity+= vel;}

    void Start()                                        {mCurFrame = mStartFrame;}
    void Reset(ParticleState initialState);
    void Reset();
    void Reset(glm::vec3 pos);
    operator bool() {return !mDestroyed;}
    bool Alive();
    bool Started();
    bool Destroyed() {return mDestroyed;}
    void Destroy() {mDestroyed = true;}

    friend void SetupParticleBinds(kaguya::State& state);
};

}


#endif // PARTICLE_H_INCLUDED
