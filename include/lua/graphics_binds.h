#ifndef GRAPHICS_BINDS_H_INCLUDED
#define GRAPHICS_BINDS_H_INCLUDED

#include "kaguya/kaguya.hpp"
#include "graphics/graphics.h"
#include "animation/animator.h"
#include "animation/animation.h"

namespace solstice {
    void SetupGraphicsBinds(kaguya::State& state);
    void SetupAnimationBinds(kaguya::State& state);
    void Setup2DGraphicsBinds(kaguya::State& state);
}
#endif // GRAPHICS_BINDS_H_INCLUDED
