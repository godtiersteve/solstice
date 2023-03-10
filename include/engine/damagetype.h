#ifndef DAMAGETYPE_H_INCLUDED
#define DAMAGETYPE_H_INCLUDED

namespace solstice {
    enum DamageType {
        DAMAGE_HOT = 1,
        DAMAGE_WATER = 2,
        DAMAGE_SHOCK = 4,
        DAMAGE_COLD = 8,
        DAMAGE_WIND = 16
    };

}

#endif // DAMAGETYPE_H_INCLUDED
