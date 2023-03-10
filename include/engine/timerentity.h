#ifndef TIMERENTITY_H_INCLUDED
#define TIMERENTITY_H_INCLUDED

namespace solstice {
    struct TimerEntity {
    private:
        int mLoopsPerformed;
    public:
        float mElapsedTime, mDuration;
        int mLoops;
        TimerEntity(float dur = -1, int loops = -1) {
            Init(dur, loops);
        }
        void Init(float dur, int loops = -1) {
            mDuration = dur;
            mLoops = loops;
            mElapsedTime = 0;
            mLoopsPerformed = 0;
        }
        bool Update(double dt) {
            if(mDuration > 0) {
                if(mElapsedTime < mDuration) {
                    mElapsedTime+=dt;
                }
                if(mElapsedTime >= mDuration && (mLoopsPerformed < mLoops || mLoops <= -1)) {
                    mLoopsPerformed++;
                    mElapsedTime = 0;
                    return true;
                }
                else {
                    return false;
                }
            }
            return true;
        }
        bool Finish() {
            mElapsedTime = mDuration;
        }
        bool Finished() {
            if(mDuration > 0) {
                if(mElapsedTime >= mDuration && (mLoopsPerformed >= mLoops || mLoops <= -1)) {
                    return true;
                }
                else {
                    return false;
                }
            }
            return true;
        }
        void Reset() {mLoopsPerformed = 0; mElapsedTime = 0;}

    };
}

#endif // TIMERENTITY_H_INCLUDED
