#include "rigidbodycontainer.h"

namespace solstice {
RigidBodyContainer::RigidBodyContainer(size_t maxObjects) {
    mRigidBodies.resize(maxObjects, nullptr);
    mLastUsedIndex = 0;
    mWeight = 0;
}

bool RigidBodyContainer::AddObject(RigidBody* b) {
    if(!Contains(b)) {
        FindNextIndex();
        if(mLastUsedIndex < mRigidBodies.size()) {
            mRigidBodies[mLastUsedIndex++] = b;
            mWeight+= b->GetMass();
            return true;
        }
    }
    return false;
}
void RigidBodyContainer::FindNextIndex() {
    if(mRigidBodies[mLastUsedIndex]) {
        for(size_t i = 0; i < mRigidBodies.size(); i++) {
            if(!mRigidBodies[i]) {
                mLastUsedIndex = i;
                return;
            }
        }
    }
    mLastUsedIndex = mRigidBodies.size();
}
bool RigidBodyContainer::RemoveObject(RigidBody* b) {
    for(size_t i = 0; i < mRigidBodies.size(); i++) {
        if(mRigidBodies[i] == b) {
            mLastUsedIndex = i;
            mRigidBodies[i] = nullptr;
            mWeight-= b->GetMass();
            return true;
        }
    }
    return false;
}
void RigidBodyContainer::Clear() {
    for(size_t i = 0; i < mRigidBodies.size(); i++) {
        mRigidBodies[i] = nullptr;
    }
}
}
