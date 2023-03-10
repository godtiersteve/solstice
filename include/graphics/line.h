#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include <cmath>

#include "graphics/vector2.h"
#include "geometry/rectangle.h"
namespace solstice {
enum LineOrientation {LORI_DRIGHT, LORI_DLEFT, LORI_URIGHT, LORI_ULEFT, LORI_HLINE, LORI_VLINE};
class Line {
protected:
    int mLength;
    float mXSlope, mYSlope;
    float mAngle;
    Vector2<float> mStartPoint, mEndPoint;
    Rectangle<float> mBoundingRect;
    Color mColor;
    bool mVisible;
    LineOrientation mOrientation;

    void GenBoundingRect();
public:
    Line();
    Line(float angle, int length, Vector2<float> origin);
    void Init(float angle, int length, Vector2<float> origin);

    void SetAngle(float a);
    void SetLength(int l);
    void SetOrigin(Vector2<float> o);
    void SetPosition(Vector2<float> pos);
    void SetVisible(bool v);
    void SetColor(Color c);

    int GetLength();
    float GetAngle();
    float GetXSlope();
    float GetYSlope();
    Vector2<float> GetSlope();
    Vector2<float> GetStartPoint();
    Vector2<float> GetEndPoint();
    Rectangle<float> GetBoundingRect();
    Color& GetColor();
    bool GetVisible();
    float GetX();
    float GetY();
    LineOrientation GetOrientation();

    bool Empty();
    void clear();
    int GetHeight();
    int GetWidth();

    int GetYFromX(float x);
    int GetXFromY(float y);
    Line GetNormal();


    void Draw();
    void Draw(Vector2<float> pt);
};
};
#endif // LINE_H_INCLUDED
