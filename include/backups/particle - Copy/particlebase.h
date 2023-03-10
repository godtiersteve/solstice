#ifndef PARTICLE_BASE_H_INCLUDED
#define PARTICLE_BASE_H_INCLUDED

#include "graphics/graphicglobals.h"
#include "graphics/vector2.h"
#include "graphics/quaddata.h"
#include "animation/animation.h"
#include <functional>

template <class T>
class ParticleBase {
    T mPosition, mVelocity, mAcceleration, mStopVelocity, mMaxSpeed;
    int mTextureID;
    float mRotateAngle, mAngle;
    float mSize, mScaleStep, mScaleMax;
    ParticleDestroy mDestroyedBy;
    float mFallSpeed;
    float mAlpha, mFadeStep;
    bool mDestroyed;
    unsigned short mMaxBounces, mBounces;
public:
    ParticleBase(T pos = 0, T startSpeed = 0, T accel = 0);
    ParticleBase(T pos, T startSpeed, float angle, float accel);
    void Update(float t = 1.0f);
    void Init(T pos, T startSpeed, T accel);
    void Init(T pos, T startSpeed, float angle, float accel);

    T GetPosition()                             {return mPosition;}
    T GetVelocity()                             {return mVelocity;}
    T GetAcceleration()                         {return mAcceleration;}
    T GetMaxSpeed()                             {return mMaxSpeed;}
    ParticleDestroy GetDestroyedBy()            {return mDestroyedBy;}
    float GetRotateAngle()                      {return mRotateAngle;}
    GLenum GetBlendSrc()                        {return mBlendSrc;}
    GLenum GetBlendDest()                       {return mBlendDest;}
    unsigned short GetCurFrame()                {return mCurFrame;}
    unsigned short GetDestroyFrame()            {return mDestroyFrame;}
    float GetSize()                             {return mSize;}
    float GetFallSpeed()                        {return mFallSpeed;}
    float GetAlpha()                            {return mAlpha;}
    float GetFadeStep()                         {return mFadeStep;}
    float GetScaleStep()                        {return mScaleStep;}
    float GetScaleMax()                         {return mScaleMax;}
    T GetStopVelocity()                         {return mStopVelocity;}
    Rectanglef& GetHitBox()                     {return mHitBox;}
    Animation& GetAnimation()                   {return mAnimation;}
    Animation& GetDestroyAnim()                 {return mDestroyAnim;}
    Rectanglef* GetHitBoxPtr()                  {return &mHitBox;}
    Animation* GetAnimationPtr()                {return &mAnimation;}
    Animation* GetDestroyAnimPtr()              {return &mDestroyAnim;}
    void SetPosition(T pos);
    void SetVelocity(T vel)                      {mVelocity = vel;}
    void SetAcceleration(T accel)                {mAcceleration = accel;}
    void SetMaxSpeed(T spd)                      {mMaxSpeed = spd;}
    void SetRotateAngle(float angle)             {mRotateAngle = angle;}
    void SetDestroyedBy(ParticleDestroy des)     {mDestroyedBy = des;}
    void SetDestroyFrame(unsigned short i)       {mDestroyFrame = i;}
    void SetSize(float s)                        {mSize = s;}
    void SetFallSpeed(float s)                   {mFallSpeed = s;}
    void SetAlpha(float a)                       {mAlpha = a;}
    void SetFadeStep(float s)                    {mFadeStep = s;}
    void SetScaleStep(float s)                   {mScaleStep = s;}
    void SetScaleMax(float s)                    {mScaleMax = s;}
    void SetStopVelocity(T v)                    {mStopVelocity = v;}
    void Start();
    void Reset(T position, T velocity);
    operator bool() {return !mDestroyed;}
    bool Alive() {return !mDestroyed;}
    bool Destroyed() {return mDestroyed;}
    void Destroy() {mDestroyed = true;}
};
typedef ParticleBase<glm::vec2> Particle2f;
typedef ParticleBase<glm::vec3> Particle3f;
typedef ParticleBase<glm::vec4> Particle4f;
typedef ParticleBase<glm::ivec2> Particle2i;
typedef ParticleBase<glm::ivec3> Particle3i;
typedef ParticleBase<glm::ivec4> Particle4i;

template <class T>
ParticleBase<T>::ParticleBase(T pos, T startSpeed, T accel) {
}

template <class T>
ParticleBase<T>::ParticleBase(T pos, T startSpeed, float angle, float accel) {
}

template <class T>
void ParticleBase<T>::Update(float t) {
}

template <class T>
void ParticleBase<T>::Init(T pos, T startSpeed, T accel) {
}

template <class T>
void ParticleBase<T>::Init(T pos, T startSpeed, float angle, float accel) {
}

template <class T>
void ParticleBase<T>::Start() {
}

template <class T>
void ParticleBase<T>::Reset(T position, T velocity) {
}

template <class T>
void ParticleBase<T>::SetPosition(T pos) {
}

#endif // PARTICLE_BASE_H_INCLUDED
