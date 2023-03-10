#ifndef PRESSURESWITCH_H_INCLUDED
#define PRESSURESWITCH_H_INCLUDED

#include "switch.h"

namespace solstice {
    class PressureSwitch : public Switch {
    public:
        float mMinimumWeight;
    };
}

#endif // PRESSURESWITCH_H_INCLUDED
