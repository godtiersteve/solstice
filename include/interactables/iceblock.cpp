#include "iceblock.h"

namespace solstice {
    IceBlock::IceBlock(float elasticity) {
        mFriction = 0.0;
        mElasticity = elasticity;
        mContents = std::vector<RigidBody*>(10, nullptr);
        mInteractableType = INTERACT_ICEBLOCK;
    }
    void IceBlock::Break() {
        for(size_t i = 0; i < mContents.size(); i++) {
            mContents[i] = nullptr;
        }
    }
    void IceBlock::Update(double dt) {
        float mass = mInvMass;
        mInvMass = 0;
        for(size_t i = 0; i < mContents.size(); i++) {
            if(mContents[i]) {
                mContents[i]->Stop();
            }
            mConstraint.Solve(this, mContents[i]);
        }
        mInvMass = mass;
        PhysicsBox2f::Update(dt);
    }
    int IceBlock::AddObject(RigidBody* obj) {
        for(size_t i = 0; i < mContents.size(); i++) {
            if(!mContents[i]) {
                mContents[i] = obj;
                return i;
            }
        }
        return -1;
    }
}
