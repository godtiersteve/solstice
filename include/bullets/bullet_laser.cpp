#include "bullet_laser.h"

namespace solstice {
    LaserPart::LaserPart(int spaceBetween, float maxDist, float speed) {
        mStep = 0;
        mSpaceBetween = spaceBetween;
        mMaxDistance = maxDist;
        mAngle = 0;
        if(mSpaceBetween <= 0) {
            mSpaceBetween = 50;
        }
        if(mMaxDistance <= 0) {
            mMaxDistance = 300;
        }
        mSpeed = speed;
        mXScale = 1.0;
        mYScale = 1.0;
        mDisplacement = 0;
    }
    void LaserPart::Update(double dt) {
        mStep++;
        float speed = mSpeed + mDisplacement;
        if(speed == 0) {
            speed = 1;
        }
        if(mStep > mMaxDistance / mSpeed - 1) {
            mStep = 0;
        }
        mAnimation.Update(dt);
        int numParts = mMaxDistance / mSpaceBetween + 1;
        if(mDrawQuads.size() != numParts) {
            mDrawQuads.resize(numParts);
            mInitialPositions.clear();
            for(size_t i = 0; i < mDrawQuads.size(); i++) {
                glm::vec2 pos = RandVec2(glm::vec2(mXPosRange.min, mYPosRange.min),
                                         glm::vec2(mXPosRange.max, mYPosRange.max));
                mInitialPositions.push_back(Vector2f(pos.x, pos.y));
                mScales.push_back(Vector2f(mXScale, mYScale));
            }
        }
        for(size_t i = 0; i < mDrawQuads.size(); i++) {
            Vector2f offset;
            offset.x = mStep * mSpeed + i * mSpaceBetween + mAnimation.GetWidth() * 0.5;
            if(offset.x > mMaxDistance) {
                mScales[i] = 1.0;
            }
            else {
                mScales[i].x*= mXScale;
                mScales[i].y*= mYScale;
            }
            offset.x = fmod(offset.x, mMaxDistance);
            offset = offset + mInitialPositions[i];
            offset.Rotate(mAngle);
            mDrawQuads[i] = mPosition + offset;
        }
    }
    void LaserPart::SetAngle(float a) {
        mAngle = a;
        mAnimation.SetAngle(mAngle);
    }
    void LaserPart::Draw(SceneRenderer& rend) {
        QuadData data = mAnimation.GetDrawQuad();
        for(size_t i = 0; i < mDrawQuads.size(); i++) {
            data.SetPosition(mDrawQuads[i]);
            data.SetHW(data.GetHeight() * mScales[i].y, data.GetWidth() * mScales[i].x);
            rend.DrawQuad(data);
        }
    }
    Laser::Laser() {
        mStartup = 0;
        mActive = 30;
        mRecovery = 0;
        mMaxWidth = 800;
        mHitBox.SetWidth(mMaxWidth);
        mCollisionType = 0;
        mCollidesWidth = PDESTROY_OBJECT | PDESTROY_GROUND;
        mAlive = true;
        mGrowthSpeed = 5;
        mCurrentFrame = 0;
        mStartWidth = 800;
        mEdgeHeight = 10;
        mLifeTimer.mLoops = -1;
        mLifeTimer.mDuration = 1.0;
        mType = BTYPE_LASER;
        mShrinking = false;
        mShrinkRate = 0.7;
        mScale = 1.0f;
        mInitScale = 1.0f;
        mLaserHeight = 0;
    }
    Laser::Laser(Vector2f origin, float height, float width, float growSpeed) {
        mStartup = 0;
        mActive = 30;
        mRecovery = 0;
        mMaxWidth = width;
        mCollisionType = 0;
        mHitBox.Init(origin, height, width);
        mCollidesWidth = PDESTROY_OBJECT | PDESTROY_GROUND;
        mAlive = true;
        mGrowthSpeed = growSpeed;
        mCurrentFrame = 0;
        mStartWidth = width;
        mEdgeHeight = 10;
        mLifeTimer.mLoops = -1;
        mLifeTimer.mDuration = 1.0;
        mType = BTYPE_LASER;
        mShrinking = false;
        mShrinkRate = 0.7;
        mScale = 1.0f;
        mInitScale = 1.0f;
        mLaserHeight = height;
    }
    void Laser::SetFrameData(int startup, int active, int recovery) {
        mStartup = startup;
        mActive = active;
        mRecovery = recovery;
    }
    void Laser::Update(double dt) {
        if(mAlive && !mLifeTimer.Finished()) {
            mLifeTimer.Update(dt);
            mCurrentFrame++;
            float width = mHitBox.GetWidth();
            if(mCollisionType) {
                width = mClosestContact.DistanceSqrt(mHitBox.GetPosition()) + 10;
                if(width > mMaxWidth) {
                    width = mMaxWidth;
                }
            }
            else {
                if(mGrowthSpeed > 0) {
                    if(mCurrentFrame > mStartup && mHitBox.GetWidth() < mMaxWidth) {
                        width = mHitBox.GetWidth() + mGrowthSpeed;
                    }
                }
                else {
                    width = mMaxWidth;
                }
            }

            mHitBox.SetWidth(width);
            UpdateAnimation(dt);
            for(size_t i = 0; i < mLaserParts.size(); i++) {
                mLaserParts[i].mPosition = mHitBox.GetPosition();
                mLaserParts[i].mMaxDistance = mHitBox.GetWidth();
                mLaserParts[i].SetAngle(mAngle);
                mLaserParts[i].mDisplacement = mVelocity.x;
                mLaserParts[i].Update(dt);
            }
            mCollisionType = 0;
        }
        else if(mLifeTimer.Finished() && mLifeTimer.mDuration > 0) {
            mShrinking = true;
        }
        if(mShrinking && mScale > 0.001) {
            mScale*= mShrinkRate;
        }
    }
    bool Laser::Finished() {
        if(mAlive) {
            if(mLifeTimer.Finished() && mScale <= 0.001) {
                return true;
            }
            return false;
        }
        else {
            return true;
        }
    }
    void Laser::UpdateAnimation(float dt) {
        if(!mStartupAnimation.Finished()) {
            mStartupAnimation.SetPosition(mHitBox.GetPosition());
            mStartupAnimation.Update(dt);
        }
        else {
            mBeamQuad.SetXYHW(mHitBox.GetPosition(), mLaserHeight, mHitBox.GetWidth());
            mTopEdgeQuad.SetXYHW(mBeamQuad.GetTopLeft(), mEdgeHeight, mHitBox.GetWidth());
            mBottomEdgeQuad.SetXYHW(mBeamQuad.GetBotLeft(), mEdgeHeight, mHitBox.GetWidth());
            Vector2f originOffset(1, 0);
            originOffset.Rotate(mAngle);
            mOriginQuad.SetPosition(mHitBox.GetLeftCenter() + originOffset);
            mSpawnAnimation.SetPosition(mHitBox.GetPosition());
            mSpawnAnimation.Update(dt);
        }
    }
    void Laser::SetPosition(glm::vec3 pos) {
        mHitBox.SetPosition(Vector2f(pos.x, pos.y));
        mPosition = pos;
    }
    void Laser::SetPosition(Vector2f pos) {
        mHitBox.SetPosition(pos);
        mPosition = glm::vec3(pos.x, pos.y, 0);
    }
    void Laser::UpdatePositions() {

    }
    bool Laser::CheckAndResolveCollision(BackgroundTile2& ground) {
        if(ground.GetType() == TILE_PLATFORM || mLifeTimer.Finished()) {
            return false;
        }
        if(mAlive && (mCollidesWidth & PDESTROY_WALL) || (mCollidesWidth & PDESTROY_GROUND)) {
            Collision c = CheckCollision(&mHitBox, &ground.GetHitBox());
            if(c) {
                mCollisionFound = true;
                if(!mCollisionType) {
                    mClosestContact = ground.GetHitBox().GetCenter();
                }
                else if(GetHitBoxPos().Distance(ground.GetHitBox().GetCenter()) < GetHitBoxPos().Distance(mClosestContact)) {
                    mClosestContact = mClosestContact = ground.GetHitBox().GetCenter();
                }
                if(fabs(c.normal.x) >= 0.75) {
                    mCollisionType|= PDESTROY_WALL;
                }
                if(fabs(c.normal.y) >= 0.25) {
                    mCollisionType|= PDESTROY_GROUND;
                }
                return true;
            }
        }
        return false;
    }
    bool Laser::CheckAndResolveCollision(RigidBody* b) {
        if(!b || mLifeTimer.Finished()) {
            return false;
        }
        bool colFound = false;
        if(mAlive && (mCollidesWidth & PDESTROY_OBJECT)) {
            Collision c = CheckCollision(&mHitBox, &b->GetHitBox());
            if(c) {
                if(!mCollisionType) {
                    mClosestContact = b->GetHitBox().GetCenter();
                }
                else if(GetHitBoxPos().Distance(b->GetHitBox().GetCenter()) < GetHitBoxPos().Distance(mClosestContact)) {
                    mClosestContact = b->GetHitBox().GetCenter();
                }
                mCollisionType|= PDESTROY_OBJECT;
                colFound = true;
            }
        }
        return colFound;
    }
    bool Laser::CheckAndResolveCollision(WaterBody& b) {
        if(mLifeTimer.Finished()) {
            return false;
        }
        return false;
    }
    void Laser::AimTowards(Vector2f pos) {
        Vector2f diff = pos - mHitBox.GetPosition();
        //mHitBox.RotateAboutOrigin(atan2(diff.y, diff.x));
        SetAngle(atan2(diff.y, diff.x));
    }
    void Laser::SetAngle(float a) {
        mHitBox.RotateAboutOrigin(a);
        mAngle = mHitBox.GetAngle();
        Vector2f origin = mHitBox.GetPosition();
        mTopEdgeQuad.RotateAboutPoint(mAngle, origin);
        mBottomEdgeQuad.RotateAboutPoint(mAngle, origin);
        mBeamQuad.RotateAboutPoint(mAngle, origin);
        mOriginQuad.RotateAboutOrigin(mAngle);
    }
    void Laser::Rotate(float angle) {
        mHitBox.RotateAboutOrigin(mHitBox.GetAngle() + angle);
    }
    void Laser::Reset() {
        mCurrentFrame = 0;
        mHitBox.SetWidth(mStartWidth);
        BulletBase::Reset();
        mShrinking = false;
        mScale = mInitScale;
        mLifeTimer.Reset();
    }
    void Laser::SetupTexture(float textureSize, float celSize, float edgeSize, float laserHeight, int xOffset, int yOffset, int textureID) {
        celSize = celSize;
        mSpawnAnimation.SetTexHW(textureSize, textureSize);
        mSpawnAnimation.AutoAddFramesSheet(3, celSize * 2, celSize * 2, yOffset, xOffset + 1);
        mSpawnAnimation.SetHeight(celSize * 2);
        mSpawnAnimation.SetWidth(celSize * 2);
        mSpawnAnimation.SetDrawFrom(DRAW_LEFTCENTER);
        mSpawnAnimation.SetDuration(0.035);
        mSpawnAnimation.Start();
        mSpawnAnimation.mRotatesAboutPoint = true;

        mTopEdgeQuad.SetTextureCoords(Vector2f(0, celSize - edgeSize), edgeSize, 1, textureSize, textureSize);
        mBottomEdgeQuad.SetTextureCoords(Vector2f(0, celSize - edgeSize), edgeSize, 1, textureSize, textureSize);
        mBottomEdgeQuad.SetVertFlip(true);
        mBeamQuad.SetTextureCoords(Vector2f(xOffset + celSize + 3, yOffset), celSize - 3, 2, textureSize, textureSize);
        mOriginQuad.SetTextureCoords(Vector2f(xOffset, yOffset + celSize * 2 - 1), celSize * 2, celSize * 2, textureSize, textureSize);

        mBeamQuad.SetHeight(laserHeight);
        mLaserHeight = laserHeight;
        mBeamQuad.SetTextureID(textureID);
        mBottomEdgeQuad.SetTextureID(textureID);
        mTopEdgeQuad.SetTextureID(textureID);
        mOriginQuad.SetTextureID(textureID);
        mSpawnAnimation.SetTextureID(textureID);

        mSpawnAnimation.SetDrawFrom(DRAW_CENTER);
        mBeamQuad.SetDrawFrom(DRAW_LEFTCENTER);
        mTopEdgeQuad.SetDrawFrom(DRAW_BOTLEFT);
        mOriginQuad.SetDrawFrom(DRAW_RIGHTCENTER);
        mBottomEdgeQuad.SetDrawFrom(DRAW_TOPLEFT);
        mEdgeHeight = edgeSize;
        mOriginQuad.SetXYHW(mHitBox.GetLeftCenter(), celSize * 2, celSize * 2);
        mHitBox.SetHeight(mLaserHeight + mEdgeHeight * 2);
    }
    void Laser::Scale(float x) {
        mSpawnAnimation.SetHW(mSpawnAnimation.GetHeight() * x, mSpawnAnimation.GetWidth() * x);
        mOriginQuad.SetHW(mOriginQuad.GetHeight() * x, mOriginQuad.GetWidth() * x);
        mLaserHeight*= x;
        mEdgeHeight*= x;
        mHitBox.SetHeight(mLaserHeight + mEdgeHeight * 2);
        mTopEdgeQuad.SetHeight(mEdgeHeight);
        mBottomEdgeQuad.SetHeight(mEdgeHeight);
        for(size_t i = 0; i < mLaserParts.size(); i++) {
            mLaserParts[i].mAnimation.SetClip(mLaserParts[i].mAnimation.GetClip().x * x, mLaserParts[i].mAnimation.GetClip().y * x);
        }
    }
    void Laser::SetWidths(float start, float max, int step) {
        if(start <= 0) {
            mStartWidth = max;
            mGrowthSpeed = 0;
        }
        else {
            mStartWidth = start;
            mGrowthSpeed = step;
        }
        mMaxWidth = max;
        SetWidth(mStartWidth);
    }
    void Laser::Draw(SceneRenderer& rend) {
        if(mStartupAnimation.Finished() || mStartupAnimation.Empty()) {
            QuadData origin, beam;
            origin = mOriginQuad;
            beam = mBeamQuad;
            origin.SetHeight(origin.GetHeight() * mScale);
            beam.SetHeight(beam.GetHeight() * mScale);
            rend.DrawQuad(origin);
            rend.DrawQuad(beam);
            QuadData spawnQuad = mSpawnAnimation.GetDrawQuad();
            spawnQuad.SetHeight(spawnQuad.GetHeight() * mScale);
            spawnQuad.RotateAboutPoint(mAngle, mHitBox.GetPosition());
            if(!mShrinking) {
                rend.DrawQuad(mTopEdgeQuad);
                rend.DrawQuad(mBottomEdgeQuad);
                for(size_t i = 0; i < mLaserParts.size(); i++) {
                    mLaserParts[i].Draw(rend);
                }
            }
        }
        else {
            rend.DrawQuad(mStartupAnimation.GetDrawQuad());
        }
        //rend.DrawRect(mHitBox, Color(1.0f, 0.0, 0.0, 0.25));
    }
    void Laser::DrawHitBoxes(SceneRenderer& rend, Color col) {
        rend.DrawRect(mHitBox, col);
        Rectanglef border = mHitBox;
        border.SetHeight(mEdgeHeight);
        border.SetDrawFrom(DRAW_BOTLEFT);
        border.SetPosition(mHitBox.GetBotLeft());
        rend.DrawRect(border, COL_BLUE);
        border.SetDrawFrom(DRAW_TOPLEFT);
        border.SetPosition(mHitBox.GetTopLeft());
        rend.DrawRect(border, COL_BLUE);
    }
}
