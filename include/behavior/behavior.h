#ifndef ENEMYBEHAVIOR_H_INCLUDED
#define ENEMYBEHAVIOR_H_INCLUDED

#include <string>
#include <vector>
#include "bullets/bullettype.h"
#include "player/frameinfo.h"
#include "physics/movableobject.h"
#include "engine/timerentity.h"
#include "behaviormotion.h"
#include "actorstate.h"

namespace solstice {
enum BehaviorType {BEHAVIOR_IDLE, BEHAVIOR_WALK, BEHAVIOR_JUMP, BEHAVIOR_HURT, BEHAVIOR_KNOCKDOWN,
                        BEHAVIOR_ATTACK, BEHAVIOR_SHOOT, BEHAVIOR_TRACKING, BEHAVIOR_FLEEING,
                        BEHAVIOR_SWIM, BEHAVIOR_HURL, BEHAVIOR_DIVE,
                        BEHAVIOR_MAX};
enum BehaviorPriority {BEHAVIOR_PRIORITY_LOWEST, BEHAVIOR_PRIORITY_LOW, BEHAVIOR_PRIORITY_MID, BEHAVIOR_PRIORITY_HIGH, BEHAVIOR_PRIORITY_SEVERE, BEHAVIOR_PRIORITY_MAX};

class Behavior {
protected:
    bool mFinished;
    int mBehaviorType;
public:
    int mStartFrame;
    TimerEntity mTimer;
    float mActivateChance;
    int mPriority;
    int mEventType;
    std::vector<int> mEventList;
    std::string mName;
    int mActivationType;
    Vector2f mDestination;
    bool mTurnsOnCollision;
    int mLoops;
    BehaviorMotion mMotion;
    FrameInfo mFrameData;
    Vector2f mActionPoint;
    ActorState mState;

    Behavior();
    Behavior(TimerEntity timer);
    Behavior(int behaviorType, BehaviorMotion motion, TimerEntity timer);

    virtual void Reverse() {mMotion.Flip();}
    virtual void Update(double dt, MovableObject* obj);
    bool Finished()             {return mFinished;}
    virtual void Reset()        {mTimer.Reset(); mFinished = false; mMotion.SetVelocityAndDir(mMotion.mVelocity);}
    float GetElapsedTime()      {return mTimer.mElapsedTime;}
    int GetBehaviorType()       {return mBehaviorType;}
    bool IsBehavior(int type)   {return mBehaviorType == type;}
};
struct FindBehaviorEvent : public std::unary_function<std::shared_ptr<Behavior>, bool> {
    int event;
    FindBehaviorEvent(int ev) {event = ev;}
    bool operator()(const std::shared_ptr<Behavior>& behavior) const {return behavior->mEventType == event;}
};
}


#endif // ENEMYBEHAVIOR_H_INCLUDED
