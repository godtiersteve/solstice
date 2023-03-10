#include "bullet_melee.h"

namespace solstice {
MeleeBullet::MeleeBullet()
{
    mCurrentBulletFrameNum = -1;
    mType = BTYPE_MELEE;
    mHitStun = 0.1;
    mHitStop = 0.1;
    mHitBox.SetDrawFrom(DRAW_CENTER);
    mMeleeBulletType = MELEEBULLET_COMBO;
}
MeleeBullet::MeleeBullet(Animation meleeAnim) {
    meleeAnim.SetLoops(0);
    meleeAnim.Reset();
    meleeAnim.Start();
    SetAnimation(meleeAnim);
    mCurrentBulletFrameNum = -1;
    mHitStun = 0.1;
    mHitStop = 0.1;
    mHitBox.SetDrawFrom(DRAW_CENTER);
    mType = BTYPE_MELEE;
    mMeleeBulletType = MELEEBULLET_COMBO;
}
MeleeBullet::MeleeBullet(std::vector<MeleeBulletFrame> bulletFrames, Animation meleeAnim) {
    InitAnimation(bulletFrames, meleeAnim);
    mCurrentBulletFrameNum = -1;
    mHitStun = 0.1;
    mHitStop = 0.1;
    mHitBox.SetDrawFrom(DRAW_CENTER);
    mType = BTYPE_MELEE;
    mMeleeBulletType = MELEEBULLET_COMBO;
}
void MeleeBullet::InitAnimation(std::vector<MeleeBulletFrame> bulletFrames, Animation meleeAnim) {
    if(meleeAnim.Size() == bulletFrames.size()) {
        mMeleeAnimation = meleeAnim;
        mBulletFrames = bulletFrames;
    }
}
void MeleeBullet::SetHitBoxes(HitBoxf hitbox, size_t i) {
    if(i < mBulletFrames.size()) {
        mBulletFrames[i].mHitBox = hitbox;
    }
}
void MeleeBullet::InitBulletFrame(MeleeBulletFrame f, size_t i) {
    if(i < mBulletFrames.size()) {
        mBulletFrames[i] = f;
    }
}
void MeleeBullet::Update(double dt) {
    BulletBase::Update(dt);
    UpdateAnimation(dt);
}

void MeleeBullet::UpdateAnimation(float dt) {
    BulletBase::UpdateAnimation(dt);
    mMeleeAnimation.Update(dt);
    mMeleeAnimation.SetPosition(GetHitBoxPos());
}
void MeleeBullet::SetAnimation(Animation anim) {
    mMeleeAnimation = anim;
    mMeleeAnimation.Start();
    mBulletFrames.resize(anim.size());
}
void MeleeBullet::Draw(SceneRenderer& rend) {
    BulletBase::Draw(rend);
    rend.DrawQuad(mMeleeAnimation.GetDrawQuad());
}
void MeleeBullet::DrawHitBoxes(SceneRenderer& rend, Color col) {
    BulletBase::DrawHitBoxes(rend, col);
    HitBoxf hbox = mMeleeAnimation.GetCurFrame().mHitBox;
    rend.DrawHitBox(hbox, col);
    if(!mBulletFrames.empty()) {
        rend.DrawHitBox(mCurrentBulletFrame.mHitBox, col);
    }
}
void MeleeBullet::SetPosition(glm::vec3 pos) {
    mMeleeAnimation.SetPosition(Vector2f(pos.x, pos.y));
    BulletBase::SetPosition(pos);
}
void MeleeBullet::SetPosition(Vector2f pos) {
    mMeleeAnimation.SetPosition(pos);
    BulletBase::SetPosition(pos);
}
bool MeleeBullet::CheckAndResolveCollision(BackgroundTile2& ground) {return false;}
bool MeleeBullet::CheckAndResolveCollision(RigidBody* b) {
    bool collision = false;
    if(!mCollisionFound && mMeleeAnimation.GetCurFrameNum() >= 1) {
        collision = CheckCollision(&mHitBox, &b->GetHitBox());
        if(collision) {
            mCollisionFound = true;
            mMeleeAnimation.Pause(mHitStop);
            Vector2f kb = mKnockback;
            if(mMeleeAnimation.GetHoriFlipped()) {
                kb.x = -kb.x;
            }
            b->ApplyImpulse(kb);
        }
    }
    return collision;
}
bool MeleeBullet::CheckAndResolveCollision(WaterBody& b) {
    return false;
}
}
