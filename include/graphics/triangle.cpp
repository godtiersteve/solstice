#include "triangle.h"

namespace solstice {
Triangle::Triangle():
mHeight(0), mWidth(0)
{
}
Triangle::Triangle(float hangle, int width, Vector2<float> origin, bool line)
{
    if(line) {
        Init(hangle, width, origin);
    }
    else {
        InitTri(hangle, width, origin);
    }
}
void Triangle::SetHeightWidth(int h, int w) {
    Init(h, w, mStartPoint);
}
void Triangle::InitTri(int h, int w, Vector2<float> origin) {
    mHeight = h;
    mWidth = w;
    Vector2<float> end(origin.x + w, origin.y + h);
    int length = Distance(origin, end);
    float angle = AngleBetween2(origin, end);
    Init(angle, length, origin);
}
void Triangle::Init(float angle, int length, Vector2<float> origin) {
    Line::Init(angle, length, origin);
    mHeight = mEndPoint.y - mStartPoint.y;
    mWidth = mEndPoint.x - mStartPoint.x;
}

void Triangle::SetHeight(int h) {
    Init(h, mWidth, mStartPoint);
}
void Triangle::SetWidth(int w) {
    Init(mHeight, w, mStartPoint);
}
void Triangle::SetAngle(float a)  {
    Init(a, mLength, mStartPoint);
}
void Triangle::SetLength(int l)   {
    Init(mAngle, l, mStartPoint);
}
void Triangle::SetOrigin(Vector2<float> o) {
    Init(mAngle, mLength, o);
}

int Triangle::GetHeight()   {return mHeight;}
int Triangle::GetWidth()    {return mWidth;}

void Triangle::Draw() {
    if(mVisible) {
        GLfloat vertices[6] = {mStartPoint.x, mStartPoint.y,
                               mEndPoint.x, mStartPoint.y,
                               mEndPoint.x, mEndPoint.y
                              };
        GLfloat colors[16];
        mColor.ToArray(colors);
        GLubyte indices[] = {0, 1, 2};
        glColorPointer(4, GL_FLOAT, 0, colors);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices);
    }
    else
        return;
}
};
