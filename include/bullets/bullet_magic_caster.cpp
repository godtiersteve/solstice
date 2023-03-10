#include "bullet_magic_caster.h"

namespace solstice {
MagicCaster::MagicCaster() {
    SetMagic(nullptr, nullptr, nullptr);
}

MagicCaster::MagicCaster(std::shared_ptr<BulletSpawner> levelOne, std::shared_ptr<BulletSpawner> levelTwo, std::shared_ptr<BulletSpawner> levelThree) {
    SetMagic(levelOne, levelTwo, levelThree);
}
MagicCaster::MagicCaster(const std::vector<std::shared_ptr<BulletSpawner> >& levelOne,
                      const std::vector<std::shared_ptr<BulletSpawner> >& levelTwo,
                      const std::vector<std::shared_ptr<BulletSpawner> >& levelThree) {
    SetMagic(levelOne, levelTwo, levelThree);
}
void MagicCaster::Update(double dt) {
    for(size_t i = 0; i < mLevelOneMagic.size(); i++) {
        mLevelOneMagic[i]->Update(dt);
    }
    for(size_t i = 0; i < mLevelTwoMagic.size(); i++) {
        mLevelTwoMagic[i]->Update(dt);
    }
    for(size_t i = 0; i < mLevelThreeMagic.size(); i++) {
        mLevelThreeMagic[i]->Update(dt);
    }
}

void MagicCaster::CastSpell(MagicCasterLevel magicLevel) {
    switch(magicLevel) {
    case MAG_LVL_1:
        for(size_t i = 0; i < mLevelOneMagic.size(); i++) {
            mLevelOneMagic[i]->Reset();
            mLevelOneMagic[i]->Start();
            mLevelOneMagic[i]->mSpawnPosition = mSpawnPosition + mSpawnOffset;
        }
        break;
    case MAG_LVL_2:
        for(size_t i = 0; i < mLevelTwoMagic.size(); i++) {
            mLevelTwoMagic[i]->Reset();
            mLevelTwoMagic[i]->Start();
            mLevelTwoMagic[i]->mSpawnPosition = mSpawnPosition + mSpawnOffset;
        }
        break;
    case MAG_LVL_3:
        for(size_t i = 0; i < mLevelThreeMagic.size(); i++) {
            mLevelThreeMagic[i]->Reset();
            mLevelThreeMagic[i]->Start();
            mLevelThreeMagic[i]->mSpawnPosition = mSpawnPosition + mSpawnOffset;
        }
        break;
    default:
        break;
    }
}
void MagicCaster::SetMagic(std::shared_ptr<BulletSpawner> levelOne, std::shared_ptr<BulletSpawner> levelTwo, std::shared_ptr<BulletSpawner> levelThree) {
    mLevelOneMagic.clear();
    mLevelTwoMagic.clear();
    mLevelThreeMagic.clear();
    if(levelOne) {
        mLevelOneMagic.push_back(levelOne);
    }
    if(levelTwo) {
        mLevelTwoMagic.push_back(levelTwo);
    }
    if(levelThree) {
        mLevelThreeMagic.push_back(levelThree);
    }
    for(size_t i = 0; i < mLevelOneMagic.size(); i++) {
        mLevelOneMagic[i]->Stop();
    }
    for(size_t i = 0; i < mLevelTwoMagic.size(); i++) {
        mLevelTwoMagic[i]->Stop();
    }
    for(size_t i = 0; i < mLevelThreeMagic.size(); i++) {
        mLevelThreeMagic[i]->Stop();
    }
}
void MagicCaster::SetMagic(const std::vector<std::shared_ptr<BulletSpawner> >& levelOne,
                      const std::vector<std::shared_ptr<BulletSpawner> >& levelTwo,
                      const std::vector<std::shared_ptr<BulletSpawner> >& levelThree)
{
    mLevelOneMagic = levelOne;
    mLevelTwoMagic = levelTwo;
    mLevelThreeMagic = levelThree;
    for(size_t i = 0; i < mLevelOneMagic.size(); i++) {
        mLevelOneMagic[i]->Stop();
    }
    for(size_t i = 0; i < mLevelTwoMagic.size(); i++) {
        mLevelTwoMagic[i]->Stop();
    }
    for(size_t i = 0; i < mLevelThreeMagic.size(); i++) {
        mLevelThreeMagic[i]->Stop();
    }
}
void MagicCaster::Draw(SceneRenderer& rend) {
    for(size_t i = 0; i < mLevelOneMagic.size(); i++) {
        mLevelOneMagic[i]->Draw(rend);
    }
    for(size_t i = 0; i < mLevelTwoMagic.size(); i++) {
        mLevelTwoMagic[i]->Draw(rend);
    }
    for(size_t i = 0; i < mLevelThreeMagic.size(); i++) {
        mLevelThreeMagic[i]->Draw(rend);
    }
}
void MagicCaster::DrawHitBoxes(SceneRenderer& rend, Color color) {
    for(size_t i = 0; i < mLevelOneMagic.size(); i++) {
        mLevelOneMagic[i]->DrawHitBoxes(rend, color);
    }
    for(size_t i = 0; i < mLevelTwoMagic.size(); i++) {
        mLevelTwoMagic[i]->DrawHitBoxes(rend, color);
    }
    for(size_t i = 0; i < mLevelThreeMagic.size(); i++) {
        mLevelThreeMagic[i]->DrawHitBoxes(rend, color);
    }
}
bool MagicCaster::CheckAndResolveCollision(BackgroundTile2& ground) {
    for(size_t i = 0; i < mLevelOneMagic.size(); i++) {
        mLevelOneMagic[i]->CheckAndResolveCollision(ground);
    }
    for(size_t i = 0; i < mLevelTwoMagic.size(); i++) {
        mLevelTwoMagic[i]->CheckAndResolveCollision(ground);
    }
    for(size_t i = 0; i < mLevelThreeMagic.size(); i++) {
        mLevelThreeMagic[i]->CheckAndResolveCollision(ground);
    }
}
bool MagicCaster::CheckAndResolveCollision(RigidBody* b) {
    for(size_t i = 0; i < mLevelOneMagic.size(); i++) {
        mLevelOneMagic[i]->CheckAndResolveCollision(b);
    }
    for(size_t i = 0; i < mLevelTwoMagic.size(); i++) {
        mLevelTwoMagic[i]->CheckAndResolveCollision(b);
    }
    for(size_t i = 0; i < mLevelThreeMagic.size(); i++) {
        mLevelThreeMagic[i]->CheckAndResolveCollision(b);
    }
}
bool MagicCaster::CheckAndResolveCollision(WaterBody& b) {
    for(size_t i = 0; i < mLevelOneMagic.size(); i++) {
        mLevelOneMagic[i]->CheckAndResolveCollision(b);
    }
    for(size_t i = 0; i < mLevelTwoMagic.size(); i++) {
        mLevelTwoMagic[i]->CheckAndResolveCollision(b);
    }
    for(size_t i = 0; i < mLevelThreeMagic.size(); i++) {
        mLevelThreeMagic[i]->CheckAndResolveCollision(b);
    }
}
}
