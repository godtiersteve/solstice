#ifndef GUNMOD_H_INCLUDED
#define GUNMOD_H_INCLUDED

#include "bullettype.h"
#include "bullets/bulletbase.h"

namespace solstice {
enum GunModType {GUNMOD_SPREAD, GUNMOD_BURST, GUNMOD_SINK, GUNMOD_FLOAT};
class GunMod {
protected:
    int mModType;
public:
    int mModID;
    float mDamageModifier, mSpeedModifier, mAccelModifier, mDecelModifier;
    Vector2f mKnockbackModifier;
    GunMod() {
        mModID = 0;
        mModType = -1;
        mDamageModifier = 1.0;
        mSpeedModifier = 1.0;
        mAccelModifier = 1.0;
        mDecelModifier = 1.0;
    }

    virtual void OnSpawn(BulletBase* b);
    virtual void OnUpdate(double dt, BulletBase* b);

    int GetModType() {return mModType;}
};
}

#endif // GUNMOD_H_INCLUDED
