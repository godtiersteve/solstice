#ifndef BULLET_MELEE_H_INCLUDED
#define BULLET_MELEE_H_INCLUDED

#include "bulletbase.h"
#include "engine/timerentity.h"
#include "geometry/hitbox.h"

namespace solstice {
    enum MeleeBulletType {MELEEBULLET_COMBO, MELEEBULLET_FINISHER};
    struct MeleeBulletFrame {
        HitBoxf mHitBox;
        int mDamage;
    };
    class MeleeBullet : public BulletBase {
    protected:
        std::vector<MeleeBulletFrame> mBulletFrames;
        MeleeBulletFrame mCurrentBulletFrame;
        int mCurrentBulletFrameNum;
    public:
        MeleeBulletType mMeleeBulletType;
        Animation mMeleeAnimation;
        MeleeBullet();
        MeleeBullet(std::vector<MeleeBulletFrame> bulletFrames, Animation meleeAnim);
        MeleeBullet(Animation meleeAnim);
        void InitAnimation(std::vector<MeleeBulletFrame> bulletFrames, Animation meleeAnim);
        void SetAnimation(Animation anim);
        void SetHitBoxes(HitBoxf hitbox, size_t i);
        void InitBulletFrame(MeleeBulletFrame f, size_t i);
        void Update(double dt);
        void UpdateAnimation(float dt);
        void Draw(SceneRenderer& rend);
        void DrawHitBoxes(SceneRenderer& rend, Color col);
        bool Finished() {return mMeleeAnimation.Finished();}
        void Reset() {BulletBase::Reset(); mMeleeAnimation.Reset(); mCollisionFound = false;}
        void SetPosition(glm::vec3 pos);
        void SetPosition(Vector2f pos);
        bool CheckAndResolveCollision(BackgroundTile2& ground);
        bool CheckAndResolveCollision(RigidBody* b);
        bool CheckAndResolveCollision(WaterBody& b);

    };

}


#endif // BULLET_MELEE_H_INCLUDED
