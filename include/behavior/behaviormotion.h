#ifndef BEHAVIORMOTION_H_INCLUDED
#define BEHAVIORMOTION_H_INCLUDED

namespace solstice {
enum BehaviorMotionType {ENMOTION_NONE, ENMOTION_STOPS, ENMOTION_SLOWS, ENMOTION_MOVES};
struct BehaviorMotion {
protected:
    Vector2i mDirection;
public:
    int mType;
    bool mFlipped, mStopsMomentum;
    int mPriority;
    Vector2f mVelocity;
    Vector2f mAcceleration;
    BehaviorMotion() {
        mType = ENMOTION_MOVES;
        mFlipped = false;
        mStopsMomentum = false;
    }
    BehaviorMotion(Vector2f vel, int t) {
        mVelocity = vel;
        SetDirection(vel);
        mType = t;
        mFlipped = false;
        mStopsMomentum = false;
    }
    Vector2f GetVelocity() {
        Vector2f vel = mVelocity;
        Vector2f dir = mFlipped ? -mDirection : mDirection;
        if(dir.x) {
            vel.x*= dir.x;
        }
        if(dir.y) {
            vel.y*= dir.y;
        }
        return vel;
    }
    void SetVelocityAndDir(Vector2f vel) {
        mVelocity = vel;
        mDirection = 1;
        //SetDirection(vel);
    }
    void SetVelocity(Vector2f vel) {
        mVelocity = vel;
    }
    void SetDirection(Vector2f dir) {
        if(dir.x > 0) {
            mDirection.x = 1;
        }
        else if(dir.x < 0) {
            mDirection.x = -1;
        }
        else {
            mDirection.x = 0;
        }
        if(dir.y > 0) {
            mDirection.y = 1;
        }
        else if(dir.y < 0) {
            mDirection.y = -1;
        }
        else {
            mDirection.y = 0;
        }
    }
    Vector2i GetDirection() {return mDirection;}
    void Flip() {mFlipped = !mFlipped;}
};
}
#endif // BEHAVIORMOTION_H_INCLUDED
