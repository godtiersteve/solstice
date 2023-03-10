#ifndef DAMAGESWITCH_H_INCLUDED
#define DAMAGESWITCH_H_INCLUDED

#include "switch.h"
#include "engine/damagetype.h"
#include "engine/healthentity.h"

namespace solstice {
    class DamageSwitch : public Switch {
    public:
        DamageType mDamageType;
        HealthEntity mHealth;
    };
}

#endif // DAMAGESWITCH_H_INCLUDED
