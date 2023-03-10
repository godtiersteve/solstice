#include "bulletentity.h"

namespace solstice {
void BulletEntity::AddMod(std::shared_ptr<GunMod> mod)
{
    mMods.push_back(mod);
}

void BulletEntity::RemoveMode(size_t i)
{
    mMods.erase(mMods.begin() + i);
}

void BulletEntity::RemoveMod(std::shared_ptr<GunMod> mod)
{
    mMods.erase(std::find(mMods.begin(), mMods.end(), mod));
}

void BulletEntity::Update(double dt) {
    if(mBullet) {
        for(size_t i = 0; i < mMods.size(); i++) {
            mMods[i]->OnUpdate(dt, mBullet.get());
        }
        mBullet->Update(dt);
    }
}

int BulletEntity::GetDamage() {
    return mBullet ? mBullet->mDamage * GetDamageMod() : 0;
}
Vector2f BulletEntity::GetKnockback() {
    if(!mBullet) {
        return 0;
    }
    Vector2f mod = GetKnockbackMod();
    mod.x*= mBullet->mKnockback.x;
    mod.y*= mBullet->mKnockback.y;
    return mod;
}
Vector2f BulletEntity::GetKnockbackMod() {
    Vector2f kb = 1.0;
    for(size_t i = 0; i < mMods.size(); i++) {
        kb.x*= mMods[i]->mKnockbackModifier.x;
        kb.y*= mMods[i]->mKnockbackModifier.y;
    }
    return kb;
}
float BulletEntity::GetSpeedMod() {
    float mod = 1.0;
    for(size_t i = 0; i < mMods.size(); i++) {
        mod *= mMods[i]->mSpeedModifier;
    }
    return mod;
}
float BulletEntity::GetDamageMod() {
    float mod = 1.0;
    for(size_t i = 0; i < mMods.size(); i++) {
        mod *= mMods[i]->mDamageModifier;
    }
    return mod;
}
float BulletEntity::GetAccelMod() {
    float mod = 1.0;
    for(size_t i = 0; i < mMods.size(); i++) {
        mod *= mMods[i]->mAccelModifier;
    }
    return mod;
}
float BulletEntity::GetDecelMod() {
    float mod = 1.0;
    for(size_t i = 0; i < mMods.size(); i++) {
        mod *= mMods[i]->mDecelModifier;
    }
    return mod;
}
}
