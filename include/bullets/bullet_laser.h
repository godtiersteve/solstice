#ifndef BULLET_LASER_H_INCLUDED
#define BULLET_LASER_H_INCLUDED

#include "bulletbase.h"
#include "engine/timerentity.h"

namespace solstice {
    class LaserPart {
    private:
        std::vector<Vector2f> mDrawQuads;
        std::vector<Vector2f> mInitialPositions;
        std::vector<Vector2f> mScales;
        int mStep;
        float mAngle;
    public:
        Vector2f mPosition;
        float mDisplacement;
        ValueRangef mXPosRange, mYPosRange;
        float mXScale, mYScale;
        Animation mAnimation;
        float mSpaceBetween, mMaxDistance, mSpeed;
        LaserPart(int spaceBetween = 30, float maxDist = 200, float speed = 15);
        void Update(double dt);
        void Draw(SceneRenderer& rend);
        void SetAngle(float a);
        void Reset() {mStep = 0;}
    };
    class Laser : public BulletBase {
    protected:
        Vector2f mClosestContact;
        int mCurrentFrame;
        float mScale;
        void UpdatePositions();
    public:
        int mStartup, mActive, mRecovery;
        int mGrowthSpeed;
        int mStartWidth, mMaxWidth;
        Animation mSpawnAnimation, mOriginAnimation, mStartupAnimation;
        QuadData mTopEdgeQuad, mBottomEdgeQuad, mBeamQuad, mOriginQuad;
        float mEdgeHeight, mLaserHeight;
        TimerEntity mLifeTimer;
        Vector2f mSpawnOffset;
        std::vector<LaserPart> mLaserParts;
        float mMotionAnimSpeed;
        float mShrinkRate, mInitScale;
        bool mShrinking;

        Laser();
        Laser(Vector2f origin, float height, float width, float growSpeed = -1);
        void SetWidth(float w) {mHitBox.SetWidth(w);}
        void SetWidths(float start, float max, int step);
        void SetFrameData(int startup, int active, int recovery);
        void Update(double dt);
        void UpdateAnimation(float dt);
        bool CheckAndResolveCollision(BackgroundTile2& ground);
        bool CheckAndResolveCollision(RigidBody* b);
        bool CheckAndResolveCollision(WaterBody& b);
        void AimTowards(Vector2f pos);
        void Rotate(float angle);
        void Shrink(float f) {mHitBox.SetWidth(mHitBox.GetWidth() - f);}
        void Reset();
        void SetupTexture(float textureSize, float celSize, float edgeSize, float laserHeight, int xOffset, int yOffset, int textureID);
        void Scale(float x);
        void Draw(SceneRenderer& rend);
        void DrawHitBoxes(SceneRenderer& rend, Color col);
        bool Finished();
        void SetAngle(float angle);
        bool Shrink() {mShrinking = true;}
        void SetPosition(glm::vec3 pos);
        void SetPosition(Vector2f pos);
        Vector2f GetHitBoxPos() {return mHitBox.GetPosition();}
    };
}

#endif // BULLET_LASER_H_INCLUDED
