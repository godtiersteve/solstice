#ifndef WALKBEHAVIOR_H_INCLUDED
#define WALKBEHAVIOR_H_INCLUDED

#include "behavior.h"

namespace solstice {
    class WalkBehavior : public Behavior {
    private:
        float mDistanceTraveled, mDistanceToTravel;
    public:
        float mHopTimer, mHopStrength;
        bool mTurnsAtEnd;
        WalkBehavior();
        WalkBehavior(Vector2f vel, float distance, bool turns = true, float hopTimer = 0, float hopStrength = 0) {Init(vel, distance, turns, hopTimer, hopStrength); mName = "Walking";}
        void Init(Vector2f vel, float distance, bool turns = true, float hopTimer = 0, float hopStrength = 0);
        void Update(double dt, MovableObject* obj);
        float GetDistanceTraveled()         {return mDistanceTraveled;}
        void Reverse();
        virtual void Reset() {Behavior::Reset(); mDistanceTraveled = 0;}
        void SetDistanceToTravel(float d) {mDistanceToTravel = d; Behavior::Reset();}

    };
}

#endif // WALKBEHAVIOR_H_INCLUDED
