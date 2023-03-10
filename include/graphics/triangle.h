#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include "line.h"
#include "line_funcs.h"
namespace solstice {
class Triangle : public Line {
protected:
    int mHeight, mWidth;
public:
    Triangle();
    Triangle(float hangle, int width, Vector2<float> origin, bool line = false);

    void SetHeightWidth(int h, int w);
    void InitTri(int h, int w, Vector2<float> origin);
    void Init(float angle, int length, Vector2<float> origin);

    void SetHeight(int h);
    void SetWidth(int w);
    void SetOrigin(Vector2<float> o);
    void SetLength(int l);
    void SetAngle(float a);

    int GetHeight();
    int GetWidth();

    void Draw();
};
};
#endif // TRIANGLE_H_INCLUDED
