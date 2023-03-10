// TODO (Stephen#1#): Test Cursor.h

#ifndef CURSOR_H_INCLUDED
#define CURSOR_H_INCLUDED

#include "globals.h"
#include "graphics/quaddata.h"
namespace solstice {
class Cursor : public QuadData
{
private:
    bool mHidden;
    float mXOff, mYOff;
    short mTexHeight, mTexWidth;
    float mXAdjust, mYAdjust;
    bool mCursorTrail;
    Vector2<float> mPrevPos;
    Vector2<float> mClickPoint, mClickOffset;
    bool mSpawn;
    void AdjustClickPoint();
    //ParticleSprayer mTrail;
public:

    Cursor();
    Cursor(Vector2<float> origin, short height, short width, short texID);

    void Initialize(Vector2<float> origin, short height, short width, short texID);

    float GetXOfffset();
    float GetYOffset();
    float GetXAdjust();
    float GetYAdjust();
    short GetTexHeight();
    short GetTexWidth();
    bool IsHidden();
    bool HasTrail();
    Vector2<float> GetClickPoint();
    Vector2<float> GetClickOffset();

    void SetXY(Vector2<float> point);
    void SetHidden(bool flag);
    void SetXOffset(float off);
    void SetYOffset(float off);
    void SetXYOffset(float x, float y);
    void SetTrail(bool t);
    void SetClickPoint(Vector2<float> pt);
    void SetClickOffset(Vector2<float> pt);

    void Update(SDL_Event event);
    void Update();
};
};
#endif // CURSOR_H_INCLUDED
