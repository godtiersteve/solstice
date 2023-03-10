#include "bullet_flurry.h"

namespace solstice {
    FlurryBullet::FlurryBullet(int numSlashes) {
        mMaxSlashes = numSlashes;
        mCreateDelay = 0;
        mAlive = true;
        mType = BTYPE_FLURRY;
        mFinishing = false;
        mRepsPerformed = 0;
    }
    FlurryBullet::FlurryBullet(std::vector<Animation>& slashAnimations, int numSlashes) {
        mAnimations = slashAnimations;
        mMaxSlashes = numSlashes;
        mAlive = true;
        mType = BTYPE_FLURRY;
        mFinishing = false;
        mRepsPerformed = 0;
    }

    void FlurryBullet::Shoot(Vector2f velocity) {
        mAlive = true;
    }

    void FlurryBullet::Shoot(Vector2f velocity, Vector2f position) {
        mAlive = true;
    }

    void FlurryBullet::Destroy(Vector2f force) {
        mAlive = false;
        mSlashAnimations.clear();
    }

    void FlurryBullet::Update(double dt) {
        if(!mFinishing && mSlashAnimations.empty() && !mAnimations.empty() && mAlive) {
            for(size_t i = 0; i < mMaxSlashes; i++) {
                mSlashAnimations.push_back(NextSlash());
            }
        }
        if(mFinishing) {
            mSlashAnimations.clear();
        }
        UpdateAnimation(dt);

    }
    Animation FlurryBullet::NextSlash(int id) {
        Animation anim;
        if(!mAnimations.empty()) {
            if(id < 0) {
                id = Randi(0, mAnimations.size() - 1);
            }
            else {
                id = (id + 1) % mAnimations.size();
            }
            float angle = Randi(-5, 5) * PI * 0.2;
            anim = mAnimations[id];
            anim.SetPosition(GetHitBoxPos());
            anim.SetAngle(angle);
            anim.Reset();
            anim.Start();
            anim.SetID(id);
        }
        return anim;
    }

    void FlurryBullet::UpdateAnimation(float dt) {
        for(auto it = mSlashAnimations.begin(); it != mSlashAnimations.end(); it++) {
            it->Update(dt);
            if(it->Finished() && !mFinishing) {
                int id = (it->GetID() + 1) % mAnimations.size();
                it->CopyFrames(mAnimations[id]);
                float angle = Randi(-10, 10) * PI * 0.1;
                it->SetAngle(angle);
                it->Reset();
                it->SetID(id);
                it->SetPosition(mHitBox.GetPosition());
            }
        }
        if(!mFinishing) {
            mRepsPerformed++;
        }
        else {
            mFinisher.Update(dt);
            mFinisher.SetPosition(mHitBox.GetPosition());
        }
    }
    void FlurryBullet::Reset() {
        BulletBase::Reset();
        mFinishing = false;
        mFinisher.Reset();
        mRepsPerformed = 0;
    }
    int FlurryBullet::GetRepsPerformed() {return mRepsPerformed;}
    void FlurryBullet::Draw(SceneRenderer& rend) {
        for(auto it = mSlashAnimations.begin(); it != mSlashAnimations.end(); it++) {
            rend.DrawQuad(it->GetDrawQuad());
        }
        if(mFinishing) {
            mFinisher.Draw(rend);
        }
    }

    void FlurryBullet::DrawHitBoxes(SceneRenderer& rend, Color col) {

    }
    void FlurryBullet::EndFlurry() {
        mFinishing = true;
        mRepsPerformed = 0;
    }
    bool FlurryBullet::Finished() {
        if(mFinishing && mFinisher.Finished()) {
            return true;
        }
        return false;
    }
    bool FlurryBullet::CheckAndResolveCollision(RigidBody* b) {
        if(mFinisher) {
            return mFinisher.CheckAndResolveCollision(b);
        }
        return false;
    }
}
