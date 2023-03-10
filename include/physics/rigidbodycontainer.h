#ifndef RIGIDBODYCONTAINER_H_INCLUDED
#define RIGIDBODYCONTAINER_H_INCLUDED

#include "rigidbody.h"

namespace solstice {
    class RigidBodyContainer {
    protected:
        std::vector<RigidBody*> mRigidBodies;
        size_t mLastUsedIndex;
        float mWeight;
        void FindNextIndex();
    public:
        RigidBodyContainer(size_t maxObjects = 10);
        bool AddObject(RigidBody* b);
        bool RemoveObject(RigidBody* b);
        RigidBody* GetObject(size_t i) {return mRigidBodies[i];}
        bool Contains(RigidBody* b) {return std::find(mRigidBodies.begin(), mRigidBodies.end(), b) != mRigidBodies.end();}
        float GetWeight() {return mWeight;}
        void Clear();

    };
}

#endif // RIGIDBODYCONTAINER_H_INCLUDED
