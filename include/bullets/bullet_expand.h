#ifndef BULLET_EXPAND_H_INCLUDED
#define BULLET_EXPAND_H_INCLUDED

#include "bulletbase.h"

namespace solstice {
    class ExpandingBullet : public BulletBase {
    private:
    public:
        float mTargetHeight, mTargetWidth;
        float mWidthSpeed, mHeightSpeed;
        bool mMovesAfterGrown;
        ExpandingBullet();
        ExpandingBullet(float height, float width, float widthSpeed, float heightSpeed, bool moves = false);
        void SetExpansion(float height, float width, float widthSpeed, float heightSpeed, bool moves = false);
        void Update(double dt);
        bool FinishedGrowing();
        void UpdateAnimation(float dt);

    };
}

#endif // BULLET_EXPAND_H_INCLUDED
