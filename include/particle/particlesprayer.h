#ifndef PARTICLESPRAYER_H_INCLUDED
#define PARTICLESPRAYER_H_INCLUDED

#include "graphics/scenerenderer.h"
#include "graphics/3d/mesh.h"
#include "particle.h"
#include "particlerenderer.h"
#include <random>
#include <deque>
#include <list>
#include <chrono>
#include <utility>

namespace solstice {
template <class T>
struct ValueRange {
    T min, max;
    ValueRange() {}
    ValueRange(T x, T y) {
        min = x;
        max = y;
    }
    ValueRange(T x) {
        min = x;
        max = x;
    }
};
typedef ValueRange<float> ValueRangef;
typedef ValueRange<glm::vec2> ValueRange2f;
typedef ValueRange<glm::vec3> ValueRange3f;
typedef ValueRange<glm::vec4> ValueRange4f;
typedef ValueRange<int> ValueRangei;
typedef ValueRange<unsigned int> ValueRangeui;
typedef ValueRange<glm::ivec2> ValueRange2i;
typedef ValueRange<glm::ivec3> ValueRange3i;
typedef ValueRange<glm::ivec4> ValueRange4i;

struct RNGRange {
    glm::vec3 min, max;
    RNGRange() {
        min = glm::vec3(0);
        max = glm::vec3(0);
    }
    RNGRange(glm::vec3 mmin, glm::vec3 mmax) {
        min = mmin;
        max = mmax;
    }
    RNGRange(float x, float y) {
        min = glm::vec3(-x, -y, 0.0);
        max = glm::vec3(x, y, 0.0);
    }
    RNGRange(float x, float y, float z) {
        min = glm::vec3(-x, -y, -z);
        max = glm::vec3(x, y, z);
    }
    RNGRange(float x) {
        min = glm::vec3(x);
        max = glm::vec3(x);
    }
};
///*TODO: Add quad rendering to particle engine
enum ParticleRenderType{PARTREND_CIRCLES, PARTREND_QUADS};
class ParticleSprayer : public Particle {
    static std::default_random_engine sGenerator;
    std::list<Particle> mParticles;
    std::vector<Particle> mFixedParticles;
    ParticleAnimation mBaseAnimation;
    RNGRange mVelocityRange, mAccelRange, mDecelRange;
    ValueRangef mAngleRange, mAngleVelRange, mAngleStepRange;
    ValueRangei mDestroyRange;
    ValueRangef mSizeRange, mScaleStepRange;
    ValueRangef mDampingRange;
    ValueRange3f mPositionRange;
    float mSpawnDistanceMin;
    ValueRangef mSpawnDistanceRange;
    float mScale, mScaleMax;
    glm::vec3 mMaxSpeed, mMinSpawnSpeed;
    ValueRangef mFadeRange, mAlphaRange;
    size_t mMaxParticles;
    size_t mParticlesPerFrame;
    int mResetFrame, mCurrentFrame;
    int mTextureID;
    Color mColor;
    bool mNoRespawn, mRegenParticles;
    size_t mGeneratedParticles;
    ParticleRenderer* mRenderer;

    Mesh mBaseQuad;
    ParticleRenderType mRenderType;

    Vector2f mInfluenceAngle;
    glm::vec3 mParticleSpawnPos;

    bool mFinished;

