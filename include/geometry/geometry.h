#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED


#include "shape.h"
#include "hitbox.h"
#include "collision.h"
#include "polygon.h"
#include "rectangle.h"
#include "circle.h"
#include "edge.h"
#include "utility/sol_math.h"
#include "hitdetection.h"

#include <iostream>
#include <vector>
#include <cfloat>
#include <stdio.h>

namespace solstice {
struct GridCell {
    int left, right, top, bottom;
    GridCell() {left = right = top = bottom = -1;}
    inline bool OutOfRange(int x, int y) {return left < 0 || right > x || top > y || bottom < 0;}
    inline bool InRange(int x, int y);
    operator bool() {return right >= 0 && top >= 0;}
};
GridCell GetCells(int minX, int minY, int celSize, Rectanglef& rect);
}
#endif // GEOMETRY_H_INCLUDED
