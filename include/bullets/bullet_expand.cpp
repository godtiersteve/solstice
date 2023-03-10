#include "bullet_expand.h"

namespace solstice {
    ExpandingBullet::ExpandingBullet() {
        SetExpansion(5, 5, 0, 0, false);
        mType = BTYPE_EXPANDING;
    }
    ExpandingBullet::ExpandingBullet(float height, float width, float widthSpeed, float heightSpeed, bool moves) {
        SetExpansion(height, width, widthSpeed, heightSpeed, moves);
        mType = BTYPE_EXPANDING;
    }
    void ExpandingBullet::SetExpansion(float height, float width, float widthSpeed, float heightSpeed, bool moves) {
        mHeightSpeed = heightSpeed;
        mWidthSpeed = widthSpeed;
        mTargetHeight = height;
        mTargetWidth = width;
        mMovesAfterGrown = moves;
    }
    void ExpandingBullet::Update(double dt) {
        if(!mMovesAfterGrown || (mMovesAfterGrown && FinishedGrowing())) {
            if(mAlive) {
                Particle::Update(dt);
            }
            SetPosition(Vector2f(mPosition.x, mPosition.y));
            Vector2f center = mHitBox.GetCenter();
            if(mTransfersPosition) {
                mEmitter.SetPosition(glm::vec3(center.x, center.y, 0.0f));
            }
            if(mTransfersVelocity) {
                mEmitter.AddVelocity(glm::vec3(mVelocity.x, mVelocity.y, 0.0));
            }
            mEmitter.Update(dt, mPosition);
            if(mEmitsOnImpact && !mAlive) {
                mImpactEmitter.Update(dt, mPosition);
            }
            if(mRotatesSprite) {
                mSpriteAngle = atan2(mVelocity.y, mVelocity.x);
            }
        }
        UpdateAnimation(dt);
        float height = mHitBox.GetHeight();
        float width = mHitBox.GetWidth();
        if(mHeightSpeed > 0) {
            if(height < mTargetHeight) {
                height+= mHeightSpeed;
            }
        }
        else if(height > mTargetHeight) {
            height+= mHeightSpeed;
        }
        if(mWidthSpeed > 0) {
            if(width < mTargetWidth) {
                width+= mWidthSpeed;
            }
        }
        else if(width > mTargetWidth) {
            width+= mWidthSpeed;
        }
        mHitBox.SetHW(height, width);

    }
    bool ExpandingBullet::FinishedGrowing() {
        return mHitBox.GetHeight() >= mTargetHeight && mHitBox.GetWidth() >= mTargetWidth;
    }
    void ExpandingBullet::UpdateAnimation(float dt) {
        float h = 0, w = 0;
        if(mTargetHeight > mHitBox.GetHeight()) {
            h = mHeightSpeed;
        }
        if(mTargetWidth > mHitBox.GetWidth()) {
            w = mWidthSpeed;
        }
        for(size_t i = 0; i < mAnimations.size(); i++) {
            mAnimations[i].AddHW(h, w);
        }
        BulletBase::UpdateAnimation(dt);
    }
}
