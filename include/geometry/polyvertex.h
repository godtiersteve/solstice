#ifndef AXIS_H_INCLUDED
#define AXIS_H_INCLUDED

#include "graphics/vector2.h"

namespace solstice {
class PolygonVertex : public Vector2f {
protected:
    PolygonVertex *mLeft, *mRight;
public:
    PolygonVertex();
    PolygonVertex(float xy);
    PolygonVertex(Vector2f vt);
    PolygonVertex(PolygonVertex* left, PolygonVertex* right);
    PolygonVertex* GetLeft();
    PolygonVertex* GetRight();
    PolygonVertex& operator=(Vector2f vt);
    operator Vector2f() const {return Vector2f(x, y);}

    void SetLeft(PolygonVertex* left);
    void SetRight(PolygonVertex* right);
};
};


#endif // AXIS_H_INCLUDED
