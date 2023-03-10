#include "behavior.h"
namespace solstice {
Behavior::Behavior() {
    mBehaviorType = BEHAVIOR_MAX;
    mEventType = -1;
    mStartFrame = -1;
    mActivateChance = 1;
    mTurnsOnCollision = true;
}
Behavior::Behavior(TimerEntity timer) {
    mBehaviorType = BEHAVIOR_MAX;
    mTimer = timer;
    mEventType = -1;
    mActivateChance = 1;
    mStartFrame = -1;
    mTurnsOnCollision = true;
}
Behavior::Behavior(int behaviorType, BehaviorMotion motion, TimerEntity timer) {
    mBehaviorType = behaviorType;
    mTimer = timer;
    mMotion = motion;
    mEventType = -1;
    mActivateChance = 1;
    mStartFrame = -1;
    mTurnsOnCollision = true;
}

void Behavior::Update(double dt, MovableObject* obj) {
    mTimer.Update(dt);
    mFinished = mTimer.Finished();
}
}
