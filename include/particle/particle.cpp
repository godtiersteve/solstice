#include "particle.h"
namespace solstice {
Particle::Particle() {
    Init(glm::vec3(0), glm::vec3(0), glm::vec3(0));
}
Particle::Particle(glm::vec3 pos, glm::vec3 startSpeed, glm::vec3 accel) {
    Init(pos, startSpeed, accel);
}
Particle::Particle(glm::vec3 pos, glm::vec3 startSpeed, float angle, float accel) {
    Init(pos, startSpeed, angle, accel);
}
void Particle::Init(glm::vec3 pos, glm::vec3 startSpeed, glm::vec3 accel) {
    mRotateAngle = 0;
    mAngle = 0;
    mCurFrame = 0;
    mDestroyFrame = 0;
    mMaxSpeed = glm::vec3(10);
    mPosition = glm::vec3(0);
    mVelocity = glm::vec3(0);
    mAcceleration = glm::vec3(0);
    mDeceleration = glm::vec3(0);
    mStopVelocity = glm::vec3(0);
    mFallSpeed = 0;
    mSize = 5;
    mAlpha = 1.0f;
    mFadeStep = 0.0f;
    mScaleStep = 0.0f;
    mScaleMax = 1.0f;
    mInitialState.size = mSize;
    mDestroyed = false;
    mDamping = 1.0f;
    mAngleVel = startSpeed.x;
    mResetFrame = -1;
    mTextureOffset = glm::vec2(0.0f, 0.0f);
    mStartFrame = 0;
    mXRotates = false;
    mYRotates = false;
    mZRotates = false;
    mDecelsToStop = false;
    mCurAnimFrame = 0;
    mCurAnimFrameStep = 0;
    mAnimColumn = 0;
    mAnimRow = 0;
    mNumFrames = 0;
    mCollidesWidth = PDESTROY_BACKGROUND | PDESTROY_ENEMY | PDESTROY_OBJECT;
    mDestroyed = PDESTROY_NONE;
    mFrameClipSize = glm::vec2(1.0);
    SetAttributes(pos, startSpeed, accel);
    mVerticallyOriented = false;
    mDampingX = 1.0;
    mDampingY = 1.0;
    mDampingZ = 1.0;
    mBlendType = BLEND_NORMAL;
}
void Particle::Init(glm::vec3 pos, glm::vec3 startSpeed, float angle, float accel) {
    Init(pos, startSpeed, glm::vec3(0.0f, 0.0f, 0.0f));
    mRotateAngle = angle;
    mInitialState.rotateAngle = angle;
    mInitialState.angleVel = mAngleVel;
    mInitialState.rotateAngle = mRotateAngle;
    mXRotates = true;
    mYRotates = true;
    mZRotates = false;
    mVerticallyOriented = false;
}
void Particle::SetAttributes(glm::vec3 pos, glm::vec3 startSpeed, float angle, float accel) {
    SetAttributes(pos, startSpeed, glm::vec3(0.0f, 0.0f, 0.0f));
    mRotateAngle = angle;
    mInitialState.angleVel = mAngleVel;
    mInitialState.rotateAngle = mRotateAngle;
    mXRotates = true;
    mYRotates = true;
    mZRotates = false;
}
void Particle::SetAttributes(glm::vec3 pos, glm::vec3 startSpeed, glm::vec3 accel) {
    mPosition = pos;
    mVelocity = startSpeed;
    mAcceleration = accel;
    mInitialState.velocity = startSpeed;
    mInitialState.position = pos;
    mXRotates = false;
    mYRotates = false;
    mZRotates = false;
}
void Particle::SetAttributes(ParticlePath p) {
    //glm::vec3 pos, accel, decel, vel;
    //float size, sizeStep, alpha, alphaStep;
    mPosition = p.pos;
    mAcceleration = p.accel;
    mDeceleration = p.decel;
    mVelocity = p.vel;
    SetSize(p.size);
    mScaleStep = p.sizeStep;
    mAlpha = p.alpha;
    mFadeStep = p.alphaStep;
    mDamping = p.damping;
    SaveInitialState();
}
void Particle::SetAttributes(ParticlePath p, float angle) {
    mPosition = p.pos;
    mAcceleration = p.accel;
    mDeceleration = p.decel;
    mVelocity = p.vel;
    SetSize(p.size);
    mScaleStep = p.sizeStep;
    mAlpha = p.alpha;
    mFadeStep = p.alphaStep;
    mDamping = p.damping;
    mAngle = angle;
    mRotateAngle = angle;
    mAngleVel = mVelocity.x;
    SaveInitialState();
}
void Particle::SaveInitialState() {
    mInitialState.position = mPosition;
    mInitialState.velocity = mVelocity;
    mInitialState.rotateAngle = mRotateAngle;
    mInitialState.angle = mAngle;
    mInitialState.angleVel = mAngleVel;
    mInitialState.size = mSize;
    mInitialState.alpha = mAlpha;
}
void Particle::SetDeceleration(glm::vec3 dec) {
    mDeceleration.x = fabs(dec.x);
    mDeceleration.y = fabs(dec.y);
}
Particle::Particle(const ParticleState& state) {
    *this = state;
}
Particle& Particle::operator=(const ParticleState& state) {
    mPosition = state.position;
    mVelocity = state.velocity;
    mRotateAngle = state.rotateAngle;
    mAngle = state.angle;
    mAngleVel = state.angleVel;
    mSize = state.size;
    mAlpha = state.alpha;

    return *this;
}
void Particle::Update(float t) {
    if(Alive()) {
        mCurFrame++;
        if(Started()) {
            if((mDestroyFrame && mCurFrame >= mDestroyFrame + mStartFrame) ||
               (mAlpha <= 0) || (mSize <= 0.0f))
            {
                mDestroyed = true;
                mCurFrame = 0;
                return;
            }
            else if(mResetFrame > -1 && mCurFrame >= mResetFrame) {
                Reset();
                mCurFrame = 0;
            }
            glm::vec3 decel = mDeceleration;
            if(mRotateAngle || mAngleVel) {
                mAngle+= mRotateAngle;
                mAngleVel+= mAcceleration.x;
                float x = cos(mAngle);
                float y = sin(mAngle);
                if(mXRotates) {
                    mVelocity.x = mAngleVel * x;
                }
                else {
                    mVelocity.x+= mAcceleration.x;
                }
                if(mYRotates) {
                    mVelocity.y = mAngleVel * y;
                }
                else {
                    mVelocity.y+= mAcceleration.y;
                }
                decel.x*= x;
                decel.y*= y;
            }
            else {
                glm::vec3 accel = mAcceleration;
                mVelocity+= mAcceleration;
            }
            decel.x = fabs(decel.x);
            decel.y = fabs(decel.y);
            if(mDecelsToStop) {
                if(mVelocity.x > 0) {
                    mVelocity.x = std::max(0.0f, mVelocity.x - decel.x);
                }
                if(mVelocity.x < 0) {
                    mVelocity.x = std::min(0.0f, mVelocity.x + decel.x);
                }
                if(mVelocity.y > 0) {
                    mVelocity.y = std::max(0.0f, mVelocity.y - decel.y);
                }
                if(mVelocity.y < 0) {
                    mVelocity.y = std::min(0.0f, mVelocity.y + decel.y);
                }
                if(mVelocity.z > 0) {
                    mVelocity.z = std::max(0.0f, mVelocity.z - decel.z);
                }
                if(mVelocity.z < 0) {
                    mVelocity.z = std::min(0.0f, mVelocity.z + decel.z);
                }
            }
            else {
                mVelocity-= decel;
            }
            mVelocity.y-= mFallSpeed;
            mVelocity*= mDamping;
            mAngleVel*= mDamping;
            mVelocity.x*= mDampingX;
            mVelocity.y*= mDampingY;
            mVelocity.z*= mDampingZ;
            mVelocity = Constrain3f(mVelocity, -mMaxSpeed, mMaxSpeed);
            CheckVelocity();
            if(!mVerticallyOriented) {
                mPosition+= mVelocity;
            }
            else {
                mPosition.x+= mVelocity.y;
                mPosition.y+= mVelocity.x;
                mPosition.z+= mVelocity.z;
                printf("Velocity: %f, %f\n", mVelocity.x, mVelocity.y);
            }
            mAlpha-= mFadeStep;
            if(mAlpha < 0.0) {
                mAlpha = 0.0;
            }
            mSize+= mScaleStep;
            if(mSize > mScaleMax) {
                mSize = mScaleMax;
            }
            mAnimation.Update(t);
            if(!mAnimation.Empty()) {
                mTextureOffset.x = mAnimation.GetCurrentFrame().col;
                mTextureOffset.y = mAnimation.GetCurrentFrame().row;
            }
        }
    }
}
void Particle::SetPosition(glm::vec3 pos) {
    mPosition = pos;
}
void Particle::CheckVelocity() {
    if(mVelocity.x > mMaxSpeed.x) {
        mVelocity.x = mMaxSpeed.x;
    }
    else if(mVelocity.x < -mMaxSpeed.x) {
        mVelocity.x = -mMaxSpeed.x;
    }
    if(mVelocity.y > mMaxSpeed.y) {
        mVelocity.y = mMaxSpeed.y;
    }
    else if(mVelocity.y < -mMaxSpeed.y) {
        mVelocity.y = -mMaxSpeed.y;
    }

}
void Particle::SwapSpeeds() {
    glm::vec3 acc = mAcceleration;
    glm::vec3 dec = mDeceleration;
    mAcceleration.y = acc.x;
    mAcceleration.x = acc.y;
    mDeceleration.x = dec.y;
    mDeceleration.y = dec.x;
}
void Particle::Reset() {
    if(mCurFrame>= mStartFrame) {
        *this = mInitialState;
        mStartFrame = abs(mStartFrame);
        mCurFrame = mStartFrame;
        mDestroyed = false;
    }
    else {
        mCurFrame++;
    }
}
void Particle::Reset(ParticleState initialState) {
    mInitialState = initialState;
    Reset();
}
void Particle::Reset(glm::vec3 pos) {
    *this = mInitialState;
    mPosition = pos + mVelocity;
    mStartFrame = abs(mStartFrame);
    mCurFrame = mStartFrame;
    mDestroyed = false;
}
bool Particle::Alive() {
    return !mDestroyed;
}
bool Particle::Started() {
    return mStartFrame < mCurFrame;
}
}
