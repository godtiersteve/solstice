#include "collision_interop.h"
namespace solstice {
void SetupHitDetectionBinds(kaguya::State& state) {
    state["Collision"].setClass(kaguya::UserdataMetatable<Collision>()
                                .setConstructors<Collision(), Collision(float), Collision(bool)>()
                                .addProperty("mtv", &Collision::mtv)
                                .addProperty("normal", &Collision::normal)
                                .addProperty("contactPoint", &Collision::contactPoint)
                                .addProperty("edge1", &Collision::e1)
                                .addProperty("edge2", &Collision::e2)
                                .addProperty("clipEdge", &Collision::clipEdge)
                                .addProperty("minEdge", &Collision::minEdge)
                                .addFunction("GetEdges", &Collision::GetEdges)
                                .addFunction("hasCollision", &Collision::hasCollision)
                                .addFunction("GetContactPoint", &Collision::GetContactPoint)
                                .addFunction("Empty", &Collision::Empty)
                                );
}
}