    glm::vec2 mTextureClip;
    GLenum mBlendSrc, mBlendDest;
    bool mResetsSelf;
    int mFrameStep;
    float mXNorm, mYNorm;
    Particle GenerateParticle(int frame);
public:
    bool mActive;
    ParticleSprayer(size_t amount = 50);
    void Init(ValueRangef angleRange, glm::vec3 vel, RNGRange accel);
    void Init(RNGRange vel, RNGRange accel);
    void GenerateParticles();
    size_t GetParticlesPerFrame()                   {return mParticlesPerFrame;}
    RNGRange GetVelocityRange()                     {return mVelocityRange;}
    RNGRange GetAccelRange()                        {return mAccelRange;}
    RNGRange GetDecelRange()                        {return mDecelRange;}
    ValueRangef GetAngleRange()                     {return mAngleRange;}
    int GetResetFrame()                             {return mResetFrame;}
    int GetCurrentFrame()                           {return mCurrentFrame;}
    size_t GetMaxParticles()                        {return mMaxParticles;}
    ValueRangei GetDestroyRange()                   {return mDestroyRange;}
    ValueRangef GetFadeRange()                      {return mFadeRange;}
    ValueRangef GetAlphaRange()                     {return mAlphaRange;}
    int GetTextureID()                              {return mTextureID;}
    Color GetColor()                                {return mColor;}
    ValueRangef GetSizeRange()                      {return mSizeRange;}
    float GetScale()                                {return mScale;}
    float GetScaleMax()                             {return mScaleMax;}
    ValueRangef GetStepScaleRange()                 {return mScaleStepRange;}
    glm::vec3 GetMaxSpeed()                         {return mMaxSpeed;}
    int GetFrameStep()                              {return mFrameStep;}
    ValueRangef GetAngleStepRange()                 {return mAngleStepRange;}
    ValueRangef GetAngleVelRange()                  {return mAngleVelRange;}
    glm::vec2 GetTextureClip()                      {return mTextureClip;}
    void SetupBuffers();
    void Draw3D(ShaderProgram& shader);
    void CleanBuffers();
    void SetFallSpeed(float s)                      {mFallSpeed = s;}
    void SetParticlesPerFrame(size_t p)             {mParticlesPerFrame = p;}
    void SetMaxParticles(size_t m)                  {mMaxParticles = m;}
    void SetVelocityRange(RNGRange vel)             {mVelocityRange = vel;}
    void SetAccelRange(RNGRange acc)                {mAccelRange = acc;}
    void SetDecelRange(RNGRange dec)                {mDecelRange = dec;}
    void SetAngleRange(ValueRangef rng)             {mAngleRange = rng;}
    void SetParticleAlphas(float a);
    void SetAngleRange(float min, float max)        {mAngleRange = ValueRangef(min, max);}
    void SetResetFrame(int i)                       {mResetFrame = i;}
    void SetDestroyRange(ValueRangei range)         {mDestroyRange = range;}
    void SetFadeRange(ValueRangef r)                {mFadeRange = r;}
    void SetAlphaRange(ValueRangef r)               {mAlphaRange = r;}
    void SetTextureID(int i)                        {mTextureID = i;}
    void SetColor(Color c)                          {mColor = c;}
    void SetSizeRange(ValueRangef r)                {mSizeRange = r;}
    void SetScale(float s)                          {mScale = s; for(auto& part : mParticles) {part.SetSize(s);}}
    void SetScale(float s, ValueRangef r)           {mScale = s; mSizeRange = r; mScaleMax = s * r.max;}
    void SetScaleStepRange(ValueRangef r)           {mScaleStepRange = r;}
    void SetScaleMax(float f)                       {mScaleMax = f;}
    void SetMaxSpeed(glm::vec3 m)                   {mMaxSpeed = m;}
    void SetFrameStep(int f)                        {mFrameStep = f;}
    void SetAngleStepRange(ValueRangef r)           {mAngleStepRange = r;}
    void SetAngleVelRange(ValueRangef r)            {mAngleVelRange = r;}
    void SetTextureClip(glm::vec2 c)                {mTextureClip = c;}
    void SetPositionRange(ValueRange3f r)           {mPositionRange = r;}
    void SetSpawnDistanceMin(float m)               {mSpawnDistanceMin = m;}
    void SetSpawnDistanceRange(ValueRangef r)       {mSpawnDistanceRange = r;}
    void SetDampingRange(ValueRangef d)             {mDampingRange = d;}
    void SetNoRespawn(bool b)                       {mNoRespawn = b;}
    void SetRegenParticles(bool r)                  {mRegenParticles = r;}
    void SetMinSpawnSpeed(glm::vec3 min)            {mMinSpawnSpeed = min;}
    void SetParticleSpawnPos(glm::vec3 pos)         {mParticleSpawnPos = pos;}
    std::list<Particle> GetParticles()            {return mParticles;}
    size_t GetSize()                                {return mParticles.size();}
    size_t GetGeneratedParticles()                  {return mGeneratedParticles;}
    void SetBlend(GLenum src, GLenum dest)          {mBlendSrc = src; mBlendDest = dest;}
    GLenum GetBlendSrc()                            {return mBlendSrc;}
    GLenum GetBlendDest()                           {return mBlendDest;}
    float GetXNorm()                                {return mXNorm;}
    float GetYNorm()                                {return mYNorm;}
    glm::vec3 GetParticleSpawnPos()                 {return mParticleSpawnPos;}
    void SetNorm(float n)                           {mYNorm = mXNorm = n;}
    void SetNorm(float x, float y)                  {mXNorm = x; mYNorm = y;}
    void AddFixedParticle(Particle p)               {mFixedParticles.push_back(p);}
    Particle& GetFixedParticle(size_t i)            {return mFixedParticles[i];}
    void SetInfluenceAngle(Vector2f a)              {mInfluenceAngle = a;}
    void AddBaseAnimFrame(ParticleAnimFrame f)      {mBaseAnimation.AddFrame(f);}
    void SetBaseAnim(ParticleAnimation anim)        {mBaseAnimation = anim;}
    ParticleRenderer* GetRenderer()                 {return mRenderer;}
    void SetRenderer(ParticleRenderer* r)           {mRenderer = r;}

    void SetParticleAnimations(ParticleAnimation anim);
    void SetParticleOffsets(glm::vec2 offset);
    void SetXRotates(bool r);
    void SetYRotates(bool r);
    void SetZRotates(bool r);
    void SetRotates(bool x, bool y, bool z);
    void SetResetsSelf(bool s)                      {mResetsSelf = s;}
    bool GetResetsSelf()                            {return mResetsSelf;}
    void CreateParticlesPath(ParticlePath p, int numParticles, int framesBetween);
    void CreateParticlesPathArc(ParticlePath p, float startAngle, float endAngle, int numParticles, int startFrame, int framesBetween);
    void AddVelocity(glm::vec3 vel);
    size_t GetNumParticles()                        {return mParticles.size() + mFixedParticles.size() + 1;}

    void Update(double dt = 1.0f, glm::vec3 trans = glm::vec3(0));
    void Reset();
    void ResetParticles();
    void ResetSelf()                                {Particle::Reset();}
    void ResetAllAttributes();
    void Draw(SceneRenderer& rend);
    bool Finished();
};

}

#endif // PARTICLESPRAYER_H_INCLUDED
