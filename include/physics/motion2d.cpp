#include "motion2d.h"
namespace solstice {
Vector2f HermiteSpline(Vector2f start, Vector2f end, float t) {
    Vector2f ret;
    float distance = start.DistanceSqrt(end);
    Vector2f startTan(0.5f * distance, distance);
    Vector2f endTan(-distance * 0.5f, 0);
    ret.x = HermiteH0(start.x, t) + HermiteH1(startTan.x, t) + HermiteH2(end.x, t) + HermiteH3(endTan.x, t);
    ret.y = HermiteH0(start.y, t) + HermiteH1(startTan.y, t) + HermiteH2(end.y, t) + HermiteH3(endTan.y, t);
    return ret;
}
float HermiteH0(float p0, float t) {
    return (2 * t * t * t - 3 * t * t + 1) * p0;
}
float HermiteH1(float m0, float t) {
    return (t * t * t - 2 * t * t + t) * m0;
}
float HermiteH2(float p1, float t) {
    return (-2 * t * t * t + 3 * t * t) * p1;
}
float HermiteH3(float m1, float t) {
    return (t * t * t - t * t) * m1;
}
Motion::Motion() {
    mSpeed = 0;
    mCurStep = 0;
}
Motion::Motion(Vector2f start, Vector2f end, float spd) {
    mCurStep = 0;
    mEndPoint = end;
    mPosition = start;
    mStartPoint = start;
    SetSpeed(spd);
}
void Motion::Update() {
    mPosition = HermiteSpline(mStartPoint, mEndPoint, mCurStep);
    if(mCurStep < 1.0f) {
        mCurStep+= mStep;
    }
}
void Motion::SetStartPoint(Vector2f start) {
    mStartPoint = start;
    mCurStep = 0;
    CalcStep();
}
void Motion::SetEndPoint(Vector2f end) {
    if(mEndPoint != end) {
        if(mCurStep >= 1) {
            mPosition = end;
            mStartPoint = end;
            mEndPoint = end;
        }
        else {
            mStartPoint = mPosition;
            mEndPoint = end;
            CalcStep();
        }
    }
}
void Motion::SetSpeed(float speed) {
    mSpeed = speed;
    CalcStep();
}
void Motion::CalcStep() {
    mStep = 1.0f / (mStartPoint.DistanceSqrt(mEndPoint) / mSpeed);
}
}
