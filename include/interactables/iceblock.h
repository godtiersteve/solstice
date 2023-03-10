#ifndef ICEBLOCK_H_INCLUDED
#define ICEBLOCK_H_INCLUDED

#include "interactables/physicsinteractable.h"
#include "engine/healthentity.h"
#include "physics/constraint.h"

namespace solstice {
    class IceBlock : public PhysicsInteractable {
    private:
        std::vector<RigidBody*> mContents;
    public:
        Constraint mConstraint;
        HealthEntity mHealth;
        IceBlock(float elasticity = 0.25);
        void Break();
        void Update(double dt);
        int AddObject(RigidBody* obj);
        void SetMaxBodies(size_t size) {mContents.resize(size);}
        size_t GetMaxBodies() {return mContents.size();}
        int ReleaseObject(size_t i) {mContents[i] = nullptr;}
    };

}


#endif // ICEBLOCK_H_INCLUDED
