#ifndef BULLETENTITY_H_INCLUDED
#define BULLETENTITY_H_INCLUDED

#include "bulletbase.h"
#include "gunmod.h"
#include <memory>
#include <vector>
#include <algorithm>

namespace solstice {
class BulletEntity {
protected:
    std::vector<std::shared_ptr<GunMod> > mMods;
public:
    std::shared_ptr<BulletBase> mBullet;

    void AddMod(std::shared_ptr<GunMod> mod);
    void RemoveMode(size_t i);
    void RemoveMod(std::shared_ptr<GunMod> mod);

    int GetDamage();
    Vector2f GetKnockback();
    Vector2f GetKnockbackMod();
    float GetSpeedMod();
    float GetDamageMod();
    float GetAccelMod();
    float GetDecelMod();

    bool HasPierce();

    bool CheckCollision();

    void Update(double dt);
};

}

#endif // BULLETENTITY_H_INCLUDED
