#ifndef CONSTRAINT_H_INCLUDED
#define CONSTRAINT_H_INCLUDED

#include "rigidbody.h"

namespace solstice {
class Constraint {
protected:
    float mMinLength, mStiffness, mDamping;
public:
    size_t mNumIter;
    RigidBody* mFirst, *mSecond;
    Constraint(float minLen = 10.0f, float stiffness = 1.0f, float damping = 0.0f);
    void Solve(RigidBody* b1, RigidBody* b2);
    void Solve() {Solve(mFirst, mSecond);}
    void SetAttributes(float minLen, float stiffness, float damping);
    float GetMinLength()                {return mMinLength;}
    float GetStiffness()                {return mStiffness;}
    float GetDamping()                  {return mDamping;}
    void SetMinLength(float l)          {mMinLength = l;}
    void SetStiffness(float s)          {mStiffness = s;}
    void SetDamping(float d)            {mDamping = d;}
    void SetBodies(RigidBody* first, RigidBody* second) {mFirst = first; mSecond = second;}
};

}

#endif // CONSTRAINT_H_INCLUDED
