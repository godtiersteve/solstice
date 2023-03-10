#ifndef PHYSICS_BINDS_H_INCLUDED
#define PHYSICS_BINDS_H_INCLUDED

#include "kaguya/kaguya.hpp"
#include "player/playerwatershed.h"
#include "physics/physics.h"
#include "physics/crate.h"
#include "environment/room.h"
#include "geometry/spatialgrid.h"

namespace solstice {
    void SetupPhysicsBinds(kaguya::State& state);
}
#endif // PHYSICS_BINDS_H_INCLUDED
