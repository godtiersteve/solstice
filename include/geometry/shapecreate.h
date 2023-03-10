#ifndef SHAPECREATE_H_INCLUDED
#define SHAPECREATE_H_INCLUDED

#include "polygon.h"

namespace solstice {
    Polygonf CreateSlope(Vector2f origin, float height, float width, float angle, bool facingRight = true);
}

#endif // SHAPECREATE_H_INCLUDED
