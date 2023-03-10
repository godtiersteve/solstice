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
    mParticles.resize(amount);
    mAlphas.resize(amount);
    mSizes.resize(amount);
    mPositions.resize(amount);
    mParticlePositions.resize(amount);
    mTextureOffsets.resize(amount);
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
void ParticleSprayer::Update(double dt) {
    mCurrentFrame++;
    if(mResetFrame && mResetFrame == mCurrentFrame) {
        Reset();
        ResetSelf();
        mCurrentFrame = 0;
    }
    GenerateParticles();
    mFinished = true;
    for(size_t i = 0; i < mGeneratedParticles; i++) {
        if(i < mGeneratedParticles) {
            mParticles[i].Update();
            if(!mParticles[i].Alive() && !mNoRespawn) {
                mAlphas[i] = 0.0f;
                if(mRegenParticles) {
                    mParticles[i] = GenerateParticle(0);
                }
                else {
                    mParticles[i].Reset(mPosition + mParticleSpawnPos);
                }
            }
            else {
                glm::vec3 vel = mParticles[i].GetVelocity();
                if(!mParticles[i].Started()) {
                    mAlphas[i] = 0.0f;
                }
                else if(mRenderer) {
                    mRenderer->DrawParticle(mParticles[i], mPosition);
                    mFinished = false;
                }
                else {
                    mAlphas[i] = mParticles[i].GetAlpha();
                    mPositions[i] = mParticles[i].GetPosition();
                    mPositions[i].x*= mXNorm;
                    mPositions[i].y*= mYNorm;
                    mSizes[i] = mParticles[i].GetSize();
                    mTextureOffsets[i] = mParticles[i].GetTextureOffsets();
                    mFinished = false;
                }
            }
        }
        else {
            mAlphas[i] = 0.0f;
        }
    }

    size_t k = mParticles.size();
    for(size_t i = 0; i < mFixedParticles.size(); i++) {
        mFixedParticles[i].Update();
        if(!mFixedParticles[i].Alive()) {
            mAlphas[k + i] = 0.0f;
            if(mCurrentFrame && (!mFrameStep || (mCurrentFrame % mFrameStep == 0))) {
                mFixedParticles[i].Reset();
            }
        }
        else {
            glm::vec3 vel = mFixedParticles[i].GetVelocity();
            if(!mFixedParticles[i].Started()) {
                mAlphas[k + i] = 0.0f;
            }
            else if(mRenderer) {
                mRenderer->DrawParticle(mFixedParticles[i], mPosition);
            }
            else {
                mAlphas[k + i] = mFixedParticles[i].GetAlpha();
                mAlphas[k + i] = mFixedParticles[i].GetAlpha();
                mPositions[k + i] = glm::vec3((mPosition + mFixedParticles[i].GetPosition()));
                mPositions[k + i].x*= mXNorm;
                mPositions[k + i].y*= mYNorm;
                mSizes[k + i] = mFixedParticles[i].GetSize();
                mTextureOffsets[k + i] = mFixedParticles[i].GetTextureOffsets();
                mFinished = false;
            }
        }
    }
    if(mRenderer) {
        mRenderer->DrawParticle(*this);
    }
    else {
        glm::vec3 pos = glm::vec3(mPosition.x * mXNorm, mPosition.y * mYNorm, mPosition.z);
        mPositions.back() = pos;
        mAlphas.back() = mAlpha;
        mSizes.back() = mSize;
        mTextureOffsets.back() = mTextureOffset;
    }
}
void ParticleSprayer::SetupBuffers() {
    if(mParticles.empty()) {
        return;
    }
    if(!glIsBuffer(mSizeBuffer)) {
        glGenBuffers(1, &mSizeBuffer);
    }
    if(!glIsBuffer(mAlphaBuffer)) {
        glGenBuffers(1, &mAlphaBuffer);
    }
    if(!glIsBuffer(mPositionBuffer)) {
        glGenBuffers(1, &mPositionBuffer);
    }
    if(!glIsBuffer(mClipBuffer)) {
        glGenBuffers(1, &mClipBuffer);
    }
    if(!glIsVertexArray(mVao)) {
        glGenVertexArrays(1, &mVao);
    }
    size_t buffSize = mParticles.size() + mFixedParticles.size() + 1;
    mSizes = std::vector<float>(buffSize, 1.0f);
    mAlphas = std::vector<float>(buffSize, 0.0f);
    mPositions = std::vector<glm::vec3>(buffSize);
    mTextureOffsets = std::vector<glm::vec2>(buffSize);
    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * mPositions.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mSizeBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * mSizes.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mAlphaBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * mAlphas.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mClipBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec2) * mTextureOffsets.size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, mAlphaBuffer);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, mSizeBuffer);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, mClipBuffer);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void ParticleSprayer::CleanBuffers() {
    glDeleteBuffers(1, &mSizeBuffer);
    glDeleteBuffers(1, &mAlphaBuffer);
    glDeleteBuffers(1, &mPositionBuffer);
    glDeleteVertexArrays(1, &mVao);
}
void ParticleSprayer::Draw3D(ShaderProgram& shader) {
    if(!glIsVertexArray(mVao) || !mGeneratedParticles) {
        return;
    }
    glBindVertexArray(mVao);
    shader.SetUniform1i("tex", 0);
    shader.SetUniform2f("clipSize", mTextureClip.x, mTextureClip.y);
    shader.SetUniform4f("color", mColor.red, mColor.green, mColor.blue, mColor.alpha);
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec3) * mPositions.size(),
                    &mPositions[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mSizeBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(float) * mSizes.size(),
                    &mSizes[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mAlphaBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(float) * mAlphas.size(),
                    &mAlphas[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mClipBuffer);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::vec2) * mTextureOffsets.size(),
                    &mTextureOffsets[0]);
    glBlendFunc(mBlendSrc, mBlendDest);
    glDrawArrays(GL_POINTS, 0, mPositions.size());
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(0);
}
void ParticleSprayer::CreateParticlesPath(ParticlePath p, int numParticles, int framesBetween) {
    mParticles.resize(numParticles);
    for(int i = 0; i < numParticles; i++) {
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
        mParticles[i].SetAttributes(path);
        mParticles[i].SetSize(mSize);
        mParticles[i].SetStartFrame(i * framesBetween);
        mParticles[i].SaveInitialState();
        if(!mBaseAnimation.Empty()) {
            mParticles[i].SetAnimation(mBaseAnimation);
        }
    }
    mRegenParticles = false;
    mGeneratedParticles = numParticles;
}
void ParticleSprayer::CreateParticlesPathArc(ParticlePath p, float startAngle, float endAngle, int numParticles, int startFrame, int framesBetween) {
    size_t startSize = mParticles.size();
    mParticles.resize(startSize + numParticles);
    float angle = startAngle;
    float angleStep = endAngle / numParticles;
    for(int i = 0; i < numParticles; i++) {
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
        mParticles[i + startSize].SetAttributes(path, startAngle);
        //mParticles[i + startSize].SetSize(mSize);
        mParticles[i + startSize].SetStartFrame(startFrame + i * framesBetween);
        mParticles[i + startSize].SetFallSpeed(mFallSpeed);
        mParticles[i + startSize].SaveInitialState();
        if(!mBaseAnimation.Empty()) {
            mParticles[i + startSize].SetAnimation(mBaseAnimation);
        }
    }
    mRegenParticles = false;
    mGeneratedParticles = numParticles;
}
void ParticleSprayer::GenerateParticles() {
    if(!mFrameStep || (mCurrentFrame % mFrameStep == 0)) {
        int parts = mParticlesPerFrame;
        if(!parts) {
            parts = mParticles.size();
        }
        for(int i = 0; i < parts; i++) {
            if(mGeneratedParticles + i >= mParticles.size()) {
                return;
            }
            Particle p = GenerateParticle(mCurFrame);
            p.SetCurrentFrame(mCurFrame);
            mParticles[mGeneratedParticles++] = p;
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
        if(mInfluenceAngle.x || mInfluenceAngle.y) {

            if(!SameSign(velocity.x, mInfluenceAngle.x) && fabs(mInfluenceAngle.x) > fabs(mInfluenceAngle.y)) {
                //velocity.x = - velocity.x;
            }
            if(!SameSign(velocity.y, mInfluenceAngle.y) && fabs(velocity.y) > fabs(mInfluenceAngle.y)) {
                //velocity.y = -velocity.y;
            }
            //velocity.x*= mInfluenceAngle.x;
            //velocity.y*= mInfluenceAngle.y;
            //float length = glm::length(velocity);
            //velocity = glm::normalize(velocity) + glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0);
            //velocity*= length * 0.5;
            glm::vec3 point = glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0);
            velocity = PointTowards(velocity, point);
            accel = PointTowards(accel, point);
            decel = PointTowards(decel, point);
            pos.x*= mInfluenceAngle.x;
            //pos.y*= mInfluenceAngle.y;

            //velocity = glm::length(velocity) * glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0);
            //accel = glm::length(accel) * glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0);
            //decel = glm::length(decel) * glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0);
            //pos =  glm::length(pos) * glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0);
            /*
            float length = glm::length(velocity);
            velocity =  glm::normalize(glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0) * length + velocity) * length;
            length = glm::length(accel);
            accel = glm::normalize(glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0) * length + accel) * length;
            length = glm::length(decel);
            if(length) {
                decel = glm::normalize(glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0) * length + decel) * length;
            }
            length = glm::length(pos);
            */
            //pos = glm::normalize(glm::vec3(mInfluenceAngle.x, mInfluenceAngle.y, 0.0) * length + pos) * length;
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
    if(!mBaseAnimation.Empty()) {
        p.SetAnimation(mBaseAnimation);
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
        for(auto& alpha : mAlphas) {
            alpha = 0.0f;
        }
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
void ParticleSprayer::Draw(SceneRenderer& rend) {
    for(size_t i = 0; i < mGeneratedParticles; i++) {
        Particle& part = mParticles[i];
        if(part.Alive()) {
            Color c = mColor;
            c.alpha = part.GetAlpha();
            if(mTextureID < 0) {
                rend.DrawRect(Rectanglef(Vector2f(part.GetPosition().x, part.GetPosition().y), part.GetSize(), part.GetSize(), DRAW_CENTER),
                              c);
            }
            else {
                QuadData q(Vector2f(part.GetPosition().x, part.GetPosition().y), part.GetSize(), part.GetSize(), mTextureID);
                q.SetDrawFrom(DRAW_CENTER);

                rend.DrawQuad(q, c);
            }
        }
    }
}
bool ParticleSprayer::Finished() {
    if(!mNoRespawn || mResetFrame) {
        return false;
    }
    else {
        return mFinished;
    }
}
}
