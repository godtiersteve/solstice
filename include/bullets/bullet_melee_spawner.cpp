#include "bullet_melee_spawner.h"

namespace solstice {
BulletMeleeSpawner::BulletMeleeSpawner () {
    mFinisherCollision = false;
}
BulletMeleeSpawner::BulletMeleeSpawner (std::vector<MeleeBullet> baseBullets) {
    mFinisherCollision = false;
}
void BulletMeleeSpawner::UpdateBullets(double dt) {
    if(!mStarted) {
        return;
    }
    mRespawnTimer.Update(dt);
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        (*it)->Update(dt);
        if((*it)->Finished()) {
            if(!mReusesBullets) {
                it = mBullets.erase(it);
            }
            else {
                it = mBullets.erase(it);
            }
        }
    }
    for(auto it = mFinishers.begin(); it != mFinishers.end(); it++) {
        it->SetPosition(mFinisherTarget);
        it->Update(dt);
        if(it->Finished()) {
            it = mFinishers.erase(it);
        }
    }
}
void BulletMeleeSpawner::CreateSlash(size_t i, bool flipped) {
    if(i < mBaseBullets.size()) {
        mBaseBullets[i].mMeleeAnimation.SetHoriFlip(flipped);
        mBullets.push_back(std::make_shared<MeleeBullet>(mBaseBullets[i]));
        mBullets.back()->SetPosition(mSpawnPosition);
    }
}
bool BulletMeleeSpawner::CheckAndResolveCollision(RigidBody* b) {
    bool col = false;
    for(auto it = mBullets.begin(); it != mBullets.end(); it++) {
        col = (*it)->CheckAndResolveCollision(b);
        if((*it)->GetType() == BTYPE_MELEE) {
            MeleeBullet& bullet = *static_cast<MeleeBullet*>((*it).get());
            if(col && bullet.mMeleeBulletType == MELEEBULLET_FINISHER) {
                mFinisherCollision = true;
                mFinisherTarget = b->GetHitBox().GetCenter();
                mFinisherBase.SetPosition(mFinisherTarget);
            }
        }
        col = false;
    }
    return col;
}

void BulletMeleeSpawner::DoFinisher() {
    if(mFinishers.empty()) {
        mFinishers.push_back(mFinisherBase);
        mFinishers.back().Start();
        mFinishers.back().SetPosition(mSpawnPosition);
        mFinisherCollision = false;
    }
}
void BulletMeleeSpawner::Draw(SceneRenderer& rend) {
    BulletSpawner::Draw(rend);
    for(auto it = mFinishers.begin(); it != mFinishers.end(); it++) {
        it->Draw(rend);
    }
}
void BulletMeleeSpawner::SetPositions(Vector2f pos) {
    mSpawnPosition = pos;
    for(auto iter = mBullets.begin(); iter != mBullets.end(); iter++) {
        (*iter)->SetPosition(pos);
    }
    for(auto it = mFinishers.begin(); it != mFinishers.end(); it++) {
        it->SetPosition(pos);
    }
    //mFinisherBase.SetPosition(pos);
}
void BulletMeleeSpawner::EndFinisher() {
    for(auto it = mFinishers.begin(); it != mFinishers.end(); it++) {
        it->EndFlurry();
    }
}
void BulletMeleeSpawner::ResetFinisherReps() {
    for(auto it = mFinishers.begin(); it != mFinishers.end(); it++) {
        it->ResetReps();
    }
}
bool BulletMeleeSpawner::FinisherEnding() {
    if(mFinishers.empty()) {
        return false;
    }
    return mFinishers.back().GetFinishing();
}
int BulletMeleeSpawner::GetFinisherReps() {
    if(mFinishers.empty()) {
        return -1;
    }
    return mFinishers.back().GetRepsPerformed();
}
}
