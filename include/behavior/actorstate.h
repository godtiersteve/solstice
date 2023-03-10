#ifndef ACTORSTATE_H_INCLUDED
#define ACTORSTATE_H_INCLUDED

#include <string>
#include "graphics/vector2.h"

namespace solstice {

struct ActorMotion {
    Vector2f mVelocity, mAcceleration;
};
class ActorState {
public:
    std::string mName;
    std::string mAnimationRef;
    int mAnimationID;
    ActorMotion mMotion;
    ActorState() {
        mAnimationID = -1;
    }
    ActorState(std::string animRef, std::string name) {
        mAnimationID = -1;
        mAnimationRef = animRef;
        mName = name;
    }
};
}


#endif // ACTORSTATE_H_INCLUDED
