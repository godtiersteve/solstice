#include "line.h"

namespace solstice {
Line::Line():
mLength(0), mXSlope(1), mYSlope(0), mAngle(0),
mVisible(true), mOrientation(LORI_HLINE)
{
}

Line::Line(float angle, int length, Vector2<float> origin) {
    Init(angle, length, origin);
}
void Line::Init(float angle, int length, Vector2<float> origin) {
    mAngle = angle;
    mLength = length;
    mXSlope = cos(angle);
    mYSlope = sin(angle);
    SetPosition(origin);
    GenBoundingRect();
    if(mXSlope > 0) {
        if(mYSlope > 0) {
            mOrientation = LORI_URIGHT;
        }
        else if(mYSlope < 0) {
            mOrientation = LORI_DRIGHT;
        }
        else {
            mOrientation = LORI_HLINE;
        }
    }
    else if(mXSlope < 0) {
        if(mYSlope > 0) {
            mOrientation = LORI_ULEFT;
        }
        else if(mYSlope < 0) {
            mOrientation = LORI_DLEFT;
        }
        else {
            mOrientation = LORI_HLINE;
        }
    }
    else {
        mOrientation = LORI_VLINE;
    }
}

void Line::SetAngle(float a)  {
    Init(a, mLength, mStartPoint);
}
void Line::SetLength(int l)   {
    Init(mAngle, l, mStartPoint);
}
void Line::SetOrigin(Vector2<float> o) {
    Init(mAngle, mLength, o);
}
void Line::SetPosition(Vector2<float> pos) {
    float endX, endY;
    endX = pos.x + (mLength * mXSlope);
    endY = pos.y + (mLength * mYSlope);
    mEndPoint = Vector2<float>(endX, endY);
    mStartPoint = pos;
}
void Line::SetVisible(bool v)         {mVisible = v;}
void  Line::SetColor(Color c)         {mColor = c;}

Vector2<float>        Line::GetSlope()          {return Vector2<float>(mXSlope, mYSlope);}
Vector2<float>        Line::GetStartPoint()     {return mStartPoint;}
Vector2<float>        Line::GetEndPoint()       {return mEndPoint;}
Rectangle<float>      Line::GetBoundingRect()   {return mBoundingRect;}
Color&          Line::GetColor()          {return mColor;}
bool            Line::GetVisible()        {return mVisible;}
int             Line::GetLength()         {return mLength;}
int             Line::GetHeight()         {return mEndPoint.y - mStartPoint.y;}
int             Line::GetWidth()          {return mEndPoint.x - mStartPoint.x;}
float           Line::GetX()              {return mStartPoint.x;}
float           Line::GetY()              {return mStartPoint.y;}
float           Line::GetAngle()          {return mAngle;}
float           Line::GetXSlope()         {return mXSlope;}
float           Line::GetYSlope()         {return mYSlope;}
LineOrientation Line::GetOrientation()    {return mOrientation;}
bool Line::Empty() {
    return mLength;
}
void Line::clear() {
    mLength = 0;
}
void Line::GenBoundingRect() {
    int height = mEndPoint.y - mStartPoint.y;
    int width = mEndPoint.x - mStartPoint.x;
    mBoundingRect.SetXYHW(mStartPoint, height, width);
}

void Line::Draw() {
    if(mVisible) {
        GLfloat vertices[4] = {mStartPoint.x, mStartPoint.y,
                               mEndPoint.x, mEndPoint.y
                              };
        GLfloat colors[16];
        mColor.ToArray(colors);
        GLubyte indices[] = {0, 1, 2};
        glColorPointer(4, GL_FLOAT, 0, colors);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, indices);
    }
    else
        return;
}

void Line::Draw(Vector2<float> pt) {
    Vector2<float> origin = GetStartPoint();
    SetPosition(pt);
    Draw();
    SetPosition(origin);
}

int Line::GetYFromX(float x) {
    x = x - GetX();
    return ((x / mXSlope) * mYSlope) + mStartPoint.y;
}

int Line::GetXFromY(float y) {
    y = y - GetY();
    return (y / mYSlope) * mXSlope + mStartPoint.x;
}
};
