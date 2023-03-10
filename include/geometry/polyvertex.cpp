#include "polyvertex.h"

namespace solstice {
PolygonVertex::PolygonVertex() {
    mLeft = NULL;
    mRight = NULL;
}
PolygonVertex::PolygonVertex(float xy) {
    x = xy;
    y = xy;
}
PolygonVertex::PolygonVertex(PolygonVertex* left, PolygonVertex* right) {
    mRight = right;
    mLeft = left;
}
PolygonVertex::PolygonVertex(Vector2f vt) {
    *this = vt;
}
PolygonVertex* PolygonVertex::GetLeft()             {return mLeft;}
PolygonVertex* PolygonVertex::GetRight()            {return mRight;}

void PolygonVertex::SetLeft(PolygonVertex* left)     {mLeft = left;}
void PolygonVertex::SetRight(PolygonVertex* right)   {mRight = right;}
PolygonVertex& PolygonVertex::operator=(Vector2f vt) {
    x = vt.x;
    y = vt.y;
    mLeft = NULL;
    mRight = NULL;
    return *this;
}
};
