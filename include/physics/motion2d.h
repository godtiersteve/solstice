#ifndef MOTION2D_H_INCLUDED
#define MOTION2D_H_INCLUDED

#include "graphics/vector2.h"
#include "utility/sol_math.h"

namespace solstice {
Vector2f HermiteSpline(Vector2f start, Vector2f end, float t);
inline float HermiteH0(float p0, float t);
inline float HermiteH1(float m0, float t);
inline float HermiteH2(float p1, float t);
inline float HermiteH3(float m1, float t);
class Motion {
private:
    float mSpeed;
    Vector2f mEndPoint, mStartPoint;
    Vector2f mPosition;
    float mStep, mCurStep;
    void CalcStep();
public:
    Motion();
    Motion(Vector2f start, Vector2f end, float spd);
    void Update();

    bool Finished()             {return mCurStep >= 1;}
    Vector2f GetPosition()      {return mPosition;}
    Vector2f GetEndPoint()      {return mEndPoint;}
    Vector2f GetStartPoint()    {return mStartPoint;}
    float GetSpeed()            {return mSpeed;}
    float GetStep()             {return mStep;}
    float GetCurStep()          {return mCurStep;}

    void SetPosition(Vector2f position) {mPosition = position;}
    void SetStartPoint(Vector2f start);
    void SetEndPoint(Vector2f end);
    void SetSpeed(float speed);
    void SetCurStep(float step) {mCurStep = step;}
};
}
#endif // MOTION2D_H_INCLUDED
