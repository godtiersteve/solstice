#include "particlesprayer.h"

namespace solstice {
std::default_random_engine ParticleSprayer::sGenerator(std::random_device{}());
ParticleSprayer::ParticleSprayer(size_t amount) {
    mMaxParticles = amount;
    mAccelRange = RNGRange(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    mDecelRange = RNGRange(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    mAngleRange = ValueRangef(0, 0);
    mFadeRange = ValueRangef(0, 0);
    mAngleVelRange = ValueRangef(0, 0);
    mAngleStepRange = ValueRangef(0, 0);
    mSizeRange = ValueRangef(1.0f, 1.0f);
    mScaleStepRange = ValueRangef(0.0f, 0.0f);
    mFallSpeed = 0;
    mParticlesPerFrame = 5;
    mResetFrame = 0;
    mCurrentFrame = 0;
    mDestroyRange = 0;
    mTextureID = -1;
    mScale = 1.0f;
    mScaleMax = 1.0f;
    mAlphaRange = ValueRangef(1.0f);
    mAlpha = 0.0f;
    mMaxSpeed = glm::vec3(10.0f, 10.0f, 10.0f);
    mGeneratedParticles = 0;
    mBlendSrc = GL_SRC_ALPHA;
    mBlendDest = GL_ONE_MINUS_SRC_ALPHA;
    mXNorm = 2.0 / 1280.0;
    mYNorm = 2.0 / 720.0;
    mDampingRange = 1.0f;
    mFrameStep = 0;
    mNoRespawn = false;
    mTextureClip = glm::vec2(1.0f);
    mDestroyed = false;
    mResetsSelf = true;
    mRegenParticles = true;
    mAlpha = 0.0f;
    SaveInitialState();
    mFinished = false;
    mRenderer = NULL;
    mSpawnDistanceMin = 0;
    mSpawnDistanceRange = ValueRangef(0.0, 0.0);
    mActive = true;
}
void ParticleSprayer::Init(ValueRangef angleRange, glm::vec3 vel, RNGRange accel) {
    mAngleRange = angleRange;
    mAngleVelRange = ValueRangef(vel.x, vel.y);
    mAccelRange = accel;
}
void ParticleSprayer::Init(RNGRange vel, RNGRange accel) {
    mVelocityRange = vel;
    mAccelRange = accel;
}
void ParticleSprayer::SetXRotates(bool r) {
    mXRotates = r;
    for(auto& particle : mParticles) {
        particle.SetXRotates(r);
    }
}
void ParticleSprayer::SetYRotates(bool r) {
    mYRotates = r;
    for(auto& particle : mParticles) {
        particle.SetYRotates(r);
    }
}
void ParticleSprayer::SetZRotates(bool r) {
    mZRotates = r;
    for(auto& particle : mParticles) {
        particle.SetZRotates(r);
    }
}
void ParticleSprayer::SetRotates(bool x, bool y, bool z) {
    mXRotates = x;
    mYRotates = y;
    mZRotates = z;
    for(auto& particle : mParticles) {
        particle.SetXRotates(x);
        particle.SetYRotates(y);
        particle.SetZRotates(z);
    }
}
void ParticleSprayer::Update(double dt, glm::vec3 trans) {
    mCurrentFrame++;
    if(mResetFrame && mResetFrame == mCurrentFrame) {
        Reset();
        ResetSelf();
        mCurrentFrame = 0;
    }
    GenerateParticles();
    mFinished = true;
    for(auto it = mParticles.begin(); it != mParticles.end(); it++) {
        it->Update();
        if(!it->Alive()) {
            it = mParticles.erase(it);
        }
        if(it->Started() && mRenderer) {
            mRenderer->DrawParticle(*it, mColor, mPosition + trans, mTextureID);
            mFinished = false;
        }
        /*
        if(i < mGeneratedParticles) {
            //mParticles[i].Update();
            if(!mParticles[i].Alive() && !mNoRespawn && (!mFrameStep || (mCurrentFrame % mFrameStep == 0))) {
                if(mRegenParticles) {
                    (*it) = GenerateParticle(0);
                }
                else {
                    //mParticles[i].Reset(mPosition + mParticleSpawnPos);
                    //mParticles[i].Reset(mPosition + mParticleSpawnPos);
                }
            }
            else {
                if(mParticles[i].Started() && mRenderer) {
                    mRenderer->DrawParticle(mParticles[i], mColor, mPosition + trans, mTextureID);
                    mFinished = false;
                }
            }
        }
        */
    }

    size_t k = mParticles.size();
    for(size_t i = 0; i < mFixedParticles.size(); i++) {
        mFixedParticles[i].Update();
        if(!mFixedParticles[i].Alive()) {
            if(mCurrentFrame && (!mFrameStep || (mCurrentFrame % mFrameStep == 0))) {
                mFixedParticles[i].Reset();
            }
        }
        else {
            glm::vec3 vel = mFixedParticles[i].GetVelocity();
            if(mFixedParticles[i].Started() && mRenderer) {
                mRenderer->DrawParticle(mFixedParticles[i], mColor, mPosition + trans, mTextureID);
            }
        }
    }
    if(mRenderer && mAlpha > 0.0) {
        mRenderer->DrawParticle(*this, mColor, trans, mTextureID);
    }
}
void ParticleSprayer::CreateParticlesPath(ParticlePath p, int numParticles, int framesBetween) {
    mParticles.resize(numParticles);
    size_t i = 0;
    for(auto it = mParticles.begin(); it != mParticles.end(); it++, i++) {
        ParticlePath path;
        path.pos = p.pos;
        path.vel = p.vel + p.accel * (float)i;
        path.accel = p.accel;
        path.decel = p.decel;
        path.size = p.size + p.sizeStep * i;
        path.alpha = p.alpha + p.alphaStep * i;
        path.sizeStep = p.sizeStep;
        path.alphaStep = p.alphaStep;
        path.damping = p.damping;
        it->SetAttributes(path);
        it->SetSize(mSize);
        it->SetStartFrame(i * framesBetween);
        it->SaveInitialState();
        if(!mBaseAnimation.Empty()) {
            it->SetAnimation(mBaseAnimation);
        }
    }
    mRegenParticles = false;
    mGeneratedParticles = numParticles;
}
void ParticleSprayer::CreateParticlesPathArc(ParticlePath p, float startAngle, float endAngle, int numParticles, int startFrame, int framesBetween) {
    mParticles.resize(numParticles);
    float angle = startAngle;
    float angleStep = endAngle / numParticles;
    size_t i = 0;
    for(auto it = mParticles.begin(); it != mParticles.end(); it++, i++) {
        ParticlePath path;
        angle+= angleStep;
        glm::vec3 velocity(cos(angle) * p.vel.x, sin(angle) * p.vel.y, p.vel.z);
        path.pos = p.pos;
        path.vel = velocity;
        path.accel = p.accel;
        path.decel = p.decel;
        path.size = p.size + p.sizeStep * i * framesBetween;
        path.alpha = p.alpha + p.alphaStep * i * framesBetween;
        path.sizeStep = p.sizeStep;
        path.alphaStep = p.alphaStep;
        path.damping = p.damping;
        it->SetAttributes(path, startAngle);
        //mParticles[i + startSize].SetSize(mSize);
        it->SetStartFrame(startFrame + i * framesBetween);
        it->SetFallSpeed(mFallSpeed);
        it->SaveInitialState();
        if(!mBaseAnimation.Empty()) {
            it->SetAnimation(mBaseAnimation);
        }
    }
    mRegenParticles = false;
    mGeneratedParticles = numParticles;
}
void ParticleSprayer::GenerateParticles() {
    if(mActive) {
        if(!mFrameStep || (mCurrentFrame % mFrameStep == 0)) {
            int parts = mParticlesPerFrame;
            if(!parts) {
                parts = mMaxParticles;
            }
            for(int i = 0; i < parts; i++) {
                if(mGeneratedParticles + i >= mMaxParticles) {
                    return;
                }
                Particle p = GenerateParticle(mCurFrame);
                p.SetCurrentFrame(mCurFrame);
                p.SetStartFrame(mCurFrame);
                mParticles.push_back(p);
                //mParticles[mGeneratedParticles++] = p;
            }
        }
    }
}
Particle ParticleSprayer::GenerateParticle(int frame) {
    Particle p;

    int subFrame = 1;
    if(mParticlesPerFrame) {
        subFrame+= mGeneratedParticles / mParticlesPerFrame;
    }
    if(fabs(mAngleRange.min) > RADIAN || fabs(mAngleRange.max) > RADIAN) {
        float angle = Randf(mAngleRange.min, mAngleRange.max);
        float angleStep = Randf(mAngleStepRange.min, mAngleStepRange.max);
        float angleVel = Randf(mAngleVelRange.min, mAngleVelRange.max);
        glm::vec3 accel = RandVec3(mAccelRange.min, mAccelRange.max);
        glm::vec3 velocity = glm::vec3(cos(angle) * angleVel, sin(angle) * angleVel, 0.0f);
        if(velocity.x == 0.0 && velocity.y == 0.0) {
            velocity = glm::vec3(cos(angle) * angleVel, sin(angle) * angleVel, 0.0f);
        }
        glm::vec3 pos = velocity;
        p.Init(pos + mParticleSpawnPos, velocity, angle, angleStep);
        p.SetAngleVel(angleVel);
        p.SetRotateAngle(angleStep);
    }
    else {
        glm::vec3 velocity = RandVec3(mVelocityRange.min, mVelocityRange.max);
        if((glm::length(mVelocityRange.min) && glm::length(mVelocityRange.max)) && (velocity.x == 0.0 && velocity.y == 0.0 && velocity.z == 0.0)) {
            velocity = RandVec3(mVelocityRange.min, mVelocityRange.max);
        }
        glm::vec3 accel = RandVec3(mAccelRange.min, mAccelRange.max);
        glm::vec3 decel = RandVec3(mDecelRange.min, mDecelRange.max);
        glm::vec3 pos =  RandVec3(mPositionRange.min, mPositionRange.max);
        velocity = ConstrainMin3f(velocity, -mMinSpawnSpeed, mMinSpawnSpeed);
        if(fabs(mInfluenceAngle.x) < 0.01 || fabs(mInfluenceAngle.y) < 0.01) {
            glm::vec3 point = glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0);
            velocity = PointTowards(velocity, point);
            accel = PointTowards(accel, point);
            decel = PointTowards(decel, point);
            pos.x*= mInfluenceAngle.x;
        }
        if(length(pos) < 0.01) {
            pos = velocity;
        }
        float minDistance = mSpawnDistanceMin * subFrame;
        pos+= glm::normalize(pos) * Randf(mSpawnDistanceRange.min, mSpawnDistanceRange.max);
        if(glm::length(pos) < minDistance) {
            pos+= glm::normalize(pos) * minDistance;
        }
        p.Init(pos + mParticleSpawnPos, velocity, accel);
        p.SetDeceleration(decel);
    }
    p.SetStartFrame(frame);
    p.SetDamping(Randf(mDampingRange.min, mDampingRange.max));
    p.SetMaxSpeed(mMaxSpeed);
    p.SetAlpha(Randf(mAlphaRange.min, mAlphaRange.max));
    p.SetFadeStep(Randf(mFadeRange.min, mFadeRange.max));
    p.SetSize(Randf(mSizeRange.min, mSizeRange.max) + mScale);
    p.SetScaleMax(mScaleMax);
    p.SetFallSpeed(mFallSpeed);
    unsigned int desRange = Randui(mDestroyRange.min, mDestroyRange.max);
    p.SetDestroyFrame(desRange);
    p.SetScaleStep(Randf(mScaleStepRange.min, mScaleStepRange.max));
    p.SetPosition(p.GetPosition() + mPosition);
    p.SaveInitialState();
    //glm::vec3 ppos = p.GetPosition();
    //printf("%f, %f, %f\n", ppos.x, ppos.y, ppos.z);
    if(!mBaseAnimation.Empty()) {
        p.SetAnimation(mBaseAnimation);
        p.mAnimation.Start();
    }
    return p;
}
void ParticleSprayer::SetParticleAlphas(float a) {
    for(auto& part : mParticles) {
        part.SetAlpha(a);
    }
}
void ParticleSprayer::Reset() {
    ResetParticles();
    if(mResetsSelf) {
        ResetSelf();
    }
}
void ParticleSprayer::SetParticleAnimations(ParticleAnimation anim) {
    for(auto& p : mParticles) {
        p.SetAnimation(anim);
    }
}
void ParticleSprayer::SetParticleOffsets(glm::vec2 offset) {
    for(auto& p : mParticles) {
        p.SetTextureOffset(offset);
    }
}
void ParticleSprayer::ResetParticles() {
    if(mRegenParticles) {
        mGeneratedParticles = 0;
    }
    else {
        for(auto& part : mParticles) {
            part.Reset();
        }
    }
    for(auto& part : mFixedParticles) {
        part.Reset();
    }
}
void ParticleSprayer::ResetAllAttributes() {
    mVelocityRange = RNGRange(glm::vec3(-5.0f, -5.0f, 0.0f), glm::vec3(5.0f, 5.0f, 0.0f));
    mAccelRange = RNGRange(glm::vec3(0.0f), glm::vec3(0.0f));
    mSizeRange = ValueRangef(1.0f, 1.0f);
    mFallSpeed = 0;
    mParticlesPerFrame = 5;
    mResetFrame = 0;
    mCurrentFrame = 0;
    mDestroyRange = 0;
    mTextureID = -1;
    mScale = 1.0f;
}
void ParticleSprayer::AddVelocity(glm::vec3 vel) {
    for(auto& p : mParticles) {
        p.AddVelocity(vel);
    }
    for(auto& p : mFixedParticles) {
        p.AddVelocity(vel);
    }
    Particle::AddVelocity(vel);
}
bool ParticleSprayer::Finished() {
    if(mParticles.empty()) {
        return true;
    }
    if(!mNoRespawn || mResetFrame) {
        return false;
    }
    else {
        return mFinished;
    }
}
}
