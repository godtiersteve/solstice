#include "cursor.h"
namespace solstice {
Cursor::Cursor():
mHidden(false), mXOff(0), mYOff(0),
mTexHeight(32), mTexWidth(32), mXAdjust(1.0f / 16.0f), mYAdjust(1.0f /16.0f),
mCursorTrail(false), mSpawn(false)
{
    SetXYHW(Vector2<float>(0), 32, 32);
    SetClickPoint(0);
}
Cursor::Cursor(Vector2<float> origin, short height, short width, short texID):
mHidden(false), mXOff(0), mYOff(0),
mCursorTrail(false), mSpawn(false)
{
    Initialize(origin, height, width, texID);
    SetClickPoint(0);
}
void Cursor::Initialize(Vector2<float> origin, short height, short width, short texID) {
    SetXYHW(origin, height, width);
    SetTextureID(texID);
    SDL_ShowCursor(0);
    mPrevPos = origin;
    SetClickPoint(0);
}

float Cursor::GetXOfffset()         {return mXOff;}
float Cursor::GetYOffset()          {return mYOff;}
float Cursor::GetXAdjust()          {return mXAdjust;}
float Cursor::GetYAdjust()          {return mYAdjust;}
short Cursor::GetTexHeight()        {return mTexHeight;}
short Cursor::GetTexWidth()         {return mTexWidth;}
bool Cursor::IsHidden()             {return mHidden;}
bool Cursor::HasTrail()             {return mCursorTrail;}
Vector2<float> Cursor::GetClickPoint()    {return mClickPoint;}
Vector2<float> Cursor::GetClickOffset()   {return mClickOffset;}

void Cursor::SetXY(Vector2<float> point) {
    Vector2<float> foo;
    foo.x = point.x + mXOff;
    foo.y = point.y + mYOff;
    SetXY(foo);
    mPrevPos = GetOrigin();
}
void Cursor::SetHidden(bool flag)           {mHidden = flag;}
void Cursor::SetXOffset(float off)          {mXOff = off;}
void Cursor::SetYOffset(float off)          {mYOff = off;}
void Cursor::SetXYOffset(float x, float y)  {mXOff = x; mYOff = y;}
void Cursor::SetTrail(bool t)               {mCursorTrail = t;}
void Cursor::SetClickPoint(Vector2<float> pt)     {mClickPoint = pt;}
void Cursor::SetClickOffset(Vector2<float> pt)   {
    mClickOffset = pt;
    mClickPoint.x = pt.x + GetOrigin().x;
    mClickPoint.y = pt.y + GetOrigin().y;
}

void Cursor::AdjustClickPoint() {
    mClickPoint.x = mClickOffset.x + GetOrigin().x;
    mClickPoint.y = mClickOffset.y + GetOrigin().y;
}
void Cursor::Update(SDL_Event event) {
    if(event.type == SDL_MOUSEMOTION) {
        int x, y;
        Vector2<float> foo;
        SDL_GetMouseState(&x, &y);
        foo.x = x;
        foo.y = INTERNAL_RESOLUTION_HEIGHT - y ;
        mSpawn = true;
        SetXY(foo);
        AdjustClickPoint();
    }
}

void Cursor::Update() {
    Update();
}
};
